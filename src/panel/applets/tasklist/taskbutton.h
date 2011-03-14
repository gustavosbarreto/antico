#ifndef _TASKBUTTON_H
#define _TASKBUTTON_H

#include <QPushButton>

class TaskWindow;

class TaskButton: public QPushButton
{
    Q_OBJECT

public:
    TaskButton(TaskWindow *t, QWidget *parent);

protected:
    void paintEvent(QPaintEvent *e);
    QSize sizeHint() const;

private slots:
    void init();

    void updateTitle(const QString &title);
    void updateIcon(const QPixmap &icon);

private:
    TaskWindow *task;
};

#endif
