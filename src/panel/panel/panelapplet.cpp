// Self
#include <panelapplet.h>

// Qt
#include <QWidget>
#include <QDebug>

class PanelApplet::Private
{
public:
    Private()
    {
    }
};

PanelApplet::PanelApplet()
    : QObject()
    , myWidget(NULL)
{
    d = new Private;
}

void PanelApplet::init()
{
    Q_ASSERT(myWidget);
}

QWidget *PanelApplet::widget()
{
    return myWidget;
}


