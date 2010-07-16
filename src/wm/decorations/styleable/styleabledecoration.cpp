#include "styleabledecoration.hpp"
#include "titlebar.hpp"
#include "client.hpp"

#include <QHBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QPainter>
#include <QLinearGradient>

StyleableDecoration::StyleableDecoration(Client *c)
    : Decoration(c)
{
    _titleBar = new TitleBar(this);
    _titleBar->setFixedHeight(borderSize().titleBarHeight());
    connect(_titleBar, SIGNAL(buttonClicked(StyleableDecoration::ButtonType)),
            SLOT(buttonClicked(StyleableDecoration::ButtonType)));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(borderSize().left() - 2, // expand 2px
                               borderSize().top(),
                               borderSize().right() - 2, // expand 2px
                               borderSize().bottom());
    layout->addWidget(_titleBar);
    layout->addStretch();

    setLayout(layout);
}

BorderSize StyleableDecoration::borderSize() const
{
    return BorderSize(1, 3, 3, 3, 28);
}

void StyleableDecoration::setTitle(const QString &title)
{
    _titleBar->setTitle(title);
}

void StyleableDecoration::setActive()
{
    _titleBar->setActive(true);
}

void StyleableDecoration::setInactive()
{
    _titleBar->setActive(false);
}

void StyleableDecoration::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.setPen(QColor(60, 59, 55));

    // top-left to bottom-left
    painter.drawLine(QPoint(0, 0), QPoint(0, rect().height()));
    // bottom-left to bottom-right
    painter.drawLine(QPoint(0, rect().height() - 1), QPoint(rect().width(), rect().height() - 1));
    // bottom-right to top-right
    painter.drawLine(QPoint(rect().width() - 1, rect().height() - 1), QPoint(rect().width() - 1, 0));
    // top-right to top-left
    painter.drawLine(QPoint(rect().width() - 1, 0), QPoint(0, 0));
}

void StyleableDecoration::buttonClicked(StyleableDecoration::ButtonType button)
{
    switch (button)
    {
        case CloseButton:
            client()->close();
            break;

        case MinimizeButton:
            client()->minimize();
            break;

        case MaximizeButton:
            client()->maximize();
            break;
    }
}

