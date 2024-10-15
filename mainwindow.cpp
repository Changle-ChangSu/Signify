#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 创建 QStackedWidget
    stackedWidget = new QStackedWidget(this);

    // 创建 HomePage 和 GameWidget
    homePage = new HomePage(this);
    gameWidget = new GameWidget(this); // GameWidget 现在不需要传递HomePage

    // 将 HomePage 和 GameWidget 添加到 QStackedWidget 中
    stackedWidget->addWidget(homePage);   // 页面 0
    stackedWidget->addWidget(gameWidget); // 页面 1

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    // 设置初始显示为 HomePage
    stackedWidget->setCurrentWidget(homePage);

    // 练习模式相关按钮的连接：
    // 连接 HomePage 的 Practice 按钮点击信号到切换页面的槽函数
    connect(homePage, &HomePage::practiceClicked, this, &MainWindow::showPracticePage);
    // 连接 GameWidget 的返回按钮信号到切换回HomePage的槽函数
    connect(gameWidget, &GameWidget::backClicked, this, &MainWindow::showHomePage);
}

void MainWindow::showPracticePage()
{
    // 切换到 GameWidget
    stackedWidget->setCurrentWidget(gameWidget);
}

void MainWindow::showHomePage()
{
    // 切换回 HomePage
    stackedWidget->setCurrentWidget(homePage);
}
