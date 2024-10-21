#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QFrame>
#include <QButtonGroup>
#include "homepage.h"
#include "gamewidget.h"
#include "studywidget.h"
#include "welcome.h"  // 确保包含 Welcome 的头文件
#include "recordwidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    QFrame *navigationBar;
    QVBoxLayout *navLayout;
    QHBoxLayout *mainLayout;

public:
    MainWindow(QWidget *parent = nullptr);

signals:
    void btnWelcomeClicked(); // 添加 Welcome 按钮的信号
    void btnStudyClicked();
    void btnGameClicked();
    void btnRecordClicked();

private slots:
    void showWelcomePage();   // 显示Welcome页面
    void showHomePage();      // 显示HomePage页面（即将弃用）
    void showStudyPage();  // 显示Study页面
    void showGamePage();  // 显示Practice页面
    void showRecordPage();  // 显示Record页面


private:
    QStackedWidget *stackedWidget;
    void showEvent(QShowEvent *event);  // 控制页面的默认位置居中

    Welcome *welcomePage; // 添加 Welcome 页面指针
    HomePage *homePage;
    StudyWidget *studyWidget;
    GameWidget *gameWidget;
    RecordWidget *recordWidget;

    QPushButton *btnWelcome; // 添加 Welcome 按钮
    QPushButton *btnHome;  // 测试用，将弃用
    QPushButton *btnStudy;
    QPushButton *btnGame;
    QPushButton *btnRecord;

};

#endif // MAINWINDOW_H
