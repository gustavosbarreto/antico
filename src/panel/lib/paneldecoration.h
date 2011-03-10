#ifndef _PANELDECORATION_H
#define _PANELDECORATION_H

#include <QObject>
#include <QRect>

class QWidget;

class PanelDecoration: public QObject
{
public:
    enum Orientation
    {
        NoneOrientation = 0,
        LeftOrientation,
        RightOrientation,
        TopOrientation,
        BottomOrientation
    };

    PanelDecoration();

    void init();
    bool hasWidget() const;

    virtual int winId();
    virtual void setGeometry(const QRect &r);
    virtual int size();
    virtual Orientation orientation();
    virtual void show();
    virtual void addWidget(QWidget *widget);
    virtual void addSeparator();
    virtual void addStretch();

protected:
    class Private;
    Private *d;
    QWidget *widget;
};

Q_DECLARE_INTERFACE(PanelDecoration, "Panel/1.0")

#endif
