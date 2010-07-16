#ifndef _DECORATION_HPP
#define _DECORATION_HPP

#include "bordersize.hpp"

#include <QWidget>

class Client;
union _XEvent;

class Decoration: public QWidget
{
    Q_OBJECT

public:
    enum CursorPosition
    {
        NoneCursorPosition,
        CenterCursorPosition,
        TopCursorPosition,
        BottomCursorPosition,
        LeftCursorPosition,
        RightCursorPosition,
        TopLeftCursorPosition,
        TopRightCursorPosition,
        BottomLeftCursorPosition,
        BottomRightCursorPosition
    };

    Decoration(Client *c);

    virtual bool x11EventFilter(_XEvent *e);

    virtual BorderSize borderSize() const = 0;
    virtual void setTitle(const QString &title) = 0;

    virtual inline void setActive() { _active = true; }
    virtual inline void setInactive() { _active = false; }
    inline bool isActive() const { return _active; }

    inline Client *client() const { return _client; }

protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);

    virtual CursorPosition cursorPosition(const QPoint &p);

    inline QPoint moveOffset() const { return _moveOffset; }
    inline void setMoveOffset(const QPoint &p) { _moveOffset = p; }

private:
    Client *_client;
    QPoint _moveOffset;
    bool _hoverResizeArea;
    CursorPosition _resizeCursorPosition;
    bool _active;
};

#endif
