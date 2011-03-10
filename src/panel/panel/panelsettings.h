#ifndef _PANELSETTINGS_H
#define _PANELSETTINGS_H

// Lib
#include <paneldecoration.h>

class QSettings;

class PanelSettings
{
public:
    PanelSettings();
    ~PanelSettings();

    const QString plugin() const;
    int size() const;
    PanelDecoration::Orientation orientation() const;

private:
    QSettings *settings;
};

#endif
