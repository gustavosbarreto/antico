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
    Q_PROPERTY(int marginLeft READ marginLeft WRITE setMarginLeft);
    Q_PROPERTY(int marginTop READ marginTop WRITE setMarginTop);
    Q_PROPERTY(int marginRight READ marginRight WRITE setMarginRight);
    Q_PROPERTY(int marginBottom READ marginBottom WRITE setMarginBottom);
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing);
    Q_PROPERTY(QString align READ align WRITE setAlign);

public:
    ButtonContainer(QWidget *parent);

    inline int marginLeft() const { return _marginLeft; }
    inline void setMarginLeft(int value) { _marginLeft = value; }

    inline int marginTop() const { return _marginTop; }
    inline void setMarginTop(int value) { _marginTop = value; }

    inline int marginRight() const { return _marginRight; }
    inline void setMarginRight(int value) { _marginRight = value; }

    inline int marginBottom() const { return _marginBottom; }
    inline void setMarginBottom(int value) { _marginBottom = value; }

    inline int spacing() const { return _spacing; }
    inline void setSpacing(int value) { _spacing = value; }

    inline const QString &align() const { return _align; }
    inline void setAlign(const QString &value) { _align = value; }

    void setActive(bool active);

private:
    void addButton(StyleableDecoration::ButtonType type);
    inline TitleBarButton *button(StyleableDecoration::ButtonType type) { return _buttons.value(type); }

private slots:
    void buttonClicked(int button);

private:
    QMap<StyleableDecoration::ButtonType, TitleBarButton *> _buttons;
    QSignalMapper *_signalMapper;

    int _marginLeft;
    int _marginTop;
    int _marginRight;
    int _marginBottom;
    int _spacing;

    QString _align;

signals:
    void clicked(StyleableDecoration::ButtonType);
};

#endif
