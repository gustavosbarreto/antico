#ifndef _TITLEBAR_HPP
#define _TITLEBAR_HPP

#include <QFrame>

#include "styleabledecoration.hpp"

class QLabel;
class ButtonContainer;

class TitleBar: public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active)

public:
    TitleBar(QWidget *parent = 0);

    void setTitle(const QString &title);

    bool active();
    void setActive(bool active);

private:
    ButtonContainer *_buttonContainer;
    QLabel *_title;
    bool _active;

signals:
    void buttonClicked(StyleableDecoration::ButtonType);
};

#endif
