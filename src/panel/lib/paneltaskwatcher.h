#ifndef _PANELTASKWATCHER_H
#define _PANELTASKWATCHER_H

#include <QObject>

union _XEvent; // avoid x11 includes

class TaskWindow;

class PanelTaskWatcher: public QObject
{
    Q_OBJECT

public:
    PanelTaskWatcher();

    bool x11EventFilter(_XEvent *e);

    static PanelTaskWatcher *instance();

signals:
    void taskAdded(TaskWindow *);
    void taskRemoved(TaskWindow *);

private:
    class Private;
    Private *d;

    void emitTaskAdded(TaskWindow *t);
    void emitTaskRemoved(TaskWindow *t);
};

#endif
