#include "titlebar.hpp"
#include "buttoncontainer.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QBitmap>
#include <QDebug>
#include <QApplication>

TitleBar::TitleBar(QWidget *parent):
    QFrame(parent)
{
    setMouseTracking(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(10, 2, 2, 2);

    _buttonContainer = new ButtonContainer(this);
    connect(_buttonContainer, SIGNAL(clicked(StyleableDecoration::ButtonType)),
            SIGNAL(buttonClicked(StyleableDecoration::ButtonType)));

    QFont f(font());
    f.setBold(true);
    f.setPixelSize(12);

    QPalette pal(palette());
    pal.setColor(QPalette::WindowText, QColor(223, 223, 223));

    _title = new QLabel(this);
    _title->setMouseTracking(true);
    _title->setFont(f);
    _title->setPalette(pal);
    _title->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    layout->addWidget(_buttonContainer);
    layout->addWidget(_title);
}

void TitleBar::setTitle(const QString &title)
{
    _title->setText(title);
}

void TitleBar::setActive(bool active)
{
    if (active)
    {
        QPalette pal(palette());
        pal.setColor(QPalette::WindowText, QColor(223, 223, 223));
        _title->setPalette(pal);
    }
    else
    {
        QPalette pal(palette());
        pal.setColor(QPalette::WindowText, QColor(153, 149, 139));
        _title->setPalette(pal);
    }

    _buttonContainer->setActive(active);
}

void TitleBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QLinearGradient grad(QPointF(rect().left(), rect().top()), QPointF(rect().left(), rect().bottom()));
    grad.setColorAt(0, QColor(107, 106, 99));
    grad.setColorAt(0.5, QColor(60, 59, 55));

    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawRect(rect());

    QFrame::paintEvent(event);
}
