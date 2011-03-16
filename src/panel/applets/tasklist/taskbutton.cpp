// Self
#include "taskbutton.h"

// Lib
#include <taskwindow.h>

// Qt
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include <QDebug>

class TaskButton::Private
{
public:
    TaskWindow *task;
};

TaskButton::TaskButton(TaskWindow *t, QWidget *parent):
    QPushButton(parent),
    d(new Private)
{
    d->task = t;

    setObjectName("TaskButton");

    connect(d->task, SIGNAL(titleUpdated(const QString &)), SLOT(updateTitle(const QString &)));
    connect(d->task, SIGNAL(iconUpdated(const QPixmap &)), SLOT(updateIcon(const QPixmap &)));

    QTimer::singleShot(0, this, SLOT(init()));
}

TaskWindow *TaskButton::task()
{
    return d->task;
}

void TaskButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QStyleOptionButton opt;
    opt.init(this);

    QRect rect = opt.rect;

    if (isDown())
        opt.state |= QStyle::State_Sunken;

    style()->drawPrimitive(QStyle::PE_PanelButtonCommand, &opt, &painter, this);

    QPoint point;
    QPixmap pix;

    if (!icon().isNull())
    {
        pix = icon().pixmap(QSize(22, 22), QIcon::Active, QIcon::On);
        point = QPoint(6, rect.y() + rect.height() / 2 - pix.height() / 2);
        painter.drawPixmap(point, pix);
    }

    QPen savedPen = painter.pen();
    painter.setPen(QPen(opt.palette.brush(QPalette::ButtonText), savedPen.widthF()));

    if ((opt.state & QStyle::State_Enabled)) {
        QPen pen = painter.pen();
        painter.setPen(pen);
    }

    rect.setX(point.x() + pix.width() + 4);
    rect.setY((geometry().height() / 2) - opt.fontMetrics.height() / 2);

    QString str = painter.fontMetrics().elidedText(text(), Qt::ElideRight, rect.width() - 4);

    painter.drawText(rect, Qt::AlignLeft, str);
    painter.setPen(savedPen);
}

QSize TaskButton::sizeHint() const
{
    return QSize(180, parentWidget()->height());
}

void TaskButton::init()
{
    setText(d->task->title());
    setIcon(d->task->icon());
    setFixedHeight(parentWidget()->height());
}

void TaskButton::updateTitle(const QString &title)
{
    setText(title);
}

void TaskButton::updateIcon(const QPixmap &icon)
{
    setIcon(icon);
}
