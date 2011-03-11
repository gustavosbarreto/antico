// Self
#include "tasklistwidget.h"

// Own
#include "taskbutton.h"

// Qt
#include <QHBoxLayout>
#include <QTimer>
#include <QDebug>

TaskListWidget::TaskListWidget():
    QFrame()
{
    setObjectName("TaskList");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    setLayout(layout);

    QTimer::singleShot(0, this, SLOT(init()));
}

void TaskListWidget::init()
{
    layout()->addWidget(new TaskButton(this));
    layout()->addWidget(new TaskButton(this));
}
