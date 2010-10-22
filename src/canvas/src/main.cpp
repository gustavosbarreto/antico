#include <QApplication>

#include "windowmanager.hpp"

static QCoreApplication::EventFilter oldEventFilter = NULL;

bool x11EventFilter(void *message, long *result)
{
    XEvent *event = reinterpret_cast<XEvent *>(message);
    if (WindowManager::self()->x11EventFilter(event))
        return true;

    if (oldEventFilter && oldEventFilter(message, result))
        return true;

    return false;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    oldEventFilter = app.setEventFilter(x11EventFilter);

    WindowManager::self()->init();

    return app.exec();
}
