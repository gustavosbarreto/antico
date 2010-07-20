#ifndef _FAKEWORKSPACE_HPP
#define _FAKEWORKSPACE_HPP

#include <QWidget>

class Decoration;

class FakeWorkspace: public QWidget
{
public:
    FakeWorkspace(QWidget *parent);

private:
    Decoration *_activeDecoration;
    Decoration *_inactiveDecoration;
};

#endif

