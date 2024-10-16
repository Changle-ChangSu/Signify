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

#include "enemyword.h"

class GameWindowLabel : public QLabel
{
    Q_OBJECT

    int rightCnt;
    int wrongCnt;
    int level; // 1 - 50
    int score; // 按键次数

    bool begin;  // 用于判断游戏是否开始

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

public:
    GameWindowLabel(QWidget *parent = 0);
    void getKey(const QString &c);  // c表示玩家的按键，即在摄像头面前比的手势对应的字母
    bool isRunning(void);
    void setRunning(bool begin);

    void generateWords(int cnt);

protected:
    void paintEvent(QPaintEvent *e) override;  // 绘图事件：用于绘制游戏中的图形界面元素


};

#endif // GAMEWINDOWLABEL_H
