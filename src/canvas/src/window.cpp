#include "window.hpp"
#include "fixx11h.h"

#include <QPixmap>
#include <QDebug>

#include <X11/extensions/Xcomposite.h>

namespace Core
{

CWindow::CWindow(Qt::HANDLE w)
    : QObject()
    , winId(w)
    , pixmap(NULL)
{
    display = QX11Info::display();
    damage = XDamageCreate(display, winId, XDamageReportNonEmpty);
    clip = XCreateRegion();

    XCompositeRedirectWindow(display, winId, CompositeRedirectAutomatic);

    grabPixmap();
}
    
void CWindow::grabPixmap()
{
    if (!pixmap)
    {
        XGrabServer(display);

        XWindowAttributes attr;
        XGetWindowAttributes(display, winId, &attr);
        if (attr.map_state == IsViewable)
             pixmap = XCompositeNameWindowPixmap(display, winId);
 
        XUngrabServer(display);
    }
}

}
