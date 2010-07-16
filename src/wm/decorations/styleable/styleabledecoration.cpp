#include "styleabledecoration.hpp"
#include "titlebar.hpp"
#include "client.hpp"

#include <QHBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QPainter>
#include <QLinearGradient>

StyleableDecoration::StyleableDecoration(Client *c, QWidget *parent)
    : Decoration(c, parent)
{
    setObjectName("Frame");

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

