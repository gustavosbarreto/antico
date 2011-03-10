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
    , widget(NULL)
{
    d = new Private;
}

void PanelApplet::init()
{
    Q_ASSERT(widget);
}

bool PanelApplet::hasWidget() const
{
    return (widget != NULL);
}
