#ifndef GAMEWINDOWLABEL_H
#define GAMEWINDOWLABEL_H

#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QSoundEffect>
#include <QTimer>
#include <QList>
#include <QTime>
#include <QCoreApplication>
#include <QSettings>
#include <QMessageBox>

// #include "gamewidget.h"
#include "enemyword.h"

#define GAMESPEED 50
#define INTERVAL 1000

class GameWindowLabel : public QLabel
{
    Q_OBJECT

    int rightCnt;
    int wrongCnt;
    int level; // 1 - 50
    int score; // 按键次数

    QSoundEffect *effect;
    QSoundEffect *effectBoom;
    QPoint firePoint;
    QPoint targetPoint;

    int gameDeadLine;

    QTimer *timerCannon;
    int cannonLen;

    QTimer *timerRunning;  // 控制单词方块生成和时间步进
    QTimer *timerStatistic;

    QList<EnemyWord *> wordList;
    EnemyWord *lockedWord;

    QStringList wordTable;

    EnemyWord * findLockedWord(const QString &mid, const QString &c);

    QRect logoRect;
    QRect quitRect;

    bool showPauseMessage;      // 控制“Game Paused”的显示状态
    bool showStudyMessage;      // 控制“Feel free to go to the study page”的显示状态
    QTimer *blinkTimer;         // 定时器，用于控制闪烁

public:
    GameWindowLabel(QWidget *parent = 0);  // GameWindowLabel是GameWidget的子对象

    bool begin;  // 用于判断游戏是否开始
    bool isPaused;  // 用于判断游戏是否暂停

    void getKey(const QString &c);  // c表示玩家的按键，即在摄像头面前比的手势对应的字母
    bool isRunning(void);
    void setRunning(bool begin);  // 控制游戏运行与结束
    void setPaused(bool isPaused);  // 控制游戏暂停与继续

    void generateWords(int cnt);
    void pauseGame();

    void showQuitConfirmation();

public slots:
    void onTogglePause();  // 用于处理暂停逻辑的槽函数
    void toggleMessages();  // 定时切换提示信息显示状态

protected:
    void paintEvent(QPaintEvent *e) override;  // 绘图事件：用于绘制游戏中的图形界面元素
    void mousePressEvent(QMouseEvent *event) override;  // 重写鼠标点击事件
    void setFontDefault();

signals:
    void gameStatusChanged(bool isStarted);

};

#endif // GAMEWINDOWLABEL_H
