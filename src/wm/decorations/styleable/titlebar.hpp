#ifndef _TITLEBAR_HPP
#define _TITLEBAR_HPP

#include <QFrame>

#include "styleabledecoration.hpp"

class QLabel;
class ButtonContainer;

class TitleBar: public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent = 0);

    void setTitle(const QString &title);

    void setActive(bool active);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    ButtonContainer *_buttonContainer;
    QLabel *_title;

signals:
    void buttonClicked(StyleableDecoration::ButtonType);
};

#endif