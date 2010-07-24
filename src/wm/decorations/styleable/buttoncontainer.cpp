#include "buttoncontainer.hpp"
#include "titlebarbutton.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QSignalMapper>
#include <QCoreApplication>
#include <QDebug>

ButtonContainer::ButtonContainer(QWidget *parent)
    : QFrame(parent)
    , _marginLeft(0)
    , _marginTop(0)
    , _marginRight(0)
    , _marginBottom(0)
    , _spacing(0)
    , _align("right")
{
    setObjectName("ButtonContainer");
    setMouseTracking(true);

    QCoreApplication::sendPostedEvents(this, 0);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(spacing());
    layout->setContentsMargins(marginLeft(), marginTop(), marginRight(), marginBottom());

    _signalMapper = new QSignalMapper(this);
    connect(_signalMapper, SIGNAL(mapped(int)), SLOT(buttonClicked(int)));

    addButton(StyleableDecoration::CloseButton);
    addButton(StyleableDecoration::MinimizeButton);
    addButton(StyleableDecoration::MaximizeButton);

    layout->addWidget(button(StyleableDecoration::CloseButton));
    layout->addWidget(button(StyleableDecoration::MinimizeButton));
    layout->addWidget(button(StyleableDecoration::MaximizeButton));

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
}

void ButtonContainer::setActive(bool active)
{
    foreach (TitleBarButton *button, _buttons)
	button->setActive(active);
}

void ButtonContainer::addButton(StyleableDecoration::ButtonType type)
{
    TitleBarButton *button = new TitleBarButton(type, this);
    connect(button, SIGNAL(clicked()), _signalMapper, SLOT(map()));
    _signalMapper->setMapping(button, type);
    _buttons.insert(type, button);
}

void ButtonContainer::buttonClicked(int button)
{
    StyleableDecoration::ButtonType type = static_cast<StyleableDecoration::ButtonType>(button);
    emit clicked(type);
}
