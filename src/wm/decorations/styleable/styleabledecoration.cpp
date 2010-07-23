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
#include <QTimer>
#include <QCoreApplication>

#include "decorationstyleloader.hpp"

StyleableDecoration::StyleableDecoration(Client *c, QWidget *parent)
    : Decoration(c, parent)
    , _borderLeftWidth(0)
    , _borderTopWidth(0)
    , _borderRightWidth(0)
    , _borderBottomWidth(0)
    , _titleBarHeight(0)
{
    setObjectName("Frame");

    QSettings settings(QSettings::UserScope, "antico", "wm", this);

    QString stylePath = settings.value("Style/Path").toString();
    if (!QFile::exists(stylePath))
        qFatal("Please, launch the configapplet to choose a style");

    DecorationStyleLoader loader(stylePath);

    if (!loader.isValid())
        qFatal("Invalid theme file format");

    setStyleSheet(loader.styleSheet());
}

void StyleableDecoration::init()
{
    QCoreApplication::sendPostedEvents(this, 0);

    _titleBar = new TitleBar(this);
    _titleBar->setFixedHeight(borderSize().titleBarHeight());
    connect(_titleBar, SIGNAL(buttonClicked(StyleableDecoration::ButtonType)),
            SLOT(buttonClicked(StyleableDecoration::ButtonType)));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(borderSize().left(), borderSize().top(), borderSize().right(), borderSize().bottom());
    layout->addWidget(_titleBar);
    layout->addStretch();

    setLayout(layout);
}

BorderSize StyleableDecoration::borderSize() const
{
    return BorderSize(borderTopWidth(), borderBottomWidth(),
                      borderLeftWidth(), borderRightWidth(), titleBarHeight());
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

