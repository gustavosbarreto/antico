#ifndef _FAKEWINDOW_HPP
#define _FAKEWINDOW_HPP

#include <QWidget>

class Decoration;

class FakeWindow: public QWidget
{
public:
    FakeWindow(QWidget *parent);

    inline Decoration *decoration() const { return _decoration; }

protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    Decoration *_decoration;
    QWidget *_centralWidget;
};

#endif
