// Self
#include <paneldecoration.h>

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

class PanelDecoration::Private
{
public:
    PanelSettings *settings;

    Private()
    {
        settings = new PanelSettings;
    }
};

PanelDecoration::PanelDecoration()
    : QObject()
    , widget(NULL)
{
    d = new Private;
}

void PanelDecoration::init()
{
    Q_ASSERT(widget);
    widget->setObjectName("Panel");
}

bool PanelDecoration::hasWidget() const
{
    return (widget != NULL);
}

int PanelDecoration::winId()
{
    return widget->winId();
}

void PanelDecoration::setGeometry(const QRect &r)
{
    widget->setGeometry(r);
}

int PanelDecoration::size()
{
    int size = d->settings->size();
    if (size <= 0) size = 26; // default size
    return size;
}

PanelDecoration::Orientation PanelDecoration::orientation()
{
    Orientation orientation = d->settings->orientation();
    if (orientation == NoneOrientation) orientation = BottomOrientation; // default orientation
    return orientation;
}

void PanelDecoration::show()
{
    widget->show();
}

void PanelDecoration::addWidget(QWidget *w)
{
    w->setParent(widget);
    widget->layout()->addWidget(w);
}

void PanelDecoration::addSeparator()
{
}

void PanelDecoration::addStretch()
{
    QBoxLayout *layout = static_cast<QBoxLayout *>(widget->layout());
    layout->addStretch();
}
