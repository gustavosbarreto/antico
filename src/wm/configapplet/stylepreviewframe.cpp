#include "stylepreviewframe.hpp"
#include "fakewindow.hpp"
#include "decoration.hpp"

#include <QFile>
#include <QResizeEvent>

StylePreviewFrame::StylePreviewFrame(const QString &fileName, QWidget *parent)
    : QFrame(parent)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);

    _window = new FakeWindow(this);
    _window->decoration()->setStyleSheet(file.readAll());
    _window->show();

    setStyleSheet("StylePreviewFrame { border-radius: 4; background-color: rgba(255, 255, 255, 100); }");

    setFixedHeight(156);
}

void StylePreviewFrame::resizeEvent(QResizeEvent *e)
{
    int windowWidth = e->size().width() / 2;

    _window->move((e->size().width() - windowWidth) / 2, 8);
    _window->resize(windowWidth, 140);
}
