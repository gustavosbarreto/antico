#include "mainpage.hpp"
#include "scrollareawidget.hpp"
#include "stylepreviewframe.hpp"

#include <QDir>
#include <QDebug>

MainPage::MainPage()
    : QWidget()
{
    ui.setupUi(this);

    ui.scrollArea->setWidget(new ScrollAreaWidget(this));

    QHBoxLayout *scrollArea = (QHBoxLayout *)ui.scrollArea->widget()->layout();

    QDir dir(QDir::homePath() + "/.antico/styles/");
    foreach (const QFileInfo &dirInfo, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QDir styleDir(dirInfo.filePath());
        if (!styleDir.exists("style.qss"))
            continue;

        QWidget *previewFrame = new StylePreviewFrame(styleDir.filePath("style.qss"), this);
        scrollArea->insertWidget(0, previewFrame);
    }
}
