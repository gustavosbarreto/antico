#include <QApplication>

#include "mainpage.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainPage *w = new MainPage;
    w->show();

    return app.exec();
}
