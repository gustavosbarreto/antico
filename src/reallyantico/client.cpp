#include "client.hpp"
#include "frame.h"
#include "atoms.hpp"

#include <X11/Xatom.h>
#include <X11/Xutil.h>

Client::Client(Qt::HANDLE window, const QString &type, Dockbar *dockBar, Desk *desk):
    QObject(),
    m_window(window),
    m_type(type),
    m_dockBar(dockBar),
    m_desk(desk)
{
    m_frame = new Frame(window, type, dockBar, desk);

    XSetWindowAttributes attr;
    attr.event_mask = ColormapChangeMask | PropertyChangeMask;
    XChangeWindowAttributes(QX11Info::display(), window, CWEventMask, &attr);

    XGrabServer(QX11Info::display());

    updateGeometry();
    updateWMHints();
    updateWMNormalHints();
    updateWMProtocols();
    updateIcon();
    updateName();
}

void Client::updateWindowType()
{
    Atom typeReturn;
    int formatReturn;
    unsigned long count, unused;
    unsigned char *data = NULL;

    if (XGetWindowProperty(QX11Info::display(), m_window, ATOM(_NET_WM_WINDOW_TYPE),
			   0l, 2048l, False, XA_ATOM, &typeReturn, &formatReturn,
			   &count, &unused, &data) == Success)
    {
	if (typeReturn == XA_ATOM && formatReturn == 32 && count > 0)
	{
	    long *types = (long *)data;

	    for (unsigned long i = 0; i < count; ++i)
	    {
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_NORMAL))
		    m_windowType = NormalType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_DESKTOP))
		    m_windowType = DesktopType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_DOCK))
		    m_windowType = DockType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_TOOLBAR))
		    m_windowType = ToolBarType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_MENU))
		    m_windowType = MenuType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_DIALOG))
		    m_windowType = DialogType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_UTILITY))
		    m_windowType = UtilityType;
		if ((Atom)types[i] == ATOM(_NET_WM_WINDOW_TYPE_SPLASH))
		    m_windowType = SplashType;
	    }
	}
    }

    if (data)
	XFree(data);
}

void Client::updateGeometry()
{
    XWindowAttributes attr;
    XGetWindowAttributes(QX11Info::display(), m_window, &attr);

    m_geometry = QRect(attr.x, attr.y, attr.width, attr.height);
}

void Client::updateWMHints()
{
    XWMHints *hints;

    if ((hints = XGetWMHints(QX11Info::display(), m_window)) != NULL)
    {
        if (hints->flags & StateHint && hints->initial_state == IconicState)
	    m_mappingState = IconicMappingState;

        XFree(hints);
    }
}

void Client::updateWMNormalHints()
{
    XSizeHints *sizeHints;
    long hints = 0;
    long flags = 0;

    if ((sizeHints = XAllocSizeHints()))
    {
        if (XGetWMNormalHints(QX11Info::display(), m_window, sizeHints, &hints) == 0)
            return;

        flags = sizeHints->flags;

        if (flags & PPosition)
        {
	    m_geometry.setX(sizeHints->x);
	    m_geometry.setY(sizeHints->y);
        }
        if (flags & PSize)
        {
            if (sizeHints->width != 0)
		m_geometry.setWidth(sizeHints->width);
            if (sizeHints->height != 0)
		m_geometry.setHeight(sizeHints->height);
        }
        if (flags & PBaseSize)
        {
            if (sizeHints->base_width != 0 && sizeHints->base_width >= m_geometry.width())
                m_geometry.setWidth(sizeHints->base_width);
            if (sizeHints->base_height != 0 && sizeHints->base_height >= m_geometry.height())
                m_geometry.setHeight(sizeHints->base_height);
        }
    }
}

void Client::updateWMProtocols()
{
    Atom *protocols;
    int count = 0;

    m_wmProtocols.clear();

    if (XGetWMProtocols(QX11Info::display(), m_window, &protocols, &count))
    {
        for (int i = 0; i < count; i++)
	    m_wmProtocols.insert(protocols[i]);

        XFree(protocols);
    }
}

void Client::updateIcon()
{
    // TODO
}

void Client::updateName()
{
    char *name = NULL;

    Atom typeReturn;
    int formatReturn;
    unsigned long count, unused;
    unsigned char *data = NULL;

    if ((XGetWindowProperty(QX11Info::display(), m_window, ATOM(_NET_WM_NAME), 0, 1024,
			    False, AnyPropertyType, &typeReturn,
			    &formatReturn, &count, &unused, &data))
	== Success && data)
    {    
        m_name = QString::fromUtf8((char *)data);
        XFree(data);
    }
    else if (XFetchName(QX11Info::display(), m_window, &name) && name != NULL)
    {
	m_name = name;
        XFree(name);
    }
    else  // use class hints
    {
        XClassHint hint;
        if (XGetClassHint(QX11Info::display(), m_window, &hint))
        {
	    m_name = hint.res_name;
            XFree(hint.res_name);
            XFree(hint.res_class);
        }
    }
}
