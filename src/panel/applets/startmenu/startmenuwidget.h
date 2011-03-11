#ifndef _STARTMENUWIDGET_H
#define _STARTMENUWIDGET_H

#include <QToolButton>

class StartMenuWidget: public QToolButton
{
    Q_OBJECT

public:
    StartMenuWidget();

private slots:
    void init();
};

#endif
