#pragma once

#include <memory>
#include <qt5/QtWidgets/qwidget.h>

class QPushButton;
class QTextBrowser;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    std::unique_ptr<QPushButton>  mButton;
    std::unique_ptr<QTextBrowser> mTextBrowser;
};