#ifndef _STYLEPREVIEWFRAME_HPP
#define _STYLEPREVIEWFRAME_HPP

#include <QFrame>

#include "ui_stylepreviewframe.h"

class FakeWindow;

class StylePreviewFrame: public QFrame
{
    Q_OBJECT

public:
    StylePreviewFrame(const QString &styleSheet, QWidget *parent);

protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    Ui::StylePreviewFrame ui;
    FakeWindow *_window;
};

#endif
