// Qt
#include <QApplication>

// Own
#include "panel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    new Panel;

    return app.exec();
}

