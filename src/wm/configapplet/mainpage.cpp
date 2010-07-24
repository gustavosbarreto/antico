#include "mainpage.hpp"
#include "scrollareawidget.hpp"
#include "stylepreviewframe.hpp"

#include <decorations/styleable/decorationstyleloader.hpp>

#include <QDir>
#include <QDebug>

MainPage::MainPage()
    : QWidget()
{
    ui.setupUi(this);

    ui.scrollArea->setWidget(new ScrollAreaWidget(this));

    QHBoxLayout *scrollArea = (QHBoxLayout *)ui.scrollArea->widget()->layout();

    foreach (const DecorationStyleLoader &loader, DecorationStyleLoader::styles())
    {
        QWidget *previewFrame = new StylePreviewFrame(loader.styleSheet(), this);
        scrollArea->insertWidget(0, previewFrame);
    }
}
