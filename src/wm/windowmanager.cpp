#include "windowmanager.hpp"

#include "src/eggwm/util/Include.h"
#include "src/eggwm/standards/EWMHRoot.h"
//#include "src/eggwm/xwindows/XWindowList.h"
#include "src/eggwm/standards/WMCheckWindow.h"
#include "src/eggwm/events/factory/EventFactory.h"

//#include <fixx11h.h>

Q_GLOBAL_STATIC(WindowManager, windowManager);

WindowManager::WindowManager()
    : QObject()
{
}

void WindowManager::init()
{
    // Inicializamos los atributos
    this->wmCheckWindow = new WMCheckWindow;
    this->windowList    = new XWindowList;
    this->eventFactory  = EventFactory::getInstance();
    this->eventFactory->initialize(this->windowList);

    // cargamos la configuración
    Config* cfg = Config::getInstance();
    cfg->loadConfig();

    // TODO Añadir a la lista de ventanas las ventanas que ya existan


    // Establecemos diversas propiedades requeridas por el estándar EWMH
    this->sendHints();

    // Establecemos que eventos queremos recibir
    XSelectInput(QX11Info::display(),
            QX11Info::appRootWindow(QX11Info::appScreen()),
              SubstructureRedirectMask /* MapRequest, ConfigureRequest,
                                          CirculateRequest */
            | SubstructureNotifyMask   /* CreateNotify, DestroyNotify,
                                          MapNotify, UnmapNotify,
                                          ReparentNotify, GravityNotify,
                                          ConfigureNotify, CirculateNotify */
            | ButtonPressMask);        /* ButtonPress */
    XFlush(QX11Info::display());
}

void WindowManager::sendHints()
{
    EWMHRoot ewmhRoot;

    ewmhRoot.sendSupportedHints();
    ewmhRoot.sendActiveWindow(None);
    ewmhRoot.sendSupportingWmCheck(this->wmCheckWindow->winId());

    ewmhRoot.sendNumberOfDesktops(1.0);
    ewmhRoot.sendDesktopNames("Egg Desktop");
    ewmhRoot.sendDesktopGeometry((long)QApplication::desktop()->width(),
            (long)QApplication::desktop()->height());
    ewmhRoot.sendDesktopViewport(0.0, 0.0);
    ewmhRoot.sendWorkarea(0.0, 0.0, (long)QApplication::desktop()->width(),
            (long)QApplication::desktop()->height());
    ewmhRoot.sendCurrentDesktop(0.0);
    ewmhRoot.sendSupportShowingdesktop(0.0);
}

bool WindowManager::x11EventFilter(_XEvent *e)
{
    EventHandler* handler = this->eventFactory->getEventHandler(e->type);

    if(handler != NULL)
        return handler->processEvent(e);
    else
        return false;
}

WindowManager *WindowManager::self()
{
    return windowManager();
}
