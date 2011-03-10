#ifndef _TASKWINDOW_H
#define _TASKWINDOW_H

// Qt
#include <QObject>

class TaskWindow: public QObject
{
    Q_OBJECT

public:
    TaskWindow(Qt::HANDLE winId, QObject *parent = 0);

private:
    class Private;
    const Private *d;
};

#endif
