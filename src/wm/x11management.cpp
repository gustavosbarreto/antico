#include <QDebug>
#include <QSettings>
#include <QX11Info>
#include <QDesktopWidget>
#include <QProcess>
#include <QDir>

#include "x11management.hpp"
#include "frame.hpp"

#include <X11/extensions/shape.h>

// //////////////////////////////////////

/* define the qDebug() color */
#define RED     "\e[0;31m"
#define GREEN   "\e[0;32m"
#define YELLOW  "\e[0;33m"
#define MAGENTA "\e[0;35m"
#define WHITE   "\e[0;37m"


Antico::Antico(int &argc, char **argv): QApplication(argc, argv)
{
    set_event_names();
    // for [Alt+Tab] key combination
    next_frame = 0;
    // get the atoms (ICCCM/EWMH)
    get_atoms();
    // set application settings on first installation
    set_settings();
    // send ClientMessage to root for supported hints
    send_supported_hints();
    // check if server supports nonrectangular windows
    int err;
    servershapes = XShapeQueryExtension(QX11Info::display(), &ShapeEventBase, &err);
}

Antico::~Antico()
{
}

void Antico::set_event_names()
{
    event_names.insert(0, "");
    event_names.insert(1, "");
    event_names.insert(2, "KeyPress");
    event_names.insert(3, "KeyRelease");
    event_names.insert(4, "ButtonPress");
    event_names.insert(5, "ButtonRelease");
    event_names.insert(6, "MotionNotify");
    event_names.insert(7, "EnterNotify");
    event_names.insert(8, "LeaveNotify");
    event_names.insert(9, "FocusIn");
    event_names.insert(10, "FocusOut");
    event_names.insert(11, "KeymapNotify");
    event_names.insert(12, "Expose");
    event_names.insert(13, "GraphicsExpose");
    event_names.insert(14, "NoExpose");
    event_names.insert(15, "VisibilityNotify");
    event_names.insert(16, "CreateNotify");
    event_names.insert(17, "DestroyNotify");
    event_names.insert(18, "UnmapNotify");
    event_names.insert(19, "MapNotify");
    event_names.insert(20, "MapRequest");
    event_names.insert(21, "ReparentNotify");
    event_names.insert(22, "ConfigureNotify");
    event_names.insert(23, "ConfigureRequest");
    event_names.insert(24, "GravityNotify");
    event_names.insert(25, "ResizeRequest");
    event_names.insert(26, "CirculateNotify");
    event_names.insert(27, "CirculateRequest");
    event_names.insert(28, "PropertyNotify");
    event_names.insert(29, "SelectionClear");
    event_names.insert(30, "SelectionRequest");
    event_names.insert(31, "SelectionNotify");
    event_names.insert(32, "ColormapNotify");
    event_names.insert(33, "ClientMessage");
    event_names.insert(34, "MappingNotify");
}

void Antico::get_atoms()
{
    // get WM protocols required by ICCCM
    wm_protocols = XInternAtom(QX11Info::display(), "WM_PROTOCOLS", False);
    wm_delete_window = XInternAtom(QX11Info::display(), "WM_DELETE_WINDOW", False);
    wm_change_state = XInternAtom(QX11Info::display(), "WM_CHANGE_STATE", False);
    wm_state = XInternAtom(QX11Info::display(), "WM_STATE", False);
    wm_take_focus = XInternAtom(QX11Info::display(), "WM_TAKE_FOCUS", False);
    wm_resource_manager = XInternAtom(QX11Info::display(), "RESOURCE_MANAGER", False);
    wm_colormaps = XInternAtom(QX11Info::display(), "WM_COLORMAP_WINDOWS", False);
    wm_hints = XInternAtom(QX11Info::display(), "WM_HINTS", False);
    wm_normal_hints = XInternAtom(QX11Info::display(), "WM_NORMAL_HINTS", False);
    wm_name = XInternAtom(QX11Info::display(), "WM_NAME", False);
    wm_transient_for = XInternAtom(QX11Info::display(), "WM_TRANSIENT_FOR", False);
    xdnd_aware = XInternAtom(QX11Info::display(), "XdndAware", False);
    xdnd_position = XInternAtom(QX11Info::display(), "XdndPosition", False);
    xdnd_enter = XInternAtom(QX11Info::display(), "XdndEnter", False);
    xdnd_finished = XInternAtom(QX11Info::display(), "XdndFinished", False);
    xdnd_status = XInternAtom(QX11Info::display(), "XdndStatus", False);

    // extensions required by EWMH
    _net_wm_name = XInternAtom(QX11Info::display(), "_NET_WM_NAME", False);
    _net_wm_icon = XInternAtom(QX11Info::display(), "_NET_WM_ICON", False);
    _net_wm_user_time = XInternAtom(QX11Info::display(), "_NET_WM_USER_TIME", False);
    _net_wm_state = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    _net_supported = XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);
    _net_wm_window_type = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    _net_wm_window_type_normal = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_NORMAL", False);
    _net_wm_window_type_desktop = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    _net_wm_window_type_dialog = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DIALOG", False);
    _net_wm_window_type_splash = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_SPLASH", False);
    _net_wm_window_type_dnd = XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE_DND", False);
    _kde_net_wm_system_tray_window_for = XInternAtom(QX11Info::display(), "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR", False);
}

void Antico::send_supported_hints()
{
    // maximum five 32-bit values for message
    Atom _net_supported = XInternAtom(QX11Info::display(), "_NET_SUPPORTED", False);

    qDebug("Sending _NET_SUPPORTED");
    XClientMessageEvent xev1;
    XClientMessageEvent xev2;
    XClientMessageEvent xev3;
    XClientMessageEvent xev4;
    XClientMessageEvent xev5;
    XClientMessageEvent xev6;

    xev1.type = ClientMessage;
    xev1.window = QApplication::desktop()->winId();
    xev1.message_type = _net_supported;
    xev1.format = 32;
    xev1.data.l[0] = wm_protocols;
    xev1.data.l[1] = wm_delete_window;
    xev1.data.l[2] = wm_change_state;
    xev1.data.l[3] = wm_state;
    xev1.data.l[4] = wm_take_focus;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev1);

    xev2.type = ClientMessage;
    xev2.window = QApplication::desktop()->winId();
    xev2.message_type = _net_supported;
    xev2.format = 32;
    xev2.data.l[0] = wm_resource_manager;
    xev2.data.l[1] = wm_colormaps;
    xev2.data.l[2] = wm_hints;
    xev2.data.l[3] = wm_normal_hints;
    xev2.data.l[4] = wm_name;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev2);

    xev3.type = ClientMessage;
    xev3.window = QApplication::desktop()->winId();
    xev3.message_type = _net_supported;
    xev3.format = 32;
    xev3.data.l[0] = wm_transient_for;
    xev3.data.l[1] = _net_wm_name;
    xev3.data.l[2] = _net_wm_icon;
    xev3.data.l[3] = _net_supported;
    xev3.data.l[4] = _net_wm_user_time;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev3);

    xev4.type = ClientMessage;
    xev4.window = QApplication::desktop()->winId();
    xev4.message_type = _net_supported;
    xev4.format = 32;
    xev4.data.l[0] = _net_wm_window_type;
    xev4.data.l[1] = _net_wm_window_type_normal;
    xev4.data.l[2] = _net_wm_window_type_desktop;
    xev4.data.l[3] = _net_wm_window_type_dialog;
    xev4.data.l[4] = _net_wm_window_type_splash;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev4);

    xev5.type = ClientMessage;
    xev5.window = QApplication::desktop()->winId();
    xev5.message_type = _net_supported;
    xev5.format = 32;
    xev5.data.l[0] = _kde_net_wm_system_tray_window_for;
    xev5.data.l[1] = _net_wm_state;
    xev5.data.l[2] = _net_wm_window_type_dnd;
    xev5.data.l[3] = xdnd_aware;
    xev5.data.l[4] = xdnd_position;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev5);

    xev6.type = ClientMessage;
    xev6.window = QApplication::desktop()->winId();
    xev6.message_type = _net_supported;
    xev6.format = 32;
    xev6.data.l[0] = xdnd_enter;
    xev6.data.l[1] = xdnd_finished;
    xev6.data.l[2] = xdnd_status;
    XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False,
               (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev6);
}

bool Antico::x11EventFilter(XEvent *event)
{
    Frame *frm;
    XWindowAttributes wa;
    XPropertyEvent *pev;
    XClientMessageEvent *mev;
    KeySym sym;
    uint mod;
    uint keymask1 = Mod1Mask & 0x0F;

    if (event->type != 6 && event->type != 12) // ignore Motion/Expose event
    {
        qDebug() << "--------------------------------------------------------------------------------------------";
        qDebug() << GREEN "XEvent:" YELLOW << event_names.value(event->type) << WHITE " ( WId:" MAGENTA << event->xany.window << WHITE ")";
    }

    switch (event->type)
    {
    // ///////////////// REQUEST EVENTS ///////////////////
    case MapRequest:
        qDebug() << "[MapRequest]" << event->xmaprequest.window;

        if (!XGetWindowAttributes(QX11Info::display(), event->xmaprequest.window, &wa))
            return true;

        if (wa.override_redirect)
        {
            qDebug() << "No Frame for Override redirect on Client:" << event->xmaprequest.window;
            return true;
        }

        if ((frm = mapping_clients.value(event->xmaprequest.window)) != NULL)
        {
            qDebug() << "--> MapRequest for already mapped Client:" << event->xmaprequest.window;
            frm->map_it();
        }
        else
        {
            qDebug() << "--> MapRequest for new Client:" << event->xmaprequest.window;
            create_frame(event->xmaprequest.window); // create new Frame for Client
        }
        return false;
        break;

    case ConfigureRequest:
        qDebug() << "[ConfigureRequest]";
        XWindowChanges wc;

        if ((frm = mapping_clients.value(event->xconfigurerequest.window)) != NULL)
        {
            qDebug() << "--> Request from map frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xconfigurerequest.window;

            if (event->xconfigurerequest.value_mask & (CWWidth | CWHeight | CWX | CWY))
            {
                int cx, cy, cw, ch;

                if (event->xconfigurerequest.value_mask & CWWidth)
                    cw = event->xconfigurerequest.width;
                else
                    cw = frm->cl_w();

                if (event->xconfigurerequest.value_mask & CWHeight)
                    ch = event->xconfigurerequest.height;
                else
                    ch = frm->cl_h();

                if ((event->xconfigurerequest.value_mask & CWX))
                    cx = event->xconfigurerequest.x;
                else
                    cx = frm->cl_x();

                if ((event->xconfigurerequest.value_mask & CWY))
                    cy = event->xconfigurerequest.y;
                else
                    cy = frm->cl_y();

                frm->resize_request(cw, ch);
                event->xconfigurerequest.value_mask &= ~(CWWidth | CWHeight | CWX | CWY);
            }

            if (!event->xconfigurerequest.value_mask)
                return true;

            wc.width = frm->width();
            wc.height = frm->height();
            wc.x = frm->x();
            wc.y = frm->y();
            wc.border_width = 0;
            wc.sibling = event->xconfigurerequest.above;
            wc.stack_mode = event->xconfigurerequest.detail;
            XConfigureWindow(QX11Info::display(), frm->winId(), event->xconfigurerequest.value_mask, &wc);
            send_configurenotify(frm);
        }
        else  // never mapped window
        {
            qDebug() << "--> Request from unmap client:" << event->xconfigurerequest.window;
            wc.x = event->xconfigurerequest.x;
            wc.y = event->xconfigurerequest.y;
            wc.width = event->xconfigurerequest.width;
            wc.height = event->xconfigurerequest.height;
            event->xconfigurerequest.value_mask &= (CWX | CWY | CWWidth | CWHeight);
            XConfigureWindow(QX11Info::display(), event->xconfigurerequest.window, event->xconfigurerequest.value_mask, &wc);
        }
        return false;
        break;

    case CirculateRequest:
        qDebug() << "[CirculateRequest]";

        if ((frm = mapping_clients.value(event->xcirculaterequest.window)) != NULL)
        {
            if (event->xcirculaterequest.place == PlaceOnTop)
            {
                frm->raise_it();
                qDebug() << "--> CirculateRequest for frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xcirculaterequest.window;
            }
        }
        else
            XLowerWindow(QX11Info::display(), event->xcirculaterequest.window);

        return false;
        break;

    // ///////////////// NOTIFY EVENTS ///////////////////

    case MapNotify:
        qDebug() << "[MapNotify]";

        if ((frm = mapping_clients.value(event->xunmap.window)) != NULL)
        {
            frm->map_it();
            qDebug() << "MapNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name() << " - Client:" << frm->cl_win();
            return true;
        }
        if (event->xmap.event != event->xmap.window)
            return true;

        return false;
        break;

    case UnmapNotify:
        qDebug() << "[UnmapNotify]";

        if ((frm = mapping_clients.value(event->xunmap.window)) != NULL)
        {
            if (event->xunmap.send_event)
            {
                frm->withdrawn_it();
                qDebug() << "Withdrawn for frame:" << frm->winId() << "- Name:" << frm->cl_name() << " - Client:" << frm->cl_win();
            }
            else
            {
                frm->unmap_it();
                qDebug() << "Unmap for frame:" << frm->winId() << "- Name:" << frm->cl_name() << " - Client:" << frm->cl_win();
            }
            return true;
        }
        if (event->xunmap.event != event->xunmap.window)
            return true;

        return false;
        break;

    case DestroyNotify:
        qDebug() << "[DestroyNotify]";

        if ((frm = mapping_clients.value(event->xdestroywindow.window)) != NULL ||
            (frm = mapping_frames.value(event->xdestroywindow.window)) != NULL)
        {
            qDebug() << "--> Destroy frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xdestroywindow.window;
            mapping_clients.remove(event->xdestroywindow.window);
            mapping_frames.remove(frm->winId());
            delete frm;
            return true;
        }
        if (event->xdestroywindow.event != event->xdestroywindow.window)
        {

            return true;
        }
        qDebug() << "Mapping clients num:" << mapping_clients.size() << "Mapping frames num:" << mapping_frames.size();
        return false;
        break;

    case EnterNotify:
        qDebug() << "[EnterNotify]";

        if ((frm = mapping_frames.value(event->xcrossing.window)) != NULL)
            qDebug() << "Enter in map frame:" << frm->winId();
        else
            qDebug() << "Enter in not map client:" << event->xcrossing.window;
        return false;
        break;

    case LeaveNotify:
        qDebug() << "[LeaveNotify]";

        return false;
        break;

    case VisibilityNotify:
        qDebug() << "[VisibilityNotify]";

        return false;
        break;

    case ConfigureNotify:
        qDebug() << "[ConfigureNotify]";

        if (event->xconfigure.event != event->xconfigure.window)
            return true;

        return false;
        break;

    case ReparentNotify:
        qDebug() << "[ReparentNotify]";

        if ((frm = mapping_clients.value(event->xreparent.window)) != NULL && event->xreparent.parent != frm->winId())
        {
            qDebug() << "ReparentNotify for frame:" << frm->winId() << "- Name:" << frm->cl_name();
            mapping_clients.remove(event->xreparent.window);
            return true;
        }
        else
            qDebug() << "ReparentNotify for client:" << event->xreparent.window << "- new Parent:" << event->xreparent.parent;
        return false;
        break;

    case PropertyNotify:
        qDebug() << "[PropertyNotify]";
        qDebug() << RED "Atom: " WHITE << XGetAtomName(QX11Info::display(), event->xproperty.atom) << '\n';
        pev = &event->xproperty;

        if ((frm = mapping_clients.value(event->xproperty.window)) != NULL)
        {
            qDebug() << "Client already mapped by frame:" << frm->winId() << "- Name:" << frm->cl_name() << "- Client:" << event->xproperty.window;

            if (pev->atom == wm_hints)
            {
                qDebug() << "---> wm_hints";
                frm->get_wm_hints();
                return true;
            }
            if (pev->atom == wm_normal_hints)
            {
                qDebug() << "---> wm_normal_hints";
                frm->get_wm_normal_hints();
                return true;
            }
            if (pev->atom == wm_name || pev->atom == _net_wm_name)
            {
                qDebug() << "---> wm_name";
                frm->get_wm_name();
                frm->update_name();
                return true;
            }
            if (pev->atom == wm_state || pev->atom == _net_wm_state)
            {
                qDebug() << "---> wm_state";
                qDebug() << "Window:" << pev->window << "changing state";
                return true;
            }
            if (pev->atom == wm_colormaps)
            {
                qDebug() << "---> wm_colormap_windows";
                frm->get_colormaps();
                return true;
            }
            if (pev->atom == wm_transient_for)
            {
                qDebug() << "---> wm_transient_for";
                return true;
            }
            if (pev->atom == _net_wm_user_time)
            {
                qDebug() << "---> _net_wm_user_time";
                frm->set_focus(CurrentTime);
                set_active_frame(frm);
                return true;
            }
            return true;
        }
        else if ((frm = mapping_frames.value(event->xproperty.window)) != NULL)
        {
            qDebug() << "Frame already mapped:" << event->xproperty.window << "- Name:" << frm->cl_name() << "- Client:" << frm->cl_win();
            return true;
        }
        else
        {
            qDebug() << "Client not mapped:" << event->xproperty.window;
            return false;
        }
        break;

    case ColormapNotify:
        qDebug() << "[ColormapNotify]";

        if ((frm = mapping_clients.value(event->xcolormap.window)) != NULL)
        {
            qDebug() << "ColormapNotify for frame:" << frm->winId() << " - Name:" << frm->cl_name() << " - Client:" << event->xproperty.window;
            frm->set_colormaps(event->xcolormap.colormap);
            return true;
        }
        return false;
        break;

    // ///////////////// OTHER EVENTS ///////////////////
    case ButtonPress:
        qDebug() << "[ButtonPress]";

        if ((frm = mapping_frames.value(event->xbutton.window)) != NULL) // get the frame from his winId
        {
            qDebug() << "Button press:" <<  event->xbutton.button << "for map frame:" << event->xbutton.window;
            set_active_frame(frm);
            XSetInputFocus(QX11Info::display(), event->xbutton.window, RevertToNone, CurrentTime);
        }
        else
        {
            qDebug() << "Button press:" <<  event->xbutton.button << "for client:" << event->xbutton.window;
            XSetInputFocus(QX11Info::display(), event->xbutton.window, RevertToNone, CurrentTime);
        }
        return false;
        break;

    case ClientMessage:
        qDebug() << "[ClientMessage]";
        mev = &event->xclient;

        if (mev->message_type == wm_change_state && event->xclient.format == 32 && event->xclient.data.l[0] == IconicState)
        {
            qDebug() << "---> wm_change_state: IconicState";

            if ((frm = mapping_clients.value(event->xclient.window)) != NULL)
                frm->iconify_it();
            return true;
        }
        /*  if (mev->message_type == xdnd_aware || mev->message_type == xdnd_position
        || mev->message_type == xdnd_enter || mev->message_type == xdnd_finished
        || mev->message_type == xdnd_status)
           {
              return true;
           }*/
        return false;
        break;

    case KeyPress:
        qDebug() << "[KeyPress]";

        sym = (int)XLookupKeysym(&event->xkey, 0);
        mod = event->xkey.state & keymask1;

        if (sym == XK_Tab && mod == keymask1)
        {
            qDebug() << "Press [Alt+Tab] - Scroll active apps";
            raise_next_frame();
            return false;
        }

        return false;
        break;

    default:

        if (servershapes && event->type == (ShapeEventBase + ShapeNotify))
        {
            qDebug() << "[ShapeNotify]";
            XShapeEvent *sev = (XShapeEvent *)event;

            if ((frm = mapping_clients.value(sev->window)) != NULL)
            {
                qDebug() << "ShapeNotify for frame:" << frm->winId() << " - Name:" << frm->cl_name() << " - Client:" << sev->window;
                frm->reshape();
                return true;
            }
        }
        return false;
        break;
    }
}

void Antico::create_frame(Window c_win) // create new frame around the client app
{
    print_window_prop(c_win);

    if (check_net_sys_tray_for(c_win))
        return;
    else
    {
        check_wm_transient_for(c_win);
        check_window_type(c_win);
    }

    // ///// MAP THE NEW CLIENT ////////
    qDebug() << "Mapping window type:" << frame_type.at(0);

    if (frame_type.at(0) != "Splash")
    {
        frm = new Frame(c_win, frame_type.at(0));   // select always the first type in list (preferred)
        mapping_clients.insert(c_win, frm);                     // save the client winId/frame
        mapping_frames.insert(frm->winId(), frm);               // save the frame winId/frame
    }
    else
        XMapRaised(QX11Info::display(), c_win);

    if (frame_type.at(0) != "Dialog" && frame_type.at(0) != "Splash")   // no Dockbar for Dialog/Splash frames
  

    frame_type.clear();                                                 // clear the window type list
    qDebug() << "Mapping clients num:" << mapping_clients.size() << "Mapping frames num:" << mapping_frames.size();
}

bool Antico::check_net_sys_tray_for(Window c_win) const
{
    Atom type_ret = None;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::display(), c_win, _kde_net_wm_system_tray_window_for, 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        qDebug() << "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR: WINDOW ADD TO SYSTEM TRAY";
        XFree(data);
        return true;
    }
    XFree(data);
    return false;
}

void Antico::check_window_type(Window c_win) // chech the window type before mapping
{
    Atom type_ret = None;
    Atom *win_type;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::display(), c_win, _net_wm_window_type, 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        win_type = (Atom *)data;

        for (unsigned i = 0; i < n; i++)
        {
            if (win_type[i] == _net_wm_window_type_normal)
            {
                frame_type << "Normal";
                qDebug() << "Window type: NORMAL TYPE";
            }
            else if (win_type[i] == _net_wm_window_type_dialog)
            {
                frame_type << "Dialog";
                qDebug() << "Window type: DIALOG TYPE";
            }
            else if (win_type[i] == _net_wm_window_type_splash)
            {
                frame_type << "Splash";
                qDebug() << "Window type: SPLASH TYPE";
            }
            else if (win_type[i] == _net_wm_window_type_desktop)
            {
                frame_type << "Desktop";
                qDebug() << "Window type: DESKTOP TYPE";
            }
            else // if other window type
            {
                frame_type << "Normal";
                qDebug() << "Other Window type. Set as NORMAL TYPE";
            }
        }
        return;
    }
    // / IF PROPERTY NOT SET ///
    frame_type << "Normal";
    qDebug() << "Window type not set: SET AS NORMAL";

    XFree(data);
}

void Antico::check_wm_transient_for(Window c_win)
{
    Atom type_ret = None;
    Window prop_window_return;
    unsigned char *data = 0;
    int format = 0;
    unsigned long n = 0;
    unsigned long extra = 0;

    if (XGetWindowProperty(QX11Info::display(), c_win, wm_transient_for, 0, 100, False,
                           AnyPropertyType, &type_ret, &format, &n, &extra, (unsigned char **)&data) == Success && data)
    {
        qDebug() << "Window type: WM_TRANSIENT_FOR. Set as DIALOG";

        if (XGetTransientForHint(QX11Info::display(), c_win, &prop_window_return))
        {
            qDebug() << "Window return:" << prop_window_return;
            frame_type << "Dialog";
        }
    }
    XFree(data);
}

void Antico::print_window_prop(Window c_win) // print the window properties
{
    int num_prop_ret = 0;
    Atom *list_prop = XListProperties(QX11Info::display(), c_win, &num_prop_ret);

    qDebug() << "Properties num:" << num_prop_ret;

    qDebug() << "---------------------------------------------";

    for (int i = 0; i < num_prop_ret; i++)
    {
        char *name = XGetAtomName(QX11Info::display(), list_prop[i]);
        qDebug() << i << "- Window Property:" << name;
        XFree(name);
    }

    qDebug() << "---------------------------------------------";
}

void Antico::raise_next_frame() // raise next frame on [Alt+Tab] key combination
{
    frm_list = mapping_clients.values();

    if (frm_list.size() == 0)
        return;

    if (frm_list.size() <= next_frame)
        next_frame = 0;  // start from first element

    Frame *frm = frm_list.at(next_frame);

    if (frm->win_state() != "IconicState" && frm->win_state() != "WithdrawnState" && !frm->is_splash())  // if not active

        set_active_frame(frm_list.at(next_frame));
    next_frame++;
}

void Antico::set_active_frame(Frame *frm) // activation of selected frame and unactivation of others
{
    foreach (Frame *frame, mapping_clients)
    {
        if (frame == frm)
        {
            qDebug() << "Frame raise and active: " << frame->winId() << "- Name:" << frame->cl_name() << "- state:" << frm->win_state();
            frame->set_active();
            frame->raise_it();
        }
        else
        {
            qDebug() << "Frame inactive:"  << frame->winId() << "- Name:" << frame->cl_name() << "- state:" << frm->win_state() << "- is Splash:" << frm->is_splash();
            frame->set_inactive();
        }
    }
}

void Antico::send_configurenotify(Frame *frm)
{
    XConfigureEvent ce;

    ce.type = ConfigureNotify;
    ce.event = frm->cl_win();
    ce.window = ce.event;
    ce.x = frm->cl_x();
    ce.y = frm->cl_y();
    ce.width = frm->cl_w();
    ce.height = frm->cl_h();
    ce.above = None;
    ce.border_width = 0;
    ce.override_redirect = 0;
    XSendEvent(QX11Info::display(), ce.window, False, StructureNotifyMask, (XEvent *)&ce);
}

void Antico::set_settings()
{
    // default path
    antico = new QSettings(QSettings::UserScope, "antico", "antico", this);
    // set default style on first installation, if no "/antico.cfg" is set
    if (antico->childGroups().isEmpty())
    {
        qDebug() << "Set default settings ...";
        antico->beginGroup("Style");
        antico->setValue("name", "default.stl");
        antico->setValue("path", QCoreApplication::applicationDirPath() + "/../share/antico/themes/default/");
        antico->endGroup(); // Style
        // ///////////////////////////////////////////////////////////////////////
        antico->beginGroup("Deskbar");
        antico->setValue("no_hide_iconify", "yes"); // The default is hide deskbar icon on iconify window
        antico->endGroup();                         // Deskbar
        antico->sync();
    }

    if (antico->status() == QSettings::AccessError)
        qFatal("Error on setting antico.cfg");
}
