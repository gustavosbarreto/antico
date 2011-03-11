#ifndef _TASKBUTTON_H
#define _TASKBUTTON_H

#include <QPushButton>

class TaskButton: public QPushButton
{
    Q_OBJECT

public:
    TaskButton(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *e);
    QSize sizeHint() const;

private slots:
    void init();
};

#endif
