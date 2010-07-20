#include "customizedecorationwidget.hpp"
#include "fakewindow.hpp"

#include <QResizeEvent>
#include <QDebug>

static QWidget *w;

CustomizeDecorationWidget::CustomizeDecorationWidget():
    QWidget()
{
    ui.setupUi(this);

    layout()->addWidget(new FakeWindow(this));
}
