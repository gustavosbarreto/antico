#ifndef _STYLEABLEDECORATION_HPP
#define _STYLEABLEDECORATION_HPP

#include "decoration.hpp"

class TitleBar;

class StyleableDecoration: public Decoration
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active)

public:
    enum ButtonType
    {
        CloseButton,
        MinimizeButton,
        MaximizeButton
    };

    StyleableDecoration(Client *c, QWidget *parent);

    BorderSize borderSize() const;

    void setTitle(const QString &title);

    bool active();
    void setActive();
    void setInactive();

private slots:
    void buttonClicked(StyleableDecoration::ButtonType button);

private:
    TitleBar *_titleBar;
    bool _active;
};

#endif
