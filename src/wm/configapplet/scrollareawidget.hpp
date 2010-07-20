#ifndef _SCROLLAREAWIDGET_HPP
#define _SCROLLAREAWIDGET_HPP

#include <QWidget>

class ScrollAreaWidget: public QWidget
{
    Q_OBJECT

public:
    ScrollAreaWidget(QWidget *parent);

protected:
    virtual void paintEvent(QPaintEvent *e);
};

#endif
