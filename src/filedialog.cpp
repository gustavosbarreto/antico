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
    read_settings();
    init();
    set_category_menu();
}

Filedialog::Filedialog(QWidget *parent) : QDialog(parent) // without Category menu
{
    cat_menu = NULL;
    setSizeGripEnabled(true);
    setWindowModality(Qt::WindowModal);
    read_settings();
    init();
}

Filedialog::~Filedialog()
{
    delete cat_menu;
    delete layout;
    delete hor_layout;
    delete dir_model;
    delete completer;
    delete main_menu;
    delete line_path;
    delete prov;
    delete &menu_list;
    delete &delete_file_pix;
    delete &open_with_pix;
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
    open_with_pix = stl_path + style->value("open_with_pix").toString();
    style->endGroup(); //Other
    style->beginGroup("Message");
    ok_button_pix_path = stl_path + style->value("ok_button_pix").toString();
    close_button_pix_path = stl_path + style->value("close_button_pix").toString();
    style->endGroup(); // Message
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
    layout = new QVBoxLayout();
    setLayout(layout);
    message = new QLabel(this); // show message
    message->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    hor_layout = new QHBoxLayout();
    line_path = new QLineEdit(this); // show selection path
    hidden_files = new QRadioButton(tr("Show hidden files"), this);
    preview_label = new QLabel(this); // show pixmap preview
    preview_label->setMaximumSize(32, 32);
    preview_label->setScaledContents(true);
    dir_model = new QDirModel(this);
    dir_model->setReadOnly(false);
    dir_model->setSupportedDragActions(Qt::LinkAction);

    button_box = new QDialogButtonBox(this);
    ok = new QPushButton(QIcon(QPixmap(ok_button_pix_path)), tr("Ok"));
    close = new QPushButton(QIcon(QPixmap(close_button_pix_path)), tr("Close"));

    completer = new QCompleter(this);
    completer->setModel(dir_model);
    line_path->setCompleter(completer);
    prov = new Fileicon(); // get the files icon
    dir_model->setIconProvider(prov);
    tree_view = new QTreeView(this);
    tree_view->setModel(dir_model);
    tree_view->setDragEnabled(true);
    hor_layout->addWidget(preview_label);
    hor_layout->addWidget(line_path);
    layout->addWidget(message);
    layout->addLayout(hor_layout);
    layout->addWidget(hidden_files);
    layout->addWidget(tree_view);
    layout->addWidget(button_box);

    connect(tree_view, SIGNAL(pressed(QModelIndex)), this, SLOT(show_path(QModelIndex)));
    connect(tree_view, SIGNAL(pressed(QModelIndex)), this, SLOT(show_preview(QModelIndex)));
    connect(hidden_files, SIGNAL(toggled(bool)), this, SLOT(show_hidden(bool)));
    connect(line_path, SIGNAL(returnPressed()), this, SLOT(path_completer()));
    connect(completer, SIGNAL(activated(QModelIndex)), this, SLOT(update_tree(QModelIndex)));
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

    QAction *del_file = main_menu->addAction(QIcon(delete_file_pix), tr("Delete"));
    connect(del_file, SIGNAL(triggered()), this, SLOT(del_file()));
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
    QModelIndex selection = tree_view->currentIndex();
    QString name = get_selected_name();

    if (dir_model->isDir(selection)) // test if is a directory
    {
        if (dir_model->rmdir(selection))
        {
            Msgbox msg;
            msg.set_header(tr("INFORMATION"));
            msg.set_info("<b>" + name + "</b>" + " " + tr("deleted"));
            msg.set_icon("Information");
            msg.exec();
        }
    }
    else if (dir_model->remove(selection)) // else is a file
    {
        Msgbox msg;
        msg.set_header(tr("INFORMATION"));
        msg.set_info("<b>" + name + "</b>" + " " + tr("deleted"));
        msg.set_icon("Information");
        msg.exec();
    }
}

void Filedialog::set_path(const QString &pth) // for folder navigation from deskfolder
{
    tree_view->setRootIndex(dir_model->index(pth));
    tree_view->collapseAll();
    line_path->setText(pth);
}

void Filedialog::show_path(const QModelIndex &index)
{
    line_path->setText(dir_model->filePath(index));
}

void Filedialog::show_preview(const QModelIndex &index) // show file pixmap preview
{
    preview_label->setPixmap(QPixmap(dir_model->filePath(index)));
}

void Filedialog::path_completer() // on user button press in line_path
{
    qDebug() << "Path completer:" << line_path->text();
    QModelIndex index = dir_model->index(line_path->text());

    if (index.isValid())
    {
        tree_view->setCurrentIndex(index);
    }
    else
    {
        reject();
    }
}

void Filedialog::update_tree(const QModelIndex &index) // update the tree on path completer
{
    if (index.isValid())
    {
        QModelIndex id = dir_model->index(line_path->text());
        tree_view->scrollTo(id);
        tree_view->setCurrentIndex(id);
    }
    else
    {
        tree_view->scrollTo(index);
        tree_view->setCurrentIndex(index);
        tree_view->resizeColumnToContents(0);
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

void Filedialog::show_hidden(bool select)
{
    if (select)
        dir_model->setFilter(dir_model->filter() | QDir::Hidden); // add hidden files to filter
    else
        dir_model->setFilter(dir_model->filter() ^ QDir::Hidden); // remove hidden files from filter
}

QString Filedialog::get_selected_path() const
{
    if (tree_view->currentIndex().isValid())
    {
        QModelIndex selection = tree_view->currentIndex();

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
    if (tree_view->currentIndex().isValid())
    {
        QString name = dir_model->fileName(tree_view->currentIndex());
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
    QString icon = prov->type(fileinfo); // get the file icon
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
    if (tree_view->currentIndex().isValid() && tree_view->geometry().contains(event->pos())
            && !dir_model->isDir(tree_view->currentIndex()) && cat_menu != NULL)
    {
        cat_menu->set_cmd_arguments(get_selected_path() + get_selected_name()); // set the file path+name as argument
        main_menu->exec(event->globalPos());
    }
}

