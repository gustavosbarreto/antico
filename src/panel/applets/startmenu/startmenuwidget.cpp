// Self
#include "startmenuwidget.h"

// Qt
#include <QTimer>

StartMenuWidget::StartMenuWidget():
    QToolButton()
{
    setObjectName("StartMenu");

    setIconSize(QSize(41, 14));
    setIcon(QIcon(":/applets/startmenu/logo.png"));
    setCheckable(true);

    QTimer::singleShot(0, this, SLOT(init()));
}

void StartMenuWidget::init()
{
    setFixedHeight(parentWidget()->height());
    setMinimumSize(QSize(41, parentWidget()->height()));
}
