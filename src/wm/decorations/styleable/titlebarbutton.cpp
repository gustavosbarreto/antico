#include "titlebarbutton.hpp"

#include <QStyle>

TitleBarButton::TitleBarButton(StyleableDecoration::ButtonType type, QWidget *parent)
    : QLabel(parent)
    , _type(type)
{
    setMouseTracking(true);
    setActive(true);

    switch (type)
    {
        case StyleableDecoration::CloseButton:
            setObjectName("CloseButton");
            setToolTip(trUtf8("Close"));
            break;
        case StyleableDecoration::MinimizeButton:
            setObjectName("MinimizeButton");
            setToolTip(trUtf8("Minimize"));
            break;
        case StyleableDecoration::MaximizeButton:
            setObjectName("MaximizeButton");
            setToolTip(trUtf8("Maximize"));
            break;
    }
}

void TitleBarButton::setActive(bool active)
{
    style()->polish(this);
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
