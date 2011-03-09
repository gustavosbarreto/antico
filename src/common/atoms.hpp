#ifndef _ATOMS_HPP
#define _ATOMS_HPP

#include <X11/Xlib.h>

struct InternalAtoms;
extern InternalAtoms *atomList;

struct InternalAtoms
{
    enum X11Atom
    {
        _NET_CLIENT_LIST,
        _NET_ACTIVE_WINDOW,

        _NET_WM_WINDOW_TYPE,
        _NET_WM_WINDOW_TYPE_DESKTOP,
        _NET_WM_WINDOW_TYPE_TOOLBAR,
        _NET_WM_WINDOW_TYPE_DOCK,
        _NET_WM_WINDOW_TYPE_SPLASH,
        _NET_WM_WINDOW_TYPE_NORMAL,
        _NET_WM_WINDOW_TYPE_DIALOG,
        _NET_WM_WINDOW_TYPE_MENU,
        _NET_WM_WINDOW_TYPE_UTILITY,

        _NET_WM_STATE,
        _NET_WM_STATE_STICKY,
        _NET_WM_STATE_SKIP_PAGER,
        _NET_WM_STATE_SKIP_TASKBAR,

        _NET_WM_STRUT_PARTIAL,

        _NET_SYSTEM_TRAY_OPCODE,

        _NET_WM_NAME,
        _NET_WM_ICON,

        _NET_WM_USER_TIME,

        WM_DELETE_WINDOW,
        WM_PROTOCOLS,
        WM_STATE,
        WM_CHANGE_STATE,
        WM_TAKE_FOCUS,
        WM_HINTS,
        WM_NAME,
        WM_NORMAL_HINTS,
        WM_TRANSIENT_FOR,
        WM_COLORMAPS,
        WM_COLORMAP_WINDOWS,
        WM_RESOURCE_MANAGER,
        WM_CLASS,

        XA_RESOURCE_MANAGER,

        STRING,
        UTF8_STRING,

        NAtoms
    };

    Atom atoms[NAtoms];
};

void createAtomList();

#define ATOM(x) atomList->atoms[InternalAtoms::x]

#endif // _ATOMS_HPP
