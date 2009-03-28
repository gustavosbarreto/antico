////////////////////////////////////////
//  File      : filedialog.cpp        //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "filedialog.h"

////////////////////////////////////////

Filedialog::Filedialog(Categorymenu *menu, QWidget *parent) : QDialog(parent)
{
    cat_menu = menu;
    setSizeGripEnabled(true);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_AlwaysShowToolTips);
    read_settings();
    init();
    set_category_menu();
    set_list_mode(); // list view at startup
}

Filedialog::Filedialog(QWidget *parent) : QDialog(parent) // without Category menu
{
    cat_menu = NULL;
    setSizeGripEnabled(true);
    setWindowModality(Qt::WindowModal);
    read_settings();
    init();
    set_list_mode(); // list view at startup
}

Filedialog::~Filedialog()
{
    delete message;
    delete preview_label;
    delete layout;
    delete button_box;
    delete ok;
    delete cancel;
    delete close;
    delete antico;
    delete style;
    delete completer;
    delete root_item;
    delete bin_item;
    delete home_item;
    delete path_widget;
    delete cut_act;
    delete copy_act;
    delete paste_act;
    delete dir_model;
    delete list_view;
    delete tree_view;
    delete abstract_view;
    delete line_path;
    delete upper_dir_but;
    delete icon_but;
    delete list_but;
    delete main_menu;
    delete open_menu;
    delete cat_menu;
    delete prov;
}

void Filedialog::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Other");
    delete_file_pix = stl_path + style->value("delete_file_pix").toString();
    cut_file_pix = stl_path + style->value("cut_file_pix").toString();
    copy_file_pix = stl_path + style->value("copy_file_pix").toString();
    paste_file_pix = stl_path + style->value("paste_file_pix").toString();
    open_with_pix = stl_path + style->value("open_with_pix").toString();
    list_view_pix = stl_path + style->value("list_view_pix").toString();
    icon_view_pix = stl_path + style->value("icon_view_pix").toString();
    upper_dir_pix = stl_path + style->value("upper_dir_pix").toString();
    style->endGroup(); //Other
    style->beginGroup("Message");
    ok_button_pix_path = stl_path + style->value("ok_button_pix").toString();
    close_button_pix_path = stl_path + style->value("close_button_pix").toString();
    style->endGroup(); // Message
    style->beginGroup("Deskfolder");
    folder_pix = stl_path + style->value("d_folder_pix").toString();
    style->endGroup(); //Deskfolder
}

void Filedialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(0, 0, width(), height());
}

void Filedialog::init()
{
    message = new QLabel(this); // show message
    message->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    message->setMaximumHeight(20);

    QHBoxLayout *path_layout = new QHBoxLayout();
    preview_label = new QLabel(this); // show pixmap preview
    preview_label->setMaximumSize(32, 32);
    preview_label->setScaledContents(true);
    line_path = new QLineEdit(this); // show path selection
    upper_dir_but = new QPushButton(QIcon(upper_dir_pix), "", this);
    upper_dir_but->setToolTip(tr("Upper directory"));
    path_layout->addWidget(preview_label);
    path_layout->addWidget(line_path);
    path_layout->addWidget(upper_dir_but);

    QHBoxLayout *view_layout = new QHBoxLayout();
    QRadioButton *hidden_radio = new QRadioButton(tr("Show hidden files"), this);
    icon_but = new QPushButton(QIcon(icon_view_pix), tr("Icons"), this);
    list_but = new QPushButton(QIcon(list_view_pix), tr("List"), this);
    hidden_radio->setAutoExclusive(false);
    icon_but->setAutoExclusive(true);
    list_but->setAutoExclusive(true);
    view_layout->addWidget(hidden_radio);
    view_layout->addWidget(icon_but);
    view_layout->addWidget(list_but);
    view_layout->addStretch(1);

    dir_model = new QDirModel(this);
    dir_model->setSupportedDragActions(Qt::LinkAction);

    completer = new QCompleter(this);
    completer->setModel(dir_model);
    line_path->setCompleter(completer); // to complete the path

    prov = new Fileicon(); // get the files icon
    dir_model->setIconProvider(prov);

    tree_view = new QTreeView(this);
    tree_view->setModel(dir_model);
    tree_view->setDragEnabled(true);
    tree_view->setItemsExpandable(false);
    tree_view->setRootIsDecorated(false);
    tree_view->setSortingEnabled(true);
    tree_view->setIconSize(QSize(16, 16));

    list_view = new QListView(this);
    list_view->setModel(dir_model);
    list_view->setDragEnabled(true);
    list_view->setFlow(QListView::LeftToRight);
    list_view->setResizeMode(QListView::Adjust);
    list_view->setViewMode(QListView::IconMode);
    list_view->setUniformItemSizes(true);
    list_view->setGridSize(QSize(70, 70));
    list_view->setSpacing(5);
    list_view->setIconSize(QSize(32, 32));

    button_box = new QDialogButtonBox(this);
    ok = new QPushButton(QIcon(QPixmap(ok_button_pix_path)), tr("Ok"));
    close = new QPushButton(QIcon(QPixmap(close_button_pix_path)), tr("Close"));

    path_widget = new QListWidget();
    path_widget->setMaximumWidth(150);
    root_item = new QListWidgetItem(path_widget);
    bin_item = new QListWidgetItem(path_widget);
    home_item = new QListWidgetItem(path_widget);
    root_item->setIcon(QIcon(folder_pix));
    bin_item->setIcon(QIcon(folder_pix));
    home_item->setIcon(QIcon(folder_pix));
    root_item->setText(tr("/"));
    bin_item->setText(tr("/usr/bin/"));
    home_item->setText(tr("/home/"));

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(path_widget);
    splitter->addWidget(tree_view);
    splitter->addWidget(list_view);

    layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(message);
    layout->addLayout(view_layout);
    layout->addLayout(path_layout);
    layout->addWidget(splitter);
    layout->addWidget(button_box);

    connect(path_widget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(change_path(QListWidgetItem *, QListWidgetItem *)));
    connect(tree_view, SIGNAL(pressed(QModelIndex)), this, SLOT(show_preview(QModelIndex)));
    connect(tree_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(update_list(QModelIndex)));
    connect(list_view, SIGNAL(pressed(QModelIndex)), this, SLOT(show_preview(QModelIndex)));
    connect(list_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(update_list(QModelIndex)));
    connect(hidden_radio, SIGNAL(toggled(bool)), this, SLOT(show_hidden(bool)));
    connect(icon_but, SIGNAL(pressed()), this, SLOT(set_icon_mode()));
    connect(list_but, SIGNAL(pressed()), this, SLOT(set_list_mode()));
    connect(line_path, SIGNAL(returnPressed()), this, SLOT(path_completer()));
    connect(upper_dir_but, SIGNAL(pressed()), this, SLOT(upper_dir()));
    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));
}

void Filedialog::set_category_menu()
{
    // show the Category apps list for open the file
    main_menu = new QMenu();
    open_menu = main_menu->addMenu(QIcon(open_with_pix), tr("Open with"));

    menu_list = cat_menu->get_menus();
    for (int i = 0; i <  menu_list.size(); ++i)
    {
        open_menu->addMenu(menu_list.at(i));
    }

    QAction *del_act = main_menu->addAction(QIcon(delete_file_pix), tr("Delete"));
    cut_act = main_menu->addAction(QIcon(cut_file_pix), tr("Cut..."));
    copy_act = main_menu->addAction(QIcon(copy_file_pix), tr("Copy..."));
    paste_act = main_menu->addAction(QIcon(paste_file_pix), tr("Paste"));

    cut_act->setEnabled(true);
    copy_act->setEnabled(true);
    paste_act->setEnabled(false);

    connect(main_menu, SIGNAL(aboutToHide()), this, SLOT(reset_actions()));
    connect(del_act, SIGNAL(triggered()), this, SLOT(del_file()));
    connect(cut_act, SIGNAL(triggered()), this, SLOT(cut_file()));
    connect(copy_act, SIGNAL(triggered()), this, SLOT(copy_file()));
    connect(paste_act, SIGNAL(triggered()), this, SLOT(paste_file()));
}

void Filedialog::reset_actions() // reset copy/paste action buttons
{
    cut_act->setEnabled(true);
    copy_act->setEnabled(true);
    paste_act->setEnabled(false);
}

void Filedialog::set_type(const QString &text, const QString &button_type) // set filedialog type
{
    message->setText(text); // show message

    if (button_type.compare("OK_Cancel") == 0)
    {
        button_box->addButton(ok, QDialogButtonBox::AcceptRole);
        button_box->addButton(close, QDialogButtonBox::RejectRole);

    }
    if (button_type.compare("Close") == 0)
    {
        button_box->removeButton(ok);
        button_box->addButton(close, QDialogButtonBox::RejectRole);
    }
}

void Filedialog::del_file()
{
    QModelIndex selection = abstract_view->currentIndex();
    QString name = get_selected_name();
    QString path = get_selected_path();

    trash_path = QDir::homePath() + "/.local/share"; // search in default path directory

    if (dir_model->isDir(selection)) // test if is a directory
    {
        // create the .trashinfo file
        QString trash_info = name + ".trashinfo";
        QSettings settings(trash_path + "/Trash/info/" + trash_info, QSettings::IniFormat);
        settings.beginGroup("Trash Info");
        settings.setValue("Path", path);
        settings.setValue("DeletionDate", QDateTime::currentDateTime().toString(Qt::ISODate));
        settings.endGroup(); // Trash Info

        QStringList rem_info_args;
        rem_info_args <<  path << trash_path + "/Trash/files/";

        if (QProcess::startDetached("/bin/mv", rem_info_args)) // remove the directory
        {
            dir_model->refresh(); // update the View

            Msgbox msg;
            msg.set_header(tr("INFORMATION"));
            msg.set_info("<b>" + name + "</b>" + " " + tr("deleted and moved in") + " " + "<b>" + trash_path + "/Trash/files/" + "</b>");
            msg.set_icon("Information");
            msg.exec();
        }
    }
    else // else is a file
    {
        // create the .trashinfo file
        QString trash_info = name + ".trashinfo";
        QSettings settings(trash_path + "/Trash/info/" + trash_info, QSettings::IniFormat);
        settings.beginGroup("Trash Info");
        settings.setValue("Path", path + name);
        settings.setValue("DeletionDate", QDateTime::currentDateTime().toString(Qt::ISODate));
        settings.endGroup(); // Trash Info

        QStringList rem_info_args;
        rem_info_args <<  path + name << trash_path + "/Trash/files/";

        if (QProcess::startDetached("/bin/mv", rem_info_args)) // remove the file
        {
            dir_model->refresh(); // update the View

            Msgbox msg;
            msg.set_header(tr("INFORMATION"));
            msg.set_info("<b>" + name + "</b>" + " " + tr("deleted and moved in") + " " + trash_path + "/Trash/files/");
            msg.set_icon("Information");
            msg.exec();
        }
    }
}

void Filedialog::cut_file()
{
    QModelIndex selection = abstract_view->currentIndex();
    QString name = get_selected_name();
    QString path = get_selected_path();

    if (dir_model->isDir(selection)) // test if is a directory
        source_path = path;
    else // is a file
        source_path = path + name;

    command = "/bin/mv"; // the same for file or directory

    cut_act->setEnabled(false);
    copy_act->setEnabled(false);
    paste_act->setEnabled(true);
}

void Filedialog::copy_file()
{
    QModelIndex selection = abstract_view->currentIndex();
    QString name = get_selected_name();
    QString path = get_selected_path();

    if (dir_model->isDir(selection)) // test if is a directory
    {
        command = "/bin/cp -R";
        source_path = path; // add source
    }
    else // is a file
    {
        command = "/bin/cp";
        source_path = path + name; // add source
    }

    cut_act->setEnabled(false);
    copy_act->setEnabled(false);
    paste_act->setEnabled(true);
}

void Filedialog::paste_file()
{
    QModelIndex selection = abstract_view->currentIndex();
    QString name = get_selected_name();
    QString path = get_selected_path();

    if (dir_model->isDir(selection)) // test if is a directory
    {
        QString destination_path = path + "/";

        command.append(" ").append(source_path).append(" ").append(destination_path); // add source + destination to command

        qDebug() << "Paste command:" << command;

        if (QProcess::startDetached(command))
        {
            dir_model->refresh(); // update the View

            Msgbox msg;
            msg.set_header(tr("INFORMATION"));
            msg.set_info("<b>" + source_path + "</b>" + " " + tr("pasted in") + " " + "<b>" + destination_path + "</b>");
            msg.set_icon("Information");
            msg.exec();
        }

        source_path.clear();
        command.clear();
        cut_act->setEnabled(true);
        copy_act->setEnabled(true);
        paste_act->setEnabled(false);
    }
}

void Filedialog::change_path(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    set_path(current->text());
}

void Filedialog::set_path(const QString &pth) // for folder navigation from deskfolder
{
    abstract_view->setRootIndex(dir_model->index(pth));
    line_path->setText(pth);
}

void Filedialog::path_completer() // on user button press in line_path
{
    update_list(dir_model->index(line_path->text()));
}

void Filedialog::upper_dir() // go to the upper directory
{
    update_list(dir_model->index(line_path->text()).parent());
}

void Filedialog::update_list(const QModelIndex &index) // update the list on path completer
{
    if (index.isValid() && dir_model->isDir(index))
    {
        set_path(dir_model->filePath(index));
    }
}

void Filedialog::set_filter(QDir::Filters fltr)
{
    dir_model->setFilter(fltr);
}

void Filedialog::set_name_filters(const QStringList &fltr)
{
    dir_model->setNameFilters(fltr);
}

void Filedialog::set_read_only(bool b)
{
    dir_model->setReadOnly(b);
}

void Filedialog::show_preview(const QModelIndex &index) // show file pixmap preview
{
    preview_label->setPixmap(QPixmap(dir_model->filePath(index)));
}

void Filedialog::show_hidden(bool select)
{
    if (select)
        dir_model->setFilter(dir_model->filter() | QDir::Hidden); // add hidden files to filter
    else
        dir_model->setFilter(dir_model->filter() ^ QDir::Hidden); // remove hidden files from filter
}

void Filedialog::set_icon_mode()
{
    list_view->show();
    tree_view->hide();
    abstract_view = list_view; // change the view
    abstract_view->setRootIndex(dir_model->index(line_path->text()));
}

void Filedialog::set_list_mode()
{
    list_view->hide();
    tree_view->show();
    abstract_view = tree_view; // change the view
    abstract_view->setRootIndex(dir_model->index(line_path->text()));
}

QString Filedialog::get_selected_path() const
{
    if (abstract_view->currentIndex().isValid())
    {
        QModelIndex selection = abstract_view->currentIndex();

        if (dir_model->isDir(selection)) // is a directory
        {
            QString path = dir_model->filePath(selection);
            qDebug() << "Selected path:" << path;
            return path;
        }
        else // is a file
        {
            QString filepath = dir_model->filePath(selection);
            QFileInfo fileinfo(filepath);
            QString path = fileinfo.absolutePath(); // remove the file name from path
            path.append("/"); // add slash at the end
            qDebug() << "Selected path:" << path;
            return path;
        }
    }
    return "";
}

QString Filedialog::get_selected_name() const
{
    if (abstract_view->currentIndex().isValid())
    {
        QString name = dir_model->fileName(abstract_view->currentIndex());
        qDebug() << "Selected name:" << name;
        return name;
    }
    return "";
}

QString Filedialog::get_selected_icon() const
{
    QString file = get_selected_name();
    QFileInfo fileinfo(file);
    Fileicon *prov = (Fileicon *)dir_model->iconProvider();
    QString icon = prov->icon_type(fileinfo); // get the file icon
    return icon;
}

void Filedialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
        raise();
    }
}

void Filedialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Filedialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}

void Filedialog::contextMenuEvent(QContextMenuEvent *event)
{
    if (abstract_view->currentIndex().isValid() && abstract_view->geometry().contains(event->pos(), true) && cat_menu != NULL)
    {
        if (dir_model->isDir(abstract_view->currentIndex()))
            cat_menu->set_cmd_arguments(get_selected_path()); // set the dir path as argument
        else
            cat_menu->set_cmd_arguments(get_selected_path() + get_selected_name()); // set the file path+name as argument

        main_menu->exec(event->globalPos());
    }
    abstract_view->clearSelection();
    event->ignore();
}

void Filedialog::update_style()
{
    read_settings();
    icon_but->setIcon(QIcon(icon_view_pix));
    list_but->setIcon(QIcon(list_view_pix));
    upper_dir_but->setIcon(QIcon(upper_dir_pix));
}