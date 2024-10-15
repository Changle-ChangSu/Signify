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
    int level; // 1 - 10
    int score; // 按键次数

    bool begin;

    QSoundEffect *effect;
    QSoundEffect *effectBoom;
    QPoint firePoint;
    QPoint targetPoint;

    int gameDeadLine;

    QTimer *timerCannon;
    int cannonLen;

    QTimer *timerRunning;
    QTimer *timerStatistic;

    QList<EnemyWord *> wordList;
    EnemyWord *lockedWord;

    QStringList wordTable;

    EnemyWord * findLockedWord(const QString &mid, const QString &c);

public:
    GameWindowLabel(QWidget *parent = 0);
    void getKey(const QString &c);
    bool isRunning(void);
    void setRunning(bool begin);

    void generateWords(int cnt);

protected:
    void paintEvent(QPaintEvent *e) override;


};

#endif // GAMEWINDOWLABEL_H
