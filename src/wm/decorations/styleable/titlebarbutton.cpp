#include "titlebarbutton.hpp"

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
    switch (_type)
    {
        case StyleableDecoration::CloseButton:
	    if (active)
		setPixmap(QPixmap(":/decorations/styleable/images/close.png"));
	    else
		setPixmap(QPixmap(":/decorations/styleable/images/close_unfocused.png"));
            break;
        case StyleableDecoration::MinimizeButton:
	    if (active)
		setPixmap(QPixmap(":/decorations/styleable/images/minimize.png"));
	    else
		setPixmap(QPixmap(":/decorations/styleable/images/minimize_unfocused.png"));
            break;
        case StyleableDecoration::MaximizeButton:
	    if (active)
		setPixmap(QPixmap(":/decorations/styleable/images/maximize.png"));
	    else
		setPixmap(QPixmap(":/decorations/styleable/images/maximize_unfocused.png"));
            break;
    }
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}
