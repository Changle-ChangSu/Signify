#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include "homepage.h"
#include "gamewidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void showHomePage();      // 显示HomePage页面
    // void showStudyPage();  // 显示Study页面
    void showPracticePage();  // 显示Practice页面
    // void showPersonalRecordPage();  // 显示Personal Record页面


private:
    QStackedWidget *stackedWidget;
    HomePage *homePage;
    // Study 页面定义
    GameWidget *gameWidget;
    // Personal Record 页面定义
};

#endif // MAINWINDOW_H
