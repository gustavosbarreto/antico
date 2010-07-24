#include "titlebar.hpp"
#include "buttoncontainer.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QBitmap>
#include <QDebug>
#include <QApplication>
#include <QStyle>

TitleBar::TitleBar(QWidget *parent):
    QFrame(parent)
{
    setObjectName("TitleBar");
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
    _title->setObjectName("TitleText");
    _title->setMouseTracking(true);
    _title->setFont(f);
    _title->setPalette(pal);
    _title->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    if (_buttonContainer->align() == "right")
    {
        layout->addWidget(_title);
        layout->addWidget(_buttonContainer);
    }
    else if (_buttonContainer->align() == "left")
    {
        layout->addWidget(_buttonContainer);
        layout->addWidget(_title);
    }
}

void TitleBar::setTitle(const QString &title)
{
    _title->setText(title);
}

bool TitleBar::active()
{
    return _active;
}

void TitleBar::setActive(bool active)
{
    _active = active;
    _buttonContainer->setActive(active);

    style()->polish(this);
    style()->polish(_title);
}
