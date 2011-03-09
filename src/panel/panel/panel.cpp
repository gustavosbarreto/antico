// Self
#include "panel.h"

// Qt
#include <QWidget>
#include <QX11Info>
#include <QDesktopWidget>
#include <QDebug>

// Own
#include "panelsettings.h"

// Plugin
#include <panelwidgetplugin.h>

// X11
#include <X11/Xatom.h>
#include <X11/Xutil.h>

class MyPanelWidgetPlugin: public PanelWidgetPlugin
{
public:
    MyPanelWidgetPlugin(): PanelWidgetPlugin()
    {
        widget = new QWidget;
    }
};

Panel::Panel():
    QObject(),
    settings(new PanelSettings),
    plugin(NULL)
{
    if (!settings->plugin().isEmpty())
    {
    }

    plugin = new MyPanelWidgetPlugin;
    plugin->init();

    sendHints();

    QDesktopWidget desktop;
    QRect rect;
    // Build panel geometry based on his orientation
    switch (plugin->orientation())
    {
        case PanelWidgetPlugin::LeftOrientation:
            rect = QRect(0, 0, plugin->size(), desktop.height());
            break;
            
        case PanelWidgetPlugin::RightOrientation:
            rect = QRect(desktop.width() - plugin->size(), 0, plugin->size(), desktop.height());
            break;

        case PanelWidgetPlugin::TopOrientation:
            rect = QRect(0, 0, desktop.width(), plugin->size());
            break;

        case PanelWidgetPlugin::BottomOrientation:
            rect = QRect(0, desktop.height() - plugin->size(), desktop.width(), plugin->size());
            break;

        default:
            break;
    }

    plugin->setGeometry(rect);
    plugin->show();
}

void Panel::sendHints()
{
    // skip panel focus
    {
        XWMHints hints;
        hints.flags = InputHint;
        hints.input = 0;
        XSetWMHints(QX11Info::display(), plugin->winId(), &hints);
        
        unsigned int value = (1<<0); // WIN_HINTS_SKIP_FOCUS;
        XChangeProperty(QX11Info::display(), plugin->winId(),
                        XInternAtom(QX11Info::display(), "_WIN_HINTS", False), XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *)&value, 1);
    }

    // set dock/panel widget
    {
        Atom value = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DOCK", False);
        XChangeProperty(QX11Info::display(), plugin->winId(),
                        XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False),
                        XA_ATOM, 32, PropModeReplace, (unsigned char *)&value, 1);
    }

    // indicates that the dock/panel should not be included on a taskbar or pager
    {
        Atom value[2];
        value[0] = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_PAGER", False);
        value[1] = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
        XChangeProperty(QX11Info::display(), plugin->winId(),
                        XInternAtom(QX11Info::display(), "_NET_WM_STATE", False),
                        XA_ATOM, 32, PropModeReplace, (unsigned char *)value, 2);
    }

    // reserve space for the panel widget
    {
        QDesktopWidget desktop;
        unsigned long struts[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        switch (plugin->orientation())
        {
            case PanelWidgetPlugin::LeftOrientation:
                struts[0] = plugin->size();
                struts[11] = desktop.height();
                break;

            case PanelWidgetPlugin::RightOrientation:
                struts[1] = plugin->size();
                struts[11] = desktop.height();
                break;

            case PanelWidgetPlugin::TopOrientation:
                struts[2] = plugin->size();
                struts[11] = desktop.width();
                break;

            case PanelWidgetPlugin::BottomOrientation:
                struts[3] = plugin->size();
                struts[11] = desktop.width();

            default:
                break;
        }

        XChangeProperty(QX11Info::display(), plugin->winId(),
                        XInternAtom(QX11Info::display(), "_NET_WM_STRUT_PARTIAL", False),
                        XA_CARDINAL, 32, PropModeReplace, (unsigned char *)struts, 12);
    }
}
