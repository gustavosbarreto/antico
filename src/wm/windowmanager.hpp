#ifndef _WINDOWMANAGER_HPP
#define _WINDOWMANAGER_HPP

#include <QObject>
#include <QMap>

class WMCheckWindow;
class XWindowList;
class EventFactory;

union _XEvent; // avoid X11 include

class WindowManager: public QObject
{
public:
    WindowManager();

    void init();
    bool x11EventFilter(_XEvent *e);

    static WindowManager *self();

private:
    WMCheckWindow* wmCheckWindow;
    XWindowList* windowList;
    EventFactory* eventFactory;

    void sendHints();
};

#endif
