// Lib
#include <taskwindow.h>

class TaskWindow::Private
{
public:
    QString title;
    QPixmap icon;
};

TaskWindow::TaskWindow(Qt::HANDLE winId, QObject *parent):
    QObject(parent),
    d(new Private)
{
}

const QString &TaskWindow::title() const
{
    return d->title;
}

void TaskWindow::setTitle(const QString &title)
{
    d->title = title;
    emit titleUpdated(title);
}

const QPixmap &TaskWindow::icon() const
{
    return d->icon;
}

void TaskWindow::setIcon(const QPixmap &icon)
{
    d->icon = icon;
}
