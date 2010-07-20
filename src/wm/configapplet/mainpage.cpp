#include "mainpage.hpp"
#include "scrollareawidget.hpp"
#include "stylepreviewframe.hpp"

MainPage::MainPage()
    : QWidget()
{
    ui.setupUi(this);

    ui.scrollArea->setWidget(new ScrollAreaWidget(this));

    ((QHBoxLayout *)ui.scrollArea->widget()->layout())->insertWidget(0, new StylePreviewFrame(this));
    ((QHBoxLayout *)ui.scrollArea->widget()->layout())->insertWidget(0, new StylePreviewFrame(this));
}
