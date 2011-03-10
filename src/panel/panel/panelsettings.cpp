#include "panelsettings.h"

// Qt
#include <QSettings>
#include <QFile>

PanelSettings::PanelSettings()
{
    settings = new QSettings(QSettings::UserScope, "workspace", "panel");
    if (!QFile::exists(settings->fileName()))
    {
        delete settings;
        settings = new QSettings(QSettings::SystemScope, "workspace", "panel");
    }
}

PanelSettings::~PanelSettings()
{
    delete settings;
}

const QString PanelSettings::plugin() const
{
    return settings->value("Panel/plugin").toString();
}

int PanelSettings::size() const
{
    return settings->value("Panel/size").toInt();
}

PanelDecoration::Orientation PanelSettings::orientation() const
{
    return (PanelDecoration::Orientation)settings->value("Panel/orientation").toInt();
}
