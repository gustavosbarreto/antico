#include "decoration.hpp"
#include "client.hpp"
#include "windowmanager.hpp"

#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

#include <X11/Xlib.h>

Decoration::Decoration(Client *c)
    : QWidget()
    , _client(c)
    , _hoverResizeArea(false)
    , _resizeCursorPosition(NoneCursorPosition)
{
    setMouseTracking(true);
}

bool Decoration::x11EventFilter(_XEvent *e)
{
    return false;
}

void Decoration::mousePressEvent(QMouseEvent *e)
{
    WindowManager::self()->setActiveClient(client());

    setMoveOffset(e->pos());
    _resizeCursorPosition = cursorPosition(e->pos());
}

void Decoration::mouseReleaseEvent(QMouseEvent *e)
{
    setMoveOffset(QPoint(0, 0)); // Clear offset
    _resizeCursorPosition = NoneCursorPosition;

    if (cursorPosition(e->pos()) == NoneCursorPosition)
    {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void Decoration::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && !moveOffset().isNull())
    {
        // Moving the window
        if (!_hoverResizeArea)
        {
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);
            client()->move(mapToGlobal(e->pos()) - moveOffset());
        }
        else // Resizing the window
        {
            QRect rect = geometry();
            QPoint pos = e->pos() - moveOffset();
            int width = rect.width();
            int height = rect.height();

/*            switch (_resizeGravity)
            {
                case NorthGravity:
                    height = rect.height() - pos.y();
                    break;

                case NorthWestGravity:
                {
                    width = rect.width() - pos.x();
                    height = rect.height() - pos.y();
                    break;
                }

                case WestGravity:
                    width = rect.width() - pos.x();
                    break;
                    
                case NorthEastGravity:
                {
                    width = rect.width() - (pos.x() * -1);
                    height = rect.height() - pos.y();
                    setMoveOffset(QPoint(width, 0));
                    break;
                }
                    
                case SouthWestGravity:
                {
                    width = rect.width() - pos.x();
                    height = rect.height() - (pos.y() * -1);
                    setMoveOffset(QPoint(0, height));
                    break;
                }
                    
                case SouthEastGravity:
                    width = rect.width() - (pos.x() * -1);
                    height = rect.height() - (pos.y() * -1);
                    setMoveOffset(QPoint(width, height));
                    break;
            }

            client()->resize(QSize(width, height), _resizeGravity);*/
        }
    }
    else
    {
        // Always assumes that the pointer is over an resizable area
        _hoverResizeArea = true;

        switch (cursorPosition(e->pos()))
        {
            case TopCursorPosition:
            case BottomCursorPosition:
                QApplication::setOverrideCursor(Qt::SizeVerCursor);
                break;

            case LeftCursorPosition:
            case RightCursorPosition:
                QApplication::setOverrideCursor(Qt::SizeHorCursor);
                break;

            case TopLeftCursorPosition:
            case BottomRightCursorPosition:
                QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
                break;

            case TopRightCursorPosition:
            case BottomLeftCursorPosition:
                QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
                break;

            default:
                QApplication::setOverrideCursor(Qt::ArrowCursor);
                _hoverResizeArea = false; // not
        }
    }
}

Decoration::CursorPosition Decoration::cursorPosition(const QPoint &p)
{
    const int expand = 10; // TODO: Move this to BorderSize class

    BorderSize border = borderSize();
    QSize size = QWidget::size();

    if (p.x() >= size.width() || p.y() >= size.height())
        return NoneCursorPosition;

    if (p.x() < border.left())
    {
        if (p.y() < (border.top() + expand)) return TopLeftCursorPosition;
        if (p.y() >= (size.height() - border.left() - expand)) return BottomLeftCursorPosition;
        return LeftCursorPosition;
    }

    if (p.x() < (border.left() + expand))
    {
        if (p.y() < border.top()) return TopLeftCursorPosition;
        if (p.y() >= (size.height() - border.bottom())) return BottomLeftCursorPosition;
    }

    if (p.x() >= (size.width() - border.right() - expand))
    {
        if (p.y() < border.top()) return TopRightCursorPosition;
        if (p.y() >= (size.height() - border.bottom())) return BottomRightCursorPosition;
    }

    if (p.x() >= (size.width() - border.left()))
    {
        if (p.y() < (border.top() + expand)) return TopRightCursorPosition;
        if (p.y() >= (size.height() - border.left() - expand)) return BottomRightCursorPosition;
        return RightCursorPosition;
    }

    if (p.y() < border.top()) return TopCursorPosition;
    if (p.y() >= size.height() - border.bottom()) return BottomCursorPosition;

    return CenterCursorPosition;
}
