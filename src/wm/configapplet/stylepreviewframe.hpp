#ifndef _STYLEPREVIEWFRAME_HPP
#define _STYLEPREVIEWFRAME_HPP

#include <QFrame>

class StylePreviewFrame: public QFrame
{
    Q_OBJECT

public:
    StylePreviewFrame(QWidget *parent);

protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    QWidget *_window;
};

#endif
