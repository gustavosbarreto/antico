#include "bordersize.hpp"

BorderSize::BorderSize(int top, int bottom, int left, int right, int titleBarHeight)
    : _top(top)
    , _bottom(bottom)
    , _left(left)
    , _right(right)
    , _titleBarHeight(titleBarHeight)
{
}

BorderSize::BorderSize(const BorderSize &other)
    : _top(other.top())
    , _bottom(other.bottom())
    , _left(other.left())
    , _right(other.right())
    , _titleBarHeight(other.titleBarHeight())
{
}
