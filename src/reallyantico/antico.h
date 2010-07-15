////////////////////////////////////////
//  File      : antico.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

// X event handler
// create new frame
// set window manager init settings to "antico.cfg"
// quit window manager

////////////////////////////////////////

#ifndef ANTICO_H
#define ANTICO_H

#include <QApplication>
#include <QStringList>
#include <QHash>

class Dockbar;
class Frame;
class Desk;
class Categorymenu;
class QSettings;

#include <X11/Xutil.h>

////////////////////////////////////////

class Antico : public QObject
{
    Q_OBJECT

public:
    static Antico *self();

    Antico();
    ~Antico();
    void init();
    void create_frame(WId, Dockbar *, Desk *);
    void raise_next_frame();
    void set_active_frame(Frame *);
    void send_configurenotify(Frame *);
    void set_settings();
    void run_app_at_startup();
    void get_atoms();
    void create_gui();
    void send_supported_hints();
    void check_window_type(WId);
    void print_window_prop(WId);
    bool check_net_sys_tray_for(WId) const;
    void check_wm_transient_for(WId);
    Desk * get_desktop();
    Dockbar * get_dockbar();
    Categorymenu * get_category_menu();
    bool x11EventFilter(void *message, long *result);
   
private:
    QHash<int, Frame *> mapping_clients; // mapping client and frame (key=client_win_id value=frame)
    QHash<int, Frame *> mapping_frames; // mapping frame with their winId (key=frame_win_id value=frame)
    QList<Frame *> frm_list;
    QStringList frame_type;
    int servershapes; // server supports shapes
    int ShapeEventBase; // event base for shape extension
    // get WM protocols required by ICCCM
    Atom wm_transient_for;
    // XDND Protocol
    Atom xdnd_aware;
    Atom xdnd_position;
    Atom xdnd_enter;
    Atom xdnd_finished;
    Atom xdnd_status;
    // Extensions
    Atom _net_wm_name;
    Atom _net_wm_icon;
    Atom _net_supported;
    Atom _net_wm_user_time;
    Atom _net_wm_state;
    Atom _net_wm_window_type;
    Atom _net_wm_window_type_normal;
    Atom _net_wm_window_type_desktop;
    Atom _net_wm_window_type_dialog;
    Atom _net_wm_window_type_splash;
    Atom _net_wm_window_type_dnd;
    Atom _kde_net_wm_system_tray_window_for;
    //////////////
    QSettings *antico;
    Dockbar *dock;
    Desk *dsk;
    Frame *frm;
    Categorymenu *cat_menu;
};
#endif
