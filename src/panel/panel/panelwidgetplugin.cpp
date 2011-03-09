// Self
#include <panelwidgetplugin.h>

// Own
#include "panelsettings.h"

// Qt
#include <QWidget>
#include <QDesktopWidget>
#include <QVariant>
#include <QSettings>
#include <QBoxLayout>
#include <QFile>
#include <QDebug>

class PanelWidgetPlugin::Private
{
public:
    PanelSettings *settings;

    Private()
    {
        settings = new PanelSettings;
    }
};

PanelWidgetPlugin::PanelWidgetPlugin()
    : QObject()
    , widget(NULL)
{
    d = new Private;
}

void PanelWidgetPlugin::init()
{
    Q_ASSERT(widget);
//    widget->setObjectName("Panel");
}

bool PanelWidgetPlugin::hasWidget() const
{
    return (widget != NULL);
}

int PanelWidgetPlugin::winId()
{
    return widget->winId();
}

void PanelWidgetPlugin::setGeometry(const QRect &r)
{
    widget->setGeometry(r);
}

int PanelWidgetPlugin::size()
{
    int size = d->settings->size();
    if (size <= 0) size = 30; // default size
    return size;
}

PanelWidgetPlugin::Orientation PanelWidgetPlugin::orientation()
{
    Orientation orientation = d->settings->orientation();
    if (orientation == NoneOrientation) orientation = BottomOrientation; // default orientation
    return orientation;
}

void PanelWidgetPlugin::show()
{
    widget->show();
}

void PanelWidgetPlugin::addWidget(QWidget *widget)
{
    widget->layout()->addWidget(widget);
}

void PanelWidgetPlugin::addSeparator()
{
}

void PanelWidgetPlugin::addStretch()
{
    QBoxLayout *layout = static_cast<QBoxLayout *>(widget->layout());
    layout->addStretch();
}
