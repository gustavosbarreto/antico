#ifndef _TASKWINDOW_H
#define _TASKWINDOW_H

// Qt
#include <QObject>
#include <QPixmap>

class TaskWindow: public QObject
{
    Q_OBJECT

public:
    TaskWindow(Qt::HANDLE winId, QObject *parent = 0);

    const QString &title() const;
    void setTitle(const QString &title);
    const QPixmap &icon() const;
    void setIcon(const QPixmap &icon);

signals:
    void titleUpdated(const QString &);
    void iconUpdated(const QPixmap &);

private:
    class Private;
    Private *d;
};

#endif
