#ifndef _PANELAPPLET_H
#define _PANELAPPLET_H

#include <QObject>
#include <QRect>

class QWidget;

class PanelApplet: public QObject
{
public:
    PanelApplet();

    void init();
    bool hasWidget() const;

protected:
    class Private;
    Private *d;
    QWidget *widget;
};

Q_DECLARE_INTERFACE(PanelApplet, "Panel/1.0")

#endif
