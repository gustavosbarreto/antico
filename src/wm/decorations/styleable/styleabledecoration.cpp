#include "styleabledecoration.hpp"
#include "titlebar.hpp"
#include "client.hpp"

#include <QHBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QPainter>
#include <QLinearGradient>
#include <QSettings>
#include <QFileInfo>
#include <QStyle>
#include <QDebug>

StyleableDecoration::StyleableDecoration(Client *c, QWidget *parent)
    : Decoration(c, parent)
{
    setObjectName("Frame");

    QSettings settings(QSettings::UserScope, "antico", "wm", this);
    QString style = settings.value("Style/Path").toString();
    if (style.isEmpty() || !QFileInfo(style).isDir() || !QFile::exists(style + "/style.qss"))
        qFatal("Missing style: You must read the README file");

    QFile file(style + "/style.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(file.readAll());

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

bool StyleableDecoration::active()
{
    return _active;
}

void StyleableDecoration::setActive()
{
    _active = true;
    _titleBar->setActive(true);
    style()->polish(this);
}

void StyleableDecoration::setInactive()
{
    _active = false;
    _titleBar->setActive(false);
    style()->polish(this);
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

