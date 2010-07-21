#ifndef _STYLEABLEDECORATION_HPP
#define _STYLEABLEDECORATION_HPP

#include "decoration.hpp"

class TitleBar;

class StyleableDecoration: public Decoration
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active);
    Q_PROPERTY(int borderLeftWidth READ borderLeftWidth WRITE setBorderLeftWidth);
    Q_PROPERTY(int borderTopWidth READ borderTopWidth WRITE setBorderTopWidth);
    Q_PROPERTY(int borderRightWidth READ borderRightWidth WRITE setBorderRightWidth);
    Q_PROPERTY(int borderBottomWidth READ borderBottomWidth WRITE setBorderBottomWidth);
    Q_PROPERTY(int titleBarHeight READ titleBarHeight WRITE setTitleBarHeight);

public:
    enum ButtonType
    {
        CloseButton,
        MinimizeButton,
        MaximizeButton
    };

    StyleableDecoration(Client *c, QWidget *parent);

    virtual void init();

    inline int borderLeftWidth() const { return _borderLeftWidth; }
    inline void setBorderLeftWidth(int value) { _borderLeftWidth = value; }

    inline int borderTopWidth() const { return _borderTopWidth; }
    inline void setBorderTopWidth(int value) { _borderTopWidth = value; }

    inline int borderRightWidth() const { return _borderRightWidth; }
    inline void setBorderRightWidth(int value) { _borderRightWidth = value; }

    inline int borderBottomWidth() const { return _borderBottomWidth; }
    inline void setBorderBottomWidth(int value) { _borderBottomWidth = value; }

    inline int titleBarHeight() const { return _titleBarHeight; }
    inline void setTitleBarHeight(int value) { _titleBarHeight = value; }

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

    int _borderLeftWidth;
    int _borderTopWidth;
    int _borderRightWidth;
    int _borderBottomWidth;
    int _titleBarHeight;
};

#endif
