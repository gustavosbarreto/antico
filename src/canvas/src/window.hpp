#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <QObject>
#include <QX11Info>

#include <X11/Xutil.h>
#include <X11/Xregion.h>
#include <X11/extensions/Xdamage.h>

namespace Core
{

class CWindow: public QObject
{
    Q_OBJECT

public:
    CWindow(Qt::HANDLE w);

    void grabPixmap();

private:
    Display *display;
    Qt::HANDLE winId;
    Pixmap pixmap;
    Damage damage;
    Region clip;
};

}

#endif
