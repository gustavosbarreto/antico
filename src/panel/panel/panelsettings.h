#ifndef _PANELSETTINGS_H
#define _PANELSETTINGS_H

// Lib
#include <panelwidgetplugin.h>

class QSettings;

class PanelSettings
{
public:
    PanelSettings();
    ~PanelSettings();

    const QString plugin() const;
    int size() const;
    PanelWidgetPlugin::Orientation orientation() const;

private:
    QSettings *settings;
};

#endif
