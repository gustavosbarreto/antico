// Self
#include <paneltaskwatcher.h>

// Qt
#include <QApplication>
#include <QX11Info>
#include <QMap>

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
        static const char *atoms[] = {
            "_NET_CLIENT_LIST\0"
            "_NET_ACTIVE_WINDOW\0"
            "_NET_WM_WINDOW_TYPE\0"
            "_NET_WM_WINDOW_TYPE_DESKTOP\0"
            "_NET_WM_WINDOW_TYPE_DOCK\0"
            "_NET_WM_WINDOW_TYPE_SPLASH\0"
            "_NET_WM_WINDOW_TYPE_NORMAL\0"
            "_NET_WM_WINDOW_TYPE_DIALOG\0"
            "_NET_WM_STATE\0"
            "_NET_WM_STATE_STICKY\0"
            "_NET_WM_STATE_SKIP_PAGER\0"
            "_NET_WM_STATE_SKIP_TASKBAR\0"
            "_NET_WM_STRUT_PARTIAL\0"
            "_NET_SYSTEM_TRAY_OPCODE\0"
            "_NET_WM_NAME\0"
            "_NET_WM_ICON\0"
            "WM_DELETE_WINDOW\0"
            "WM_PROTOCOLS\0"
            "WM_STATE\0"
            "UTF8_STRING\0"
        };

        int size = sizeof(atoms) / sizeof(char *);
        Atom atomsReturn[size];

        XInternAtoms(QX11Info::display(), (char **)atoms, size, false, atomsReturn);

        for (int i = 0; i < size; ++i)
            atomMap.insert(atoms[i], atomsReturn[i]);
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

        int result = XGetWindowProperty(QX11Info::display(), winId, atom("NET_WM_WINDOW_TYPE"), 0L, sizeof(Atom), False,
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
                q->taskAdded(task); // emit
            }
            else
            {
                int *value = &refCount[task];
                (*value)++; // Increment ref count for this task
            }

            // Remove tasks that got closed
            // --> Yes, ref count is really fast than getting the client list from the WM
            QMutableMapIterator<Qt::HANDLE, TaskWindow *> it(tasks);
            while (it.hasNext())
            {
                it.next();
                TaskWindow *t = it.value();
                if (!refCount.contains(t))
                    refCount.insert(t, 1); // initial value

                int *value = &refCount[t];
                (*value)--;
                if ((*value) < 0)
                {
                    it.remove();
                    refCount.remove(t);
                    q->taskRemoved(t); // emit
                }
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
