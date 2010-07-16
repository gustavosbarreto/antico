#ifndef _STYLEABLEDECORATION_HPP
#define _STYLEABLEDECORATION_HPP

#include "decoration.hpp"

class TitleBar;

class StyleableDecoration: public Decoration
{
    Q_OBJECT

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

    void setActive();
    void setInactive();

private slots:
    void buttonClicked(StyleableDecoration::ButtonType button);

private:
    TitleBar *_titleBar;
};

#endif
