#include "client.hpp"
#include "windowmanager.hpp"
#include "decorations/ubuntu/ubuntudecoration.hpp"
#include "decorations/panther/pantherdecoration.hpp"
#include "atoms.hpp"
#include "fixx11h.h"

#include <QX11Info>
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

Client::Client(Qt::HANDLE winId, QObject *parent)
    : QObject(parent)
    , _winId(winId)
{
    XWindowAttributes attr;
    XGetWindowAttributes(QX11Info::display(), _winId, &attr);

    _geometry.setX(attr.x);
    _geometry.setY(attr.y);
    _geometry.setWidth(attr.width);
    _geometry.setHeight(attr.height);

    qDebug() << __PRETTY_FUNCTION__ << "Client window geometry" << _geometry;

    /*
      If you have a custom decoration, pass the pointer to it.
      Yes. I know it is ugly but there are no plans to add support for dynamic plugins (the custom decoration would be a dynamic library).
      Once compiled it will not be possible to change the decoration unless the decoration be a engine (like KWin)
    */

    _decoration = new UbuntuDecoration(this);

    // Allow composited clients
    if (WindowManager::self()->compositeManagerIsRunning())
        _decoration->setAttribute(Qt::WA_TranslucentBackground);

    XSetWindowBorderWidth(QX11Info::display(), _winId, 0);
    XSetWindowBorderWidth(QX11Info::display(), _decoration->winId(), 0);
    XReparentWindow(QX11Info::display(), _winId, _decoration->winId(),
                    _decoration->borderSize().left(),
                    _decoration->borderSize().top() + _decoration->borderSize().titleBarHeight());
    XAddToSaveSet(QX11Info::display(), _winId);

    XGrabButton(QX11Info::display(), Button1, AnyModifier, _winId, False,
                ButtonPressMask | ButtonReleaseMask,
                GrabModeSync, GrabModeAsync, None, None);

    _decoration->setGeometry(_geometry.x(), _geometry.y(),
                             _geometry.width() + _decoration->borderSize().measuredWidth(),
                             _geometry.height() + _decoration->borderSize().measuredHeight());

    qDebug() << __PRETTY_FUNCTION__ << "Decoration window geometry" << _decoration->geometry();

    XSelectInput(QX11Info::display(), _decoration->winId(),
                 KeyPressMask | KeyReleaseMask |
                 ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask |
                 PointerMotionMask | EnterWindowMask |
                 LeaveWindowMask | FocusChangeMask |
                 ExposureMask |StructureNotifyMask |
                 SubstructureRedirectMask | SubstructureNotifyMask);

    XSetWindowAttributes sattr;
    sattr.event_mask = ColormapChangeMask | PropertyChangeMask | StructureNotifyMask;
    XChangeWindowAttributes(QX11Info::display(), _winId, CWEventMask, &sattr);

    updateTitle();
    _decoration->setTitle(_title);

    map();
}

Client::~Client()
{
    _decoration->close();
    _decoration->deleteLater();
}

bool Client::x11EventFilter(_XEvent *e)
{
    switch (e->type)
    {
        case PropertyNotify:
            // Name has updated
            if (e->xproperty.atom == ATOM(WM_NAME) || e->xproperty.atom == ATOM(_NET_WM_NAME))
            {
                updateTitle();
                _decoration->setTitle(_title);
                return true;
            }

            break;

        case ConfigureNotify:
            _geometry.setX(e->xconfigure.x);
            _geometry.setY(e->xconfigure.y);
            _geometry.setWidth(e->xconfigure.width);
            _geometry.setHeight(e->xconfigure.height);

            qDebug() << __PRETTY_FUNCTION__ << "ConfigureNotify: received geometry" << _geometry;

            break;

        case ConfigureRequest:
            if (e->xconfigurerequest.value_mask & (CWWidth | CWHeight | CWX | CWY))
            {
                QRect newGeometry = _geometry;

                int mask = e->xconfigurerequest.value_mask;
                if (mask & CWWidth)
                    newGeometry.setWidth(e->xconfigurerequest.width);
                if (mask & CWHeight)
                    newGeometry.setHeight(e->xconfigurerequest.height);
                if (mask & CWX)
                    newGeometry.setX(e->xconfigurerequest.x);
                if (mask & CWY)
                    newGeometry.setY(e->xconfigurerequest.y);

                _geometry = newGeometry;
            }

            break;

        case ButtonPress:
            WindowManager::self()->setActiveClient(this);
            // Replay the event...
            XAllowEvents(QX11Info::display(), ReplayPointer, CurrentTime);
            return true;
    }

    return false;
}

void Client::move(const QPoint &p)
{
    qDebug() << __PRETTY_FUNCTION__ << "Move to" << p;

    _decoration->move(p.x(), p.y());

    XConfigureEvent e;
    e.type = ConfigureNotify;
    e.send_event = True;
    e.event = _winId;
    e.window = _winId;
    e.x = p.x();
    e.y = p.y();
    e.width = _geometry.width();
    e.height = _geometry.height();
    e.border_width = 0;
    e.above = None;
    e.override_redirect = 0;
    XSendEvent(QX11Info::display(), e.event, true, StructureNotifyMask, (XEvent *)&e);
    XSync(QX11Info::display(), False);
}

void Client::resize(const QSize &size, int gravity)
{
    qDebug() << __PRETTY_FUNCTION__ << "resize to" << size;

    QSize currentSize = _decoration->size();
    int x = _decoration->x();
    int y = _decoration->y();

    switch (gravity)
    {
        case NorthGravity:
            y += currentSize.height() - size.height();
            break;

        case NorthWestGravity:
        {
            x += currentSize.width() - size.width();
            y += currentSize.height() - size.height();
            break;
        }

        case WestGravity:
            x += currentSize.width() - size.width();
            break;
        
        case NorthEastGravity:
        {
            y += currentSize.height() - size.height();
            break;
        }
        
        case SouthWestGravity:
        {
            x += currentSize.width() - size.width();
            break;
        }
    }

    XSetWindowAttributes a;
    a.win_gravity = StaticGravity;
    XChangeWindowAttributes(QX11Info::display(), _winId, CWWinGravity, &a);

    _decoration->setGeometry(x, y, size.width(), size.height());

    BorderSize border = _decoration->borderSize();
    XMoveResizeWindow(QX11Info::display(), _winId,
                      border.left(), border.measuredHeight() - border.bottom(),
                      size.width() - border.measuredWidth(), size.height() - border.measuredHeight());
}

void Client::minimize()
{
}

void Client::maximize()
{    
}

void Client::close()
{
}

void Client::setActive()
{
    _decoration->setActive();
    _decoration->update();
}

void Client::setInactive()
{
    _decoration->setInactive();
    _decoration->update();
}

void Client::map()
{
    _decoration->show();
    XMapWindow(QX11Info::display(), _winId);
    XSync(QX11Info::display(), False);
}

void Client::updateTitle()
{
    char *name = NULL;

    Atom typeReturn;
    int formatReturn;
    unsigned long count, unused;
    unsigned char *data = NULL;

    if ((XGetWindowProperty(QX11Info::display(), _winId, ATOM(_NET_WM_NAME), 0, 1024,
                            False, AnyPropertyType, &typeReturn,
                            &formatReturn, &count, &unused, &data))
        == Success && data)
    {    
        _title = QString::fromUtf8((char *)data);
        XFree(data);
    }
    else if (XFetchName(QX11Info::display(), _winId, &name) && name != NULL)
    {
        _title = name;
        XFree(name);
    }
    else  // use class hints
    {
        XClassHint hint;
        if (XGetClassHint(QX11Info::display(), _winId, &hint))
        {
            _title = hint.res_name;
            XFree(hint.res_name);
            XFree(hint.res_class);
        }
    }    
}
