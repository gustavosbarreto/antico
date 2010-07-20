#include "scrollareawidget.hpp"

#include <QVBoxLayout>
#include <QPainter>

ScrollAreaWidget::ScrollAreaWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(9);
    layout->addStretch();
    setLayout(layout);
}

void ScrollAreaWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QRect r(0, 0, rect().width(), rect().height());
    QPixmap pixmap = QPixmap(":/configapplet/images/bg.png").scaled(r.width(), r.height(),
                                                                    Qt::IgnoreAspectRatio,
                                                                    Qt::SmoothTransformation);
    painter.drawPixmap(r, pixmap, r);
}
