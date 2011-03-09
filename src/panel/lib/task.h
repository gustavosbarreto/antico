#ifndef _TASK_H
#define _TASK_H

// Qt
#include <QObject>
#include <QPixmap>

// X11
#include <X11/Xlib.h>

namespace Workspace::Panel
{

class Task: public QObject
{
    Q_OBJECT

public:
    Task(Qt::HANDLE winId, QObject *parent);

private:
    const TaskPrivate *d;
};

} // namespace

#endif // _TASK_H
