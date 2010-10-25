#include "windowmanager.hpp"
#include "window.hpp"
#include "fixx11h.h"

#include <QWebView>
#include <QDesktopWidget>
#include <QDebug>

#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xcomposite.h>

static Core::WindowManager *instance = 0;

namespace Core
{

void WindowManager::init()
{
    display = QX11Info::display();

    Extensions extensions = queryExtensions();
    if (!(extensions & CompositeExtension))
        qFatal("Missing extension: Composite");
    if (!(extensions & DamageExtension))
        qFatal("Missing extension: XDamage");
    if (!(extensions & FixesExtension))
        qFatal("Missing extension: XFixes");
    if (!(extensions & ShapeExtension))
        qFatal("Missing extension: Shape");
    if (!(extensions & TestExtension))
        qFatal("Missing extension: XTest");

    if (isWindowManagerRunning())
        qFatal("Another window manager is already running!");
    if (isCompositeManagerRunning())
        qFatal("Another composite manager is already running!");

    if (!registerWindowManager())
        qFatal("Failed to register the window manager");
    if (!registerCompositeManager())
        qFatal("Failed to register the composite manager");

    if (!createOverlay())
        qFatal("Failed to create the overlay");

    XGrabServer(display);
    XSelectInput(display, QX11Info::appRootWindow(),
                 (SubstructureRedirectMask |
                  SubstructureNotifyMask |
                  StructureNotifyMask |
                  ResizeRedirectMask |
                  PropertyChangeMask |
                  FocusChangeMask));
    XUngrabServer(display);

    XDefineCursor(display, QX11Info::appRootWindow(), QCursor(Qt::ArrowCursor).handle());
}

bool WindowManager::x11EventFilter(_XEvent *event)
{
    if (event->type == XDamageNotify + damageEventBase)
    {
        XDamageNotifyEvent *e = reinterpret_cast<XDamageNotifyEvent *>(event);
        do
        {
        } while (XCheckTypedEvent(display, XDamageNotify + damageEventBase, event));

        return true;
    }

    switch (event->type)
    {
        case ClientMessage:
        case CreateNotify:
        {
            if (event->xcreatewindow.window == webView->winId())
                return false;
            
            return true;
        }
        case DestroyNotify:
            break;
        case ConfigureNotify:
            break;
        case ConfigureRequest:
            break;
        case ReparentNotify:
            break;
        case MapRequest:
        {
            // add window
            Core::CWindow *c = new Core::CWindow(event->xmaprequest.window);

            XAddToSaveSet(display, event->xmaprequest.window);
            XMapWindow(display, event->xmaprequest.window);

            c->grabPixmap();

            return true;
        }
        case MapNotify:
            break;
        case UnmapNotify:
            break;
        case PropertyNotify:
            break;
        case FocusIn:
        case FocusOut:
            break;
        default:
            return false;
    }
}

WindowManager::WindowManager()
    : QObject()
{
}

WindowManager *WindowManager::self()
{
    if (!instance)
        instance = new WindowManager;
    return instance;
}

WindowManager::Extensions WindowManager::queryExtensions()
{
    Extensions extensions;
    int compositeEventBase, testEventBase, errorBase, major, minor;

    if (XCompositeQueryExtension(display, &compositeEventBase, &errorBase))
        extensions |= CompositeExtension;
    if (XDamageQueryExtension(display, &damageEventBase, &errorBase))
        extensions |= DamageExtension;
    if (XFixesQueryExtension(display, &fixesEventBase, &errorBase))
        extensions |= FixesExtension;
    if (XShapeQueryExtension(display, &shapeEventBase, &errorBase))
        extensions |= ShapeExtension;
    if (XTestQueryExtension(display, &testEventBase, &errorBase, &major, &minor))
        extensions |= TestExtension;

    return extensions;
}

bool WindowManager::isCompositeManagerRunning() const
{
    Atom atom = XInternAtom(display, QString("_NET_WM_CM_S%1").arg(QX11Info::appScreen()).toUtf8().data(), False);
    return (XGetSelectionOwner(display, atom) != None);
}

bool WindowManager::isWindowManagerRunning() const
{
    Atom atom = XInternAtom(display, QString("WM_S%1").arg(QX11Info::appScreen()).toUtf8().data(), False);
    return (XGetSelectionOwner(display, atom) != None);
}

bool WindowManager::registerWindowManager()
{
    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    attrs.event_mask = PropertyChangeMask;

    Window window = XCreateWindow(display,
                                  QX11Info::appRootWindow(),
                                  -100, -100, 1, 1, 0,
                                  CopyFromParent, CopyFromParent,
                                  (Visual *)CopyFromParent,
                                  CWOverrideRedirect | CWEventMask,
                                  &attrs);

    Atom atom = XInternAtom(display, QString("WM_S%1").arg(QX11Info::appScreen()).toUtf8().data(), False);
//    if (!XSetSelectionOwner(display, atom, window, 0))
//        return false;
    return true;
}

bool WindowManager::registerCompositeManager()
{
    Window window = XCreateSimpleWindow(display, RootWindow(display, 0),
                                        0, 0, 1, 1, 0,
                                        None, None);

    Atom atom = XInternAtom(display, QString("_NET_WM_CM_S%1").arg(QX11Info::appScreen()).toUtf8().data(), False);
//    if (!XSetSelectionOwner(display, atom, window, 0))
//        return false;
    return true;
}

bool WindowManager::createOverlay()
{
    overlayWindow = XCompositeGetOverlayWindow(display, QX11Info::appRootWindow());
    if (overlayWindow == None)
        return false;

    webView = new QWebView();
    webView->setWindowFlags(Qt::X11BypassWindowManagerHint);
    webView->resize(QDesktopWidget().availableGeometry().size());

    XReparentWindow(display, webView->winId(), overlayWindow, 0, 0);

    allowInputThroughOverlay(overlayWindow);
    //allowInputThroughOverlay(webView->winId());

    return true;
}

void WindowManager::allowInputThroughOverlay(Qt::HANDLE window)
{
    XRectangle rect = { 0, 0, DisplayWidth(display, 0), DisplayHeight(display, 0) };
    XserverRegion region = XFixesCreateRegion(display, &rect, 1);
    XFixesSetWindowShapeRegion(display, window, ShapeBounding, 0, 0, 0);
    XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, 0);
    XFixesDestroyRegion(display, region);
}

}
