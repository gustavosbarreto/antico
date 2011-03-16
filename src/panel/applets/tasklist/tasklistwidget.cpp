// Self
#include "tasklistwidget.h"

// Own
#include "taskbutton.h"

// Lib
#include <taskwindow.h>
#include <paneltaskwatcher.h>

// Qt
#include <QHBoxLayout>
#include <QTimer>
#include <QX11Info>
#include <QDebug>

// X11
#include <X11/Xlib.h>

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
    connect(PanelTaskWatcher::instance(), SIGNAL(taskAdded(TaskWindow *)), SLOT(addTaskButton(TaskWindow *)));
    connect(PanelTaskWatcher::instance(), SIGNAL(taskRemoved(TaskWindow *)), SLOT(removeTaskButton(TaskWindow *)));

    XSelectInput(QX11Info::display(), QX11Info::appRootWindow(),
                 StructureNotifyMask | SubstructureNotifyMask | PropertyChangeMask);
}

void TaskListWidget::addTaskButton(TaskWindow *task)
{
    layout()->addWidget(new TaskButton(task, this));
}

void TaskListWidget::removeTaskButton(TaskWindow *task)
{
    for (int i = 0; i < layout()->count(); ++i)
    {
        TaskButton *button = (TaskButton *)layout()->itemAt(i)->widget();
        if (button->task() == task)
        {
            layout()->removeWidget(button);
            break;
        }
    }

//    task->deleteLater();
}
