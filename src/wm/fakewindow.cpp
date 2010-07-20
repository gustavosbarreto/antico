#include "fakewindow.hpp"
#include "decorations/styleable/styleabledecoration.hpp"

#include <QHBoxLayout>

FakeWindow::FakeWindow(QWidget *parent)
    : QWidget(parent)
{
    _decoration = new StyleableDecoration(NULL, this);

    BorderSize borderSize = _decoration->borderSize();

    _centralWidget = new QWidget(_decoration);
    _centralWidget->setStyleSheet("background-color: palette(window);");
    _centralWidget->move(borderSize.left(), borderSize.top() + borderSize.titleBarHeight());
    _centralWidget->show();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(_decoration);
    setLayout(layout);
}

void FakeWindow::resizeEvent(QResizeEvent *e)
{
    BorderSize borderSize = _decoration->borderSize();

    _centralWidget->resize(size().width() - borderSize.measuredWidth(),
                           size().height() - borderSize.measuredHeight());
}
