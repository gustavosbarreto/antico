// Self
#include "tasklistapplet.h"

// Own
#include "tasklistwidget.h"

TaskListApplet::TaskListApplet():
    PanelApplet()
{
    myWidget = new TaskListWidget;
}
