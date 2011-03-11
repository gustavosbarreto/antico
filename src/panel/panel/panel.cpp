// Self
#include "panel.h"

// Qt
#include <QWidget>
#include <QX11Info>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QFile>
#include <QDebug>

// Own
#include "panelsettings.h"

// Lib
#include <paneldecoration.h>

// Applets
#include <applets/startmenu/startmenuapplet.h>
#include <applets/tasklist/tasklistapplet.h>

// X11
#include <X11/Xatom.h>
#include <X11/Xutil.h>

class StyleSheetDecoration: public PanelDecoration
{
public:
    StyleSheetDecoration(): PanelDecoration()
    {
        widget = new QWidget;

        QFile file(":/style.qss");
        file.open(QFile::ReadOnly);
        widget->setStyleSheet(file.readAll());

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setContentsMargins(0, 2, 0, 0);
        layout->setSizeConstraint(QLayout::SetNoConstraint);
        widget->setLayout(layout);
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

    plugin = new StyleSheetDecoration;
    plugin->init();

    sendHints();

    QDesktopWidget desktop;
    QRect rect;
    // Build panel geometry based on his orientation
    switch (plugin->orientation())
    {
        case PanelDecoration::LeftOrientation:
            rect = QRect(0, 0, plugin->size(), desktop.height());
            break;
            
        case PanelDecoration::RightOrientation:
            rect = QRect(desktop.width() - plugin->size(), 0, plugin->size(), desktop.height());
            break;

        case PanelDecoration::TopOrientation:
            rect = QRect(0, 0, desktop.width(), plugin->size());
            break;

        case PanelDecoration::BottomOrientation:
            rect = QRect(0, desktop.height() - plugin->size(), desktop.width(), plugin->size());
            break;

        default:
            break;
    }

    plugin->setGeometry(rect);
    plugin->show();

    plugin->addWidget((new StartMenuApplet)->widget());
    plugin->addWidget((new TaskListApplet)->widget());
    plugin->addStretch();
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
            case PanelDecoration::LeftOrientation:
                struts[0] = plugin->size();
                struts[11] = desktop.height();
                break;

            case PanelDecoration::RightOrientation:
                struts[1] = plugin->size();
                struts[11] = desktop.height();
                break;

            case PanelDecoration::TopOrientation:
                struts[2] = plugin->size();
                struts[11] = desktop.width();
                break;

            case PanelDecoration::BottomOrientation:
                struts[3] = 20;
                struts[11] = desktop.width();

            default:
                break;
        }

        XChangeProperty(QX11Info::display(), plugin->winId(),
                        XInternAtom(QX11Info::display(), "_NET_WM_STRUT_PARTIAL", False),
                        XA_CARDINAL, 32, PropModeReplace, (unsigned char *)struts, 12);
    }
}
