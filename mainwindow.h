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

class MainWindow : public QWidget
{
    Q_OBJECT

    QFrame *navigationBar;
    QVBoxLayout *navLayout;
    QHBoxLayout *mainLayout;

public:
    MainWindow(QWidget *parent = nullptr);

signals:
    void btnStudyClicked();
    void btnGameClicked();
    void btnRecordClicked();

private slots:
    void showHomePage();      // 显示HomePage页面（即将弃用）
    void showStudyPage();  // 显示Study页面
    void showGamePage();  // 显示Practice页面
    // void showPersonalRecordPage();  // 显示Personal Record页面


private:
    QStackedWidget *stackedWidget;
    void showEvent(QShowEvent *event);  // 控制页面的默认位置居中

    HomePage *homePage;
    StudyWidget *studyWidget;
    GameWidget *gameWidget;
    // Personal Record 页面定义
    QPushButton *btnHome;  // 测试用，将弃用
    QPushButton *btnStudy;
    QPushButton *btnGame;
    QPushButton *btnRecord;

};

#endif // MAINWINDOW_H
