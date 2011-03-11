// Self
#include "startmenuapplet.h"

// Own
#include "startmenuwidget.h"

StartMenuApplet::StartMenuApplet():
    PanelApplet()
{
    myWidget = new StartMenuWidget;
}
