#include <QtWidgets>
#include "main_widget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    mButton.reset(new QPushButton(tr("Push me!")));
    mTextBrowser.reset(new QTextBrowser());

    std::unique_ptr<QGridLayout> mainLayout(new QGridLayout);
    mainLayout->addWidget(mButton.get(), 0, 0);
    mainLayout->addWidget(mTextBrowser.get(), 1, 0);
    setLayout(mainLayout.release());
    setWindowTitle(tr("Connecting buttons to process."));
}

MainWidget::~MainWidget()
{
}