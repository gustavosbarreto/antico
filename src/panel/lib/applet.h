#ifndef _APPLET_H
#define _APPLET_H

// Qt
#include <QObject>

class QWidget;

namespace Workspace::Panel
{

class Applet: public QObject
{
public:
    Applet(): QWidget() { }

    inline virtual QWidget *widget() = 0;
};

} // namespace

#endif // _APPLET_H
