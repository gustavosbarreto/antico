#ifndef _PANEL_H
#define _PANEL_H

#include <QObject>

class PanelSettings;
class PanelWidgetPlugin;

class Panel: public QObject
{
public:
    Panel();

    void sendHints();
    void setPosition();
    void show();

private:
    PanelSettings *settings;
    PanelWidgetPlugin *plugin;
};

#endif
