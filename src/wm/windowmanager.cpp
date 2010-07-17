#include "windowmanager.hpp"
#include "decoration.hpp"
#include "client.hpp"
#include "atoms.hpp"
#include "debug.hpp"

#include <fixx11h.h>
#include <QDebug>
#include <QX11Info>

Q_GLOBAL_STATIC(WindowManager, windowManager);

WindowManager::WindowManager()
    : QObject()
{
}

void WindowManager::init()
{
    ::createAtomList();

    XClearWindow(QX11Info::display(), QX11Info::appRootWindow());
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(), SubstructureRedirectMask);
    XSync(QX11Info::display(), False);
    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(),
                 SubstructureNotifyMask | SubstructureRedirectMask |
                 StructureNotifyMask | ButtonPressMask | PropertyChangeMask|
                 KeyPressMask | ColormapChangeMask |EnterWindowMask);
    XSync(QX11Info::display(), False);

    // Some apps don't have a cursor
    XDefineCursor(QX11Info::display(), QX11Info::appRootWindow(), QCursor(Qt::ArrowCursor).handle());
}

bool WindowManager::x11EventFilter(_XEvent *e)
{
    static int debugs = 0;
    qDebug() << ++debugs << __PRETTY_FUNCTION__ << "got event" << ::eventName(e->type);

    switch (e->type)
    {
        case DestroyNotify:
        {
            Client *c = findClient(e->xdestroywindow.window);
            if (c)
            {
                _clients.remove(e->xdestroywindow.window);
                c->deleteLater();
                return true;
            }

            if (e->xdestroywindow.event != e->xdestroywindow.window)
            {
                return true;
            }

            return false;
        }

        case MapNotify:
            if (e->xmap.event != e->xmap.window)
                return true;
            return false;

        case UnmapNotify:
        {
            Client *c = findClient(e->xunmap.window);
            if (c)
            {
                if (e->xunmap.send_event)
                {
                }

                return true;
            }

            if (e->xunmap.event != e->xunmap.window)
            {
                return true;
            }

            return false;
        }

        case MapRequest:
        {
            Client *c = findClient(e->xmaprequest.window);
            if (c)
            {
                c->map();
                return true;
            }

            if ((c = createClient(e->xmaprequest.window)))
                _clients.insert(e->xmaprequest.window, c);

            return true;
        }
    }

    if (Client *c = findClient(eventWindow(e)))
    {
        if (c->x11EventFilter(e))
            return true;
    }
    else if (Client *c = findClientByDecorationWindow(eventWindow(e)))
    {
        if (c->decoration()->x11EventFilter(e))
            return true;
    }

    return false;
}

void WindowManager::setActiveClient(Client *client)
{
    foreach (Client *c, _clients)
    {
        if (c == client)
        {
            XRaiseWindow(QX11Info::display(), client->wrapper()->winId());

            c->setActive();
        }
        else
            c->setInactive();
    }
}

bool WindowManager::compositeManagerIsRunning() const
{
    Atom atom = XInternAtom(QX11Info::display(),
                            QString("_NET_WM_CM_S%1").arg(QX11Info::appScreen()).toLatin1(),
                            False);

    return XGetSelectionOwner(QX11Info::display(), atom) != None;
}

Qt::HANDLE WindowManager::eventWindow(_XEvent *e) const
{
    Qt::HANDLE winId;

    switch (e->type)
    {
        case ConfigureRequest:
            winId = e->xconfigurerequest.window;
            break;
        default:
            winId = e->xany.window;
    }

    return winId;
}

Client *WindowManager::createClient(Qt::HANDLE winId)
{
    return new Client(winId, this);
}

Client *WindowManager::findClientByDecorationWindow(Qt::HANDLE winId) const
{
    foreach (Client *c, _clients)
    {
        if (c->decoration()->winId() == winId)
            return c;
    }

    return NULL;
}

WindowManager *WindowManager::self()
{
    return windowManager();
}
