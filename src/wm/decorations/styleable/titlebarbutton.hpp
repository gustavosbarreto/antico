#ifndef _TITLEBARBUTTON_HPP
#define _TITLEBARBUTTON_HPP

#include <QLabel>

#include "styleabledecoration.hpp"

class TitleBarButton: public QLabel
{
    Q_OBJECT

public:
    TitleBarButton(StyleableDecoration::ButtonType type, QWidget *parent);

    void setActive(bool active);

    inline StyleableDecoration::ButtonType type() const { return _type; }

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    StyleableDecoration::ButtonType _type;

signals:
    void clicked();
};

#endif
