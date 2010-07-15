////////////////////////////////////////
//  File      : launcher.cpp          //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include <QSettings>
#include <QMenu>
#include <QMouseEvent>

#include "launcher.h"
#include "antico.h"
#include "utils.h"

////////////////////////////////////////

Launcher::Launcher(Antico *a, QWidget *parent) : QLabel(parent)
{
    app = a;
    read_settings();
    init();
    setPixmap(QPixmap(launcher_pix).scaled(dock_height-5, dock_height-5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    show();
}

Launcher::~Launcher()
{
    delete app;
    delete antico;
}

void Launcher::read_settings()
{
    // get style path
    antico = new QSettings(QSettings::UserScope, "antico", "antico", this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get action icons
    QSettings *style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Launcher");
    launcher_pix = stl_path + style->value("launcher_pix").toString();
    style->endGroup(); // Launcher
    style->beginGroup("Dockbar");
    dock_height = style->value("dock_height").toInt();
    dock_position = style->value("dock_position").toInt();
    style->endGroup(); //Dockbar
}

void Launcher::init()
{
    main_menu = new QMenu(this);
    connect(main_menu, SIGNAL(triggered(QAction *)), this, SLOT(run_command(QAction *))); // Quit, Run, Refresh, Manager

    // add Category menu on Launcher
    QList <QMenu *> menu_list = app->get_category_menu()->get_menus();
    for (int i = 0; i <  menu_list.size(); ++i)
    {
        main_menu->addMenu(menu_list.at(i)); // add Category menu on Launcher
    }
    
    main_menu->addSeparator();
}

void Launcher::run_command(QAction *act)
{
    QString cmd = act->data().toString();
}

void Launcher::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint p = mapToGlobal(QPoint(0, 0));

        if (dock_position == 0) // 0 = bottom / 1 = top
            p.setY(p.y()-main_menu->sizeHint().height());
        else
            p.setY(p.y()+height());

        main_menu->popup(p);
    }
}

void Launcher::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setPixmap(QPixmap(launcher_pix).scaled(dock_height, dock_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void Launcher::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setPixmap(QPixmap(launcher_pix).scaled(dock_height-5, dock_height-5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void Launcher::update_style()
{
    read_settings();
    setPixmap(QPixmap(launcher_pix).scaled(dock_height-5, dock_height-5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    app->get_category_menu()->update_menu(); // update .desktop/user menu entry
    app->get_category_menu()->update_style(); // update category menu pixmap
}




