#ifndef _TASKBUTTON_H
#define _TASKBUTTON_H

#include <QPushButton>

class TaskWindow;

class TaskButton: public QPushButton
{
    Q_OBJECT

public:
    TaskButton(TaskWindow *t, QWidget *parent);

    TaskWindow *task();

protected:
    void paintEvent(QPaintEvent *e);
    QSize sizeHint() const;

private slots:
    void init();

    void updateTitle(const QString &title);
    void updateIcon(const QPixmap &icon);

private:
    class Private;
    Private *d;
};

#endif
