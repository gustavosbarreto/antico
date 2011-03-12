// Self
#include "taskbutton.h"

// Qt
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include <QDebug>

TaskButton::TaskButton(QWidget *parent):
    QPushButton(parent)
{
    setObjectName("TaskButton");

    setText("Titulo");

    QTimer::singleShot(0, this, SLOT(init()));
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
    setFixedHeight(parentWidget()->height());
}
