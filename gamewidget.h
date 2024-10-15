#ifndef GameWidget_H
#define GameWidget_H  // 防止重复定义

#include <QWidget>  // 提供了创建窗口和基本用户界面元素的功能
#include <QLayout>  // 用于管理子控件的布局
#include <QPushButton>  // 提供按钮的功能
#include <QSpinBox>  // 用于创建一个数字选择器（增减框）
#include <QCoreApplication>  // 用于管理应用程序的主事件循环和应用程序的全局控制
#include <QSettings>  // 用于保存和读取应用程序设置

#include "gamewindowlabel.h"  // 与游戏窗口显示相关的控件
// # include "gamewidget.h"


class GameWidget : public QWidget
{
    Q_OBJECT

    QGridLayout *layout;  // 用于管理窗口中控件的布局，QGridLayout 允许将控件排列在网格中
    GameWindowLabel *lblGameWindow;  // 用于显示游戏内容的区域
    QSpinBox *spinMaxLvl;  // 用于用户选择游戏的最大关卡数的控件（左下角的等级选择）
    QLabel *lblStatistics;  // 用于显示游戏统计信息的标签（右下角的统计信息展示）
    QPushButton *backbutton;  // 用于返回到主界面

    // 用于存储键盘按键的状态。这个数组可能用于检测和处理多按键的输入，例如某些游戏中会同时检测多个按键的按下与松开
    bool keyPressFlag[8];

public:
    // 类的构造函数，用于初始化窗口以及所有的子控件。parent 参数默认是 nullptr，表示这个控件是一个独立的窗口
    GameWidget(QWidget *parent = nullptr);
    // 这是析构函数，用于清理资源，当 GameWidget 对象被销毁时自动调用。
    ~GameWidget();

    int getMaxLvl(void);  // 返回 spinMaxLvl 控件的当前值
    void updateStatistic(int rightCnt, int wrongCnt);  // 实时更新 lblStatistics 控件的内容，根据参数 rightCnt（正确次数）和 wrongCnt（错误次数）来显示统计信息

protected:
    // 重载了 QWidget 的 keyPressEvent 方法，用于处理键盘按下事件。这个函数会在用户按下某个键时被调用，ev 是包含按键信息的事件对象。
    // 处理一次按下多个按键的情况
    void keyPressEvent(QKeyEvent *ev) override;
    // 重载了 QWidget 的 keyReleaseEvent 方法，用于处理键盘释放事件。当用户松开某个键时，这个函数会被调用。
    // 处理一次释放多个按键的情况
    void keyReleaseEvent(QKeyEvent *ev) override;

signals:
    void backClicked();  // 返回按钮点击信号

private slots:
    void goBack();  // 返回槽函数

};
#endif // GameWidget_H
