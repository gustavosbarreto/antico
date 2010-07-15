#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <QObject>
#include <QX11Info>
#include <QRect>
#include <QSet>

#include <X11/Xdefs.h>

class Dockbar;
class Desk;
class Frame;

class Client: public QObject
{
    Q_OBJECT

public:
    // Supported window types
    enum Type
    {
	UnknownType,
	DesktopType,
	DockType,
	ToolBarType,
	MenuType,
	UtilityType,
	SplashType,
	DialogType,
	NormalType
    };

    // Supported window states
    enum MappingState
    {
	IconicMappingState
    };

    // Somewhere in the future will not need the pointer to DockBar and Desk
    Client(Qt::HANDLE window, const QString &type, Dockbar *dockBar, Desk *desk);

    inline Type windowType() const { return m_windowType; }

private:
    void updateWindowType();
    void updateGeometry();
    void updateWMHints();
    void updateWMNormalHints();
    void updateWMProtocols();
    void updateIcon();
    void updateName();

    inline bool hasWmProtocol(Atom protocol) { return m_wmProtocols.contains(protocol); }
    
private:
    Qt::HANDLE m_window;
    QString m_type; // TODO: this is ugly!
    Dockbar *m_dockBar;
    Desk *m_desk;
    Frame *m_frame;

    Type m_windowType;
    QRect m_geometry;
    MappingState m_mappingState;
    QSet<Atom> m_wmProtocols;
    QString m_name;
};

#endif
