#ifndef _BORDERSIZE_HPP
#define _BORDERSIZE_HPP

class BorderSize
{
public:
    BorderSize(int top, int bottom, int left, int right, int titleBarHeight);
    BorderSize(const BorderSize &other);

    inline int top() const { return _top; }
    inline int bottom() const { return _bottom; }
    inline int left() const { return _left; }
    inline int right() const { return _right; }

    inline int measuredWidth() const { return _left + _right; }
    inline int measuredHeight() const { return _top + _titleBarHeight + _bottom; }

    inline int titleBarHeight() const { return _titleBarHeight; }

private:
    int _top;
    int _bottom;
    int _left;
    int _right;
    int _titleBarHeight;
};

#endif
