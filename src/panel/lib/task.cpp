// Self
#include "task.h"

// Qt
#include <QX11Info>
#include <QDebug>

// X11
#include <X11/Xatom.h>
#include <X11/Xutil.h>

// System
#include <values.h>

// Local

namesp
Task::Task(Qt::HANDLE winId, QObject *parent):
    QObject(parent)
{
}
