// Self
#include <paneltaskwatcher.h>

// Qt
#include <QApplication>
#include <QX11Info>
#include <QMap>
#include <QDebug>

// Lib
#include <taskwindow.h>

// X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

// Standard lib
#include <values.h>

static PanelTaskWatcher *self = NULL;
static QCoreApplication::EventFilter oldX11EventFilter = NULL;

static bool x11EventFilter(void *message, long *result)
{
    XEvent *e = reinterpret_cast<XEvent *>(message);
    if (PanelTaskWatcher::instance()->x11EventFilter(e))
        return true;

    if (oldX11EventFilter && oldX11EventFilter != &x11EventFilter)
        return oldX11EventFilter(message, result);
    else
        return false;
}

class PanelTaskWatcher::Private
{
public:
    Private(PanelTaskWatcher *parent): q(parent)
    {
        const char *atoms[] = {
            "_NET_CLIENT_LIST",
            "_NET_ACTIVE_WINDOW",
            "_NET_WM_WINDOW_TYPE",
            "_NET_WM_WINDOW_TYPE_DESKTOP",
            "_NET_WM_WINDOW_TYPE_DOCK",
            "_NET_WM_WINDOW_TYPE_SPLASH",
            "_NET_WM_WINDOW_TYPE_NORMAL",
            "_NET_WM_WINDOW_TYPE_DIALOG",
            "_NET_WM_STATE",
            "_NET_WM_STATE_STICKY",
            "_NET_WM_STATE_SKIP_PAGER",
            "_NET_WM_STATE_SKIP_TASKBAR",
            "_NET_WM_STRUT_PARTIAL",
            "_NET_SYSTEM_TRAY_OPCODE",
            "_NET_WM_NAME",
            "_NET_WM_ICON",
            "WM_DELETE_WINDOW",
            "WM_PROTOCOLS",
            "WM_STATE",
            "UTF8_STRING"
        };

        int size = sizeof(atoms) / sizeof(char *);
        Atom atomsReturn[size];

        XInternAtoms(QX11Info::display(), (char **)atoms, size, false, atomsReturn);

        for (int i = 0; i < size; ++i)
            atomMap.insert(atoms[i], atomsReturn[i]);

        oldX11EventFilter = qApp->setEventFilter(::x11EventFilter);
    }

    const Atom &atom(const char *name)
    {
        return atomMap[name];
    }

    bool handlePropertyNotify(_XEvent *e)
    {
        if (e->xproperty.window == QX11Info::appRootWindow())
        {
            if (e->xproperty.atom == atom("_NET_CLIENT_LIST"))
                updateTaskList();
            else if (e->xproperty.atom == atom("_NET_ACTIVE_WINDOW"))
                updateCurrentTask();
        }
        else
        {
            if (e->xproperty.atom == XA_WM_NAME  ||
                e->xproperty.atom == atom("_NET_WM_NAME"))
            {
                TaskWindow *task = tasks.value(e->xproperty.window);
                if (task)
                {
                    QString title = windowTitle(e->xproperty.window);
                    task->setTitle(title);
                }
            }
            else if (e->xproperty.atom == XA_WM_HINTS)
            {
                TaskWindow *task = tasks.value(e->xproperty.window);
                if (task)
                {
                    QPixmap icon = windowIcon(e->xproperty.window);
                    task->setIcon(icon);
                }
            }
        }

        return false;
    }

    Atom windowType(Qt::HANDLE winId)
    {
        Atom type = None;
        unsigned char *data = NULL;
        Atom typeReturned;
        int formatReturned;
        unsigned long count = 0;
        unsigned long unused;

        int result = XGetWindowProperty(QX11Info::display(), winId, atom("_NET_WM_WINDOW_TYPE"), 0L, sizeof(Atom), False,
                                        XA_ATOM, &typeReturned, &formatReturned, &count, &unused, &data);
        if (result != Success)
        {
            if (data) XFree(data);
            return None;
        }

        type = *(Atom *)data;
        XFree(data);

        return type;
    }

    Atom windowState(Qt::HANDLE winId)
    {
        Atom state = None;
        unsigned char *data = NULL;
        Atom typeReturned;
        int formatReturned;
        unsigned long count = 0;
        unsigned long unused;

        int result = XGetWindowProperty(QX11Info::display(), winId, atom("_NET_WM_STATE"), 0, 1, False, XA_ATOM,
                                        &typeReturned, &formatReturned, &count, &unused, &data);
        if (result != Success || data == NULL)
        {
            if (data) XFree(data);
            return None;
        }

        memcpy(&state, data, sizeof(state));
        XFree(data);

        return state;
    }

    QString windowTitle(Qt::HANDLE winId)
    {
        QString title;
        unsigned char *data = NULL;
        Atom typeReturned;
        int formatReturned;
        unsigned long count = 0;
        unsigned long unused;

        // EWMH
        int result = XGetWindowProperty(QX11Info::display(), winId,
                                    atom("_NET_WM_NAME"), 0, 1024, False, atom("UTF8_STRING"),
                                    &typeReturned, &formatReturned, &count, &unused, &data);
        if (result == Success && typeReturned == atom("UTF8_STRING") && formatReturned == 8)
        {
            title = QString::fromUtf8((const char *) data);
            XFree(data);
        }
        else
        {
            if (data)
            {
                XFree(data);
                data = NULL;
            }

            // Old X stuff
            result = XGetWindowProperty(QX11Info::display(), winId, XA_WM_NAME, 0, MAXLONG, False, XA_STRING,
                                        &typeReturned, &formatReturned, &count, &unused, &data);
            if (result == Success)
            {
                title = QString((char *) data);
                XFree(data);
            }
            else if (data)
                XFree(data);
        }

        return title;
    }

    QPixmap windowIcon(Qt::HANDLE winId)
    {
        unsigned char *data = NULL;
        Atom typeReturned;
        int formatReturned;
        unsigned long count;
        unsigned long unused;

        QPixmap icon;
        QSize size;

        if (XGetWindowProperty(QX11Info::display(), winId, atom("_NET_WM_ICON"), 0, 1, False, XA_CARDINAL,
                               &typeReturned, &formatReturned, &count, &unused, &data) == Success && data)
        {
            size.setWidth(data[0]);
            XFree(data);
            data = NULL;
        }
        else if (data)
        {
            XFree(data);
            data = NULL;
        }

        if (XGetWindowProperty(QX11Info::display(), winId, atom("_NET_WM_ICON"), 1, 1, False, XA_CARDINAL,
                           &typeReturned, &formatReturned, &count, &unused, &data) == Success && data)
        {
            size.setHeight(data[0]);
            XFree(data);
            data = NULL;
        }
        else if (data)
        {
            XFree(data);
            data = NULL;
        }

        if (XGetWindowProperty(QX11Info::display(), winId, atom("_NET_WM_ICON"),
                               2, size.width() * size.height(), False, XA_CARDINAL,
                               &typeReturned, &formatReturned, &count, &unused, &data) == Success && data)
        {
            QImage img(data, size.width(), size.height(), QImage::Format_ARGB32);
            icon = QPixmap::fromImage(img).scaled(16, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            XFree(data);
        }
        else
        {
            if (data) XFree(data);
            icon = QPixmap();
        }

        qDebug() << size;

        return icon;
    }

    void updateTaskList()
    {
        Window *clients = NULL;
        unsigned long count = 0;
        Atom typeReturned;
        int formatReturned;
        unsigned long unused;

        int result = XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(),
                                        atom("_NET_CLIENT_LIST"), 0, MAXLONG, False, XA_WINDOW,
                                        &typeReturned, &formatReturned, &count, &unused, (unsigned char **)&clients);

        if (result != Success || !clients)
        {
            if (clients) XFree(clients);
            return;
        }

        for (int i = 0; i < (int)count; ++i)
        {
            Atom type = windowType(clients[i]);
            Atom state = windowState(clients[i]);

            // skip
            if (type == atom("_NET_WM_WINDOW_TYPE_DESKTOP") ||
                type == atom("_NET_WM_WINDOW_TYPE_DOCK") ||
                type == atom("_NET_WM_WINDOW_TYPE_SPLASH"))
            {
                continue;
            }

            // skip
            if (state == atom("_NET_WM_STATE_SKIP_PAGER") ||
                state == atom("_NET_WM_STATE_SKIP_TASKBAR") ||
                state == atom("_NET_WM_STATE_STICKY"))
            {
                continue;
            }

            TaskWindow *task = tasks.value(clients[i]);
            if (!task)
            {
                task = new TaskWindow(clients[i]);
                task->setTitle(windowTitle(clients[i]));
                task->setIcon(windowIcon(clients[i]));
                tasks.insert(clients[i], task);

                refCount.insert(task, 1); // initial ref count value

                q->taskAdded(task); // emit
            }
            else
            {
                int *value = &refCount[task];
                (*value)++; // Increment ref count for this task
            }
        }

        // Remove tasks that got closed
        // --> Yes, ref count is really fast than getting the client list from the WM
        QMutableMapIterator<Qt::HANDLE, TaskWindow *> it(tasks);
        while (it.hasNext())
        {
            it.next();

            int *value = &refCount[it.value()];
            (*value)--;
            if ((*value) < 0)
            {
                it.remove();
                refCount.remove(it.value());
                q->taskRemoved(it.value()); // emit
            }
        }
    }

    void updateCurrentTask()
    {
    }

    PanelTaskWatcher *q;
    QMap<const char *, Atom> atomMap;
    QMap<Qt::HANDLE, TaskWindow *> tasks;
    QMap<TaskWindow *, int> refCount;
};

bool PanelTaskWatcher::x11EventFilter(_XEvent *e)
{
    if (e->type == PropertyNotify)
        return d->handlePropertyNotify(e);

    return false;
}

PanelTaskWatcher *PanelTaskWatcher::instance()
{
    if (!self) self = new PanelTaskWatcher;
    return self;
}

PanelTaskWatcher::PanelTaskWatcher():
    QObject()
{
    d = new Private(this);
}

void PanelTaskWatcher::emitTaskAdded(TaskWindow *t)
{
    emit taskAdded(t);
}

void PanelTaskWatcher::emitTaskRemoved(TaskWindow *t)
{
    emit taskRemoved(t);
}
