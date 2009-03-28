////////////////////////////////////////
//  File      : trashdialog.cpp       //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "trashdialog.h"

////////////////////////////////////////

Trashdialog::Trashdialog(QWidget *parent) : QDialog(parent)
{
    setSizeGripEnabled(true);
    setWindowModality(Qt::WindowModal);
    read_settings();
    init();
}

Trashdialog::~Trashdialog()
{
    delete style;
    delete antico;
    delete line_path;
    delete dir_model;
    delete tree_view;
    delete prov;
}

void Trashdialog::read_settings()
{
    // get style path
    antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    stl_name = antico->value("name").toString();
    stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path + stl_name, QSettings::IniFormat, this);
    style->beginGroup("Message");
    ok_button_pix = stl_path + style->value("ok_button_pix").toString();
    close_button_pix = stl_path + style->value("close_button_pix").toString();
    restore_button_pix = stl_path + style->value("restore_button_pix").toString();
    remove_button_pix = stl_path + style->value("remove_button_pix").toString();
    style->endGroup(); //Message
}

void Trashdialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(0, 0, width(), height());
}

void Trashdialog::init()
{
    QVBoxLayout *trash_layout = new QVBoxLayout();
    setLayout(trash_layout);
    QLabel *name = new QLabel(tr("TRASH"), this);
    name->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    line_path = new QLineEdit(); // show trash path
    line_path->setReadOnly(true);
    dir_model = new QDirModel();
    prov = new Fileicon(); // get the files icon
    dir_model->setIconProvider(prov);
    tree_view = new QTreeView();
    tree_view->setModel(dir_model);
    tree_view->setSortingEnabled(true);
    trash_path = QDir::homePath() + "/.local/share"; // search in default path directory
    line_path->setText(trash_path + "/Trash/files");
    tree_view->setRootIndex(dir_model->index(trash_path + "/Trash/files"));
    QHBoxLayout *button_layout = new QHBoxLayout();
    QPushButton* delete_but = new QPushButton(QIcon(remove_button_pix), tr("Delete"));
    QPushButton* restore_but = new QPushButton(QIcon(restore_button_pix), tr("Restore"));
    QPushButton* close_but = new QPushButton(QIcon(close_button_pix), tr("Close"));
    button_layout->addWidget(delete_but);
    button_layout->addWidget(restore_but);
    button_layout->addWidget(close_but);
    connect(delete_but, SIGNAL(pressed()), this, SLOT(delete_pressed()));
    connect(restore_but, SIGNAL(pressed()), this, SLOT(restore_pressed()));
    connect(close_but, SIGNAL(pressed()), this, SLOT(close_pressed()));
    trash_layout->addWidget(name);
    trash_layout->addWidget(line_path);
    trash_layout->addWidget(tree_view);
    trash_layout->addLayout(button_layout);
}

void Trashdialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousepos = event->pos();
        grabMouse(QCursor(Qt::SizeAllCursor));
        raise();
    }
}

void Trashdialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->globalPos()-mousepos;
    move(p.x(), p.y());
}

void Trashdialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    releaseMouse();
}

void Trashdialog::close_pressed()
{
    hide();
}

void Trashdialog::delete_pressed()
{
    if (tree_view->currentIndex().isValid())
    {
        QString selection_path = dir_model->filePath(tree_view->currentIndex());
        QString selection_name = dir_model->fileName(tree_view->currentIndex());
        qDebug() << "Selected file path:" << selection_path;
        QString trash_info = selection_name + ".trashinfo";
        QStringList rem_info_args;
        rem_info_args <<  trash_path + "/Trash/info/" + trash_info;
        QProcess::startDetached("/bin/rm", rem_info_args); // remove the info file
        QStringList rem_file_args;
        rem_file_args << "-rf" << selection_path;
        QProcess::startDetached("/bin/rm", rem_file_args); // remove the selected dir/file
        update(); // update the TreeView

        Msgbox msg;
        msg.set_header(tr("INFORMATION"));
        msg.set_info("<b>" + selection_name + "</b>" + " " + tr("deleted"));
        msg.set_icon("Information");
        msg.exec();
    }
}

void Trashdialog::restore_pressed()
{
    if (tree_view->currentIndex().isValid())
    {
        QString selection_path = dir_model->filePath(tree_view->currentIndex());
        QString selection_name = dir_model->fileName(tree_view->currentIndex());
        qDebug() << "Selected file path:" << selection_path;
        QString trash_info = selection_name + ".trashinfo";
        QSettings settings(trash_path + "/Trash/info/" + trash_info, QSettings::IniFormat);
        settings.beginGroup("Trash Info");
        QString restore_full_path = settings.value("Path").toString(); // with file name at end
        settings.endGroup(); // Trash Info
        qDebug() << "Restore full path:" << restore_full_path;
        QString restore_path = restore_full_path.remove(selection_name); // remove the file name from restore path
        qDebug() << "Restore path:" << restore_path;
        QStringList restore_args;
        restore_args << selection_path << restore_path;
        QProcess::startDetached("/bin/mv", restore_args); // restore the selected dir/file
        QStringList remove_args;
        remove_args <<  trash_path + "/Trash/info/" + trash_info;
        QProcess::startDetached("/bin/rm", remove_args); // remove the info file
        update(); // update the TreeView

        Msgbox msg;
        msg.set_header(tr("INFORMATION"));
        msg.set_info("<b>" + selection_name + "</b>" + " " + tr("restored in") + " " + "<b>" + restore_path + "</b>");
        msg.set_icon("Information");
        msg.exec();
    }
}

void Trashdialog::update()
{
    tree_view->setRootIndex(dir_model->index(trash_path + "/Trash/files"));
    dir_model->refresh(dir_model->index(line_path->text())); // update the TreeView
}