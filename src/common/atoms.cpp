#include <QX11Info>
#include <QDebug>

#include "atoms.hpp"

static const char *atomNames = {
    "_NET_CLIENT_LIST\0"
    "_NET_ACTIVE_WINDOW\0"

    "_NET_WM_WINDOW_TYPE\0"
    "_NET_WM_WINDOW_TYPE_DESKTOP\0"
    "_NET_WM_WINDOW_TYPE_TOOLBAR\0"
    "_NET_WM_WINDOW_TYPE_DOCK\0"
    "_NET_WM_WINDOW_TYPE_SPLASH\0"
    "_NET_WM_WINDOW_TYPE_NORMAL\0"
    "_NET_WM_WINDOW_TYPE_DIALOG\0"
    "_NET_WM_WINDOW_TYPE_MENU\0"
    "_NET_WM_WINDOW_TYPE_UTILITY\0"

    "_NET_WM_STATE\0"
    "_NET_WM_STATE_STICKY\0"
    "_NET_WM_STATE_SKIP_PAGER\0"
    "_NET_WM_STATE_SKIP_TASKBAR\0"

    "_NET_WM_STRUT_PARTIAL\0"

    "_NET_SYSTEM_TRAY_OPCODE\0"

    "_NET_WM_NAME\0"
    "_NET_WM_ICON\0"

    "_NET_WM_USER_TIME\0"

    "WM_DELETE_WINDOW\0"
    "WM_PROTOCOLS\0"
    "WM_STATE\0"
    "WM_CHANGE_STATE\0"
    "WM_TAKE_FOCUS\0"
    "WM_HINTS\0"
    "WM_NAME\0"
    "WM_NORMAL_HINTS\0"
    "WM_TRANSIENT_FOR\0"
    "WM_COLORMAPS\0"
    "WM_COLORMAP_WINDOWS\0"
    "WM_RESOURCE_MANAGER\0"

    "XA_RESOURCE_MANAGER\0"
    "XA_CLASS\0"

    "STRING\0"
    "UTF8_STRING\0"
};

InternalAtoms *atomList = 0;

void createAtomList()
{
    atomList = new InternalAtoms;

    const char *names[InternalAtoms::NAtoms];
    const char *ptr = atomNames;

    int i = 0;
    while (*ptr)
    {
        names[i++] = ptr;
        while (*ptr)
            ++ptr;
        ++ptr;
    }

    Q_ASSERT(i == InternalAtoms::NAtoms);

    for (i = 0; i < InternalAtoms::NAtoms; ++i)
        atomList->atoms[i] = XInternAtom(QX11Info::display(), names[i], False);
}
