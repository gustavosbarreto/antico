////////////////////////////////////////
//  File      : systray.cpp           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "systray.h"

////////////////////////////////////////
/* defined in the systray spec */
#define SYSTEM_TRAY_REQUEST_DOCK    0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2


Systray::Systray(QWidget *parent) : QLabel(parent)
{
    read_settings();
    setPixmap(sys_pix);
    setScaledContents(true);
    layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignRight);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);
    setLayout(layout);

    // Freedesktop.org System Tray support
    char name[20] = {0};
    qsnprintf(name, 20, "_NET_SYSTEM_TRAY_S%d", DefaultScreen(QX11Info::display()));
    net_selection_atom = XInternAtom(QX11Info::display(), name, False);
    net_opcode_atom = XInternAtom(QX11Info::display(), "_NET_SYSTEM_TRAY_OPCODE", False);
    net_manager_atom = XInternAtom(QX11Info::display(), "MANAGER", False);
    net_message_data_atom = XInternAtom(QX11Info::display(), "_NET_SYSTEM_TRAY_MESSAGE_DATA", False);

    XSetSelectionOwner(QX11Info::display(), net_selection_atom, winId(), CurrentTime);

    if (XGetSelectionOwner(QX11Info::display(), net_selection_atom) == winId())
    {
        qDebug("Setting the selection owner");
        XClientMessageEvent xev;

        xev.type = ClientMessage;
        xev.window = QApplication::desktop()->winId();
        xev.message_type = net_manager_atom;
        xev.format = 32;
        xev.data.l[0] = CurrentTime;
        xev.data.l[1] = net_selection_atom;
        xev.data.l[2] = winId();
        xev.data.l[3] = 0;        /* manager specific data */
        xev.data.l[4] = 0;        /* manager specific data */

        XSendEvent(QX11Info::display(), QApplication::desktop()->winId(), False, StructureNotifyMask, (XEvent *)&xev);
    }
}

Systray::~Systray()
{
    delete layout;
    delete s_icon;
    delete emb_cont;
}

void Systray::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Dockbar");
    sys_pix = stl_path + style->value("dock_pix").toString();
    style->endGroup(); //Dockbar
}

void Systray::add(Frame *frm)
{
    s_icon = new Sysicon(frm, this);
    sys_icons.insert(frm->winId(), s_icon); // save the Frame winId/Sysicon
    layout->addWidget(s_icon);
    updateGeometry();
    qDebug() <<"Frame added to System Tray." << "Frame name:" << frm->cl_name() << "Frame Id:" << frm->winId() << "Client Id:" << frm->cl_win();
    connect(s_icon, SIGNAL(destroy_sys(Sysicon *)), this, SLOT(remove(Sysicon *)));
}

void Systray::add(Window win_id)
{
    emb_cont = new QX11EmbedContainer(this);
    emb_cont->setPalette(QPalette(QPalette::Dark));
    emb_cont->embedClient(win_id);
    qDebug() << "Client added to System Tray." << "Client Id:" << win_id;
    layout->addWidget(emb_cont);
    emb_cont->setContentsMargins(0, 0, 0, 0);
    emb_cont->setFixedSize(qMin(25, height()), qMin(25, height()));
    updateGeometry();
    XResizeWindow(QX11Info::display(), win_id, emb_cont->width(), emb_cont->height());
    XMapRaised(QX11Info::display(), win_id);

    connect(emb_cont, SIGNAL(clientClosed()), emb_cont, SLOT(close()));
}

void Systray::remove(Window win_id) // if Sysicon is on Systray together with equal icon from _NET_SYSTEM_TRAY protocol
{
    if (sys_icons.contains(win_id))
    {
        s_icon = sys_icons.value(win_id);
        sys_icons.remove(win_id);
        qDebug() << "Sysicon remove from Systray cmd.";
        s_icon->close();
        updateGeometry();
    }
}

void Systray::remove(Sysicon *s_icon) // remove from "Close" right button mouse on SysTray
{
    sys_icons.remove(sys_icons.key(s_icon));
    qDebug() << "Sysicon remove. Num. after deletion:" << sys_icons.size();
    s_icon->close();
}

bool Systray::x11Event(XEvent *event)
{
    if (event->type == ClientMessage)
    {
        qDebug("Systray::x11Event client message");
        if (event->xclient.message_type == net_opcode_atom && event->xclient.data.l[1] == SYSTEM_TRAY_REQUEST_DOCK)
        {
            qDebug("SYSTEM_TRAY_REQUEST_DOCK");
            add(event->xclient.data.l[2]);
        }
        else if (event->xclient.message_type == net_opcode_atom && event->xclient.data.l[1] == SYSTEM_TRAY_BEGIN_MESSAGE)
        {
            qDebug("SYSTEM_TRAY_BEGIN_MESSAGE");
        }
        else if (event->xclient.message_type == net_opcode_atom && event->xclient.data.l[1] == SYSTEM_TRAY_CANCEL_MESSAGE)
        {
            qDebug("SYSTEM_TRAY_CANCEL_MESSAGE");
        }
        else if (event->xclient.data.l[1] == (signed)net_message_data_atom)
        {
            qDebug("Text message from Dockapp:\n%s", event->xclient.data.b);
        }
        return true;
    }
    return false;
}

void Systray::update_style()
{
    // update sysicons
    foreach(Sysicon *icon, sys_icons)
    icon->update_style();
}
