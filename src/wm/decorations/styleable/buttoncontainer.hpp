#ifndef _BUTTONCONTAINER_HPP
#define _BUTTONCONTAINER_HPP

#include <QFrame>
#include <QMap>

#include "styleabledecoration.hpp"

class QSignalMapper;
class TitleBarButton;

class ButtonContainer: public QFrame
{
    Q_OBJECT

public:
    ButtonContainer(QWidget *parent);

    void setActive(bool active);

private:
    void addButton(StyleableDecoration::ButtonType type);
    inline TitleBarButton *button(StyleableDecoration::ButtonType type) { return _buttons.value(type); }

private slots:
    void buttonClicked(int button);

private:
    QMap<StyleableDecoration::ButtonType, TitleBarButton *> _buttons;
    QSignalMapper *_signalMapper;

signals:
    void clicked(StyleableDecoration::ButtonType);
};

#endif
