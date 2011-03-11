#ifndef _TASKLISTWIDGET_H
#define _TASKLISTWIDGET_H

// Qt
#include <QFrame>

class TaskListWidget: public QFrame
{
    Q_OBJECT

public:
    TaskListWidget();

private slots:
    void init();
};

#endif
