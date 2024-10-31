/**
 * @class GameWindowLabel
 * @brief Handles the main gameplay display and logic for the "Signify Strike" game.
 *
 * The GameWindowLabel class is a custom QLabel used to implement the main game mechanics, display, and interactions for the game.
 * This class manages the visual elements on the screen, including falling word/letter "enemies," the player's cannon, and the score and level display.
 * It also handles gameplay control, including game start, pause, and quit functionality, as well as generating and tracking enemy words.
 *
 * Key Features:
 * - **Gameplay Control**: Manages game start, pause, and quit functions. The game state can be toggled between running and paused modes.
 * - **Enemy Word Generation**: Generates a list of words (or letters) that fall towards a "deadline." If a word reaches the deadline, the game ends.
 * - **Cannon Firing Mechanism**: When the user inputs a correct gesture, the cannon fires at the targeted word, gradually destroying it letter by letter.
 * - **Score and Level Tracking**: Keeps track of the player's score and level, displaying them on the screen. The score updates as words are destroyed.
 * - **Sound Effects**: Includes sound effects for cannon firing and word destruction.
 * - **Visual Effects**: Displays instructions, game information, and blinking effects during pauses or at game start.
 *
 * The class also emits signals to notify the GameWidget parent of game status changes or high score achievements.
 *
 * Dependencies:
 * - **EnemyWord**: Represents individual enemy words that descend on the screen.
 * - **GameWidget**: The main parent widget containing the GameWindowLabel. Signals from GameWindowLabel can trigger actions in GameWidget.
 * - **QSoundEffect**: Manages sound effects.
 * - **QTimer**: Controls various timing elements like game speed, word generation, and animation effects.
 *
 * Usage:
 * The GameWindowLabel is initialized and displayed within a GameWidget. To start the game, users interact with the logo or click "Start."
 * Words will begin falling, and players must make sign gestures to destroy them. If a word reaches the "gameDeadLine," the game ends.
 * Users can pause the game, which will display pause messages. A click on the quit button in pause mode will bring up a confirmation dialog.
 *
 * Signals:
 * - `gameStatusChanged(bool isRunning)`: Emitted when the game starts or stops.
 * - `newHighScore()`: Emitted if the player achieves a new high score.
 *
 */

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
#include <QFile>
#include <random>
#include <QFontDatabase>
#include <QApplication>
#include <QProcess>

// #include "gamewidget.h"
#include "enemyword.h"

#define GAMESPEED 50
// update the page every 50ms
#define INTERVAL 1000
// try to shoot a projectile every 1000ms, in order to prevent repeated input in a short time

class GameWindowLabel : public QLabel
{
    Q_OBJECT

    int rightCnt;
    int wrongCnt;
    int level; // 1 - 10
    int score;

    QSoundEffect *effect;
    QSoundEffect *effectBoom;
    QPoint firePoint;
    QPoint targetPoint;

    int gameDeadLine;

    QTimer *timerCannon;
    int cannonLen;

    QTimer *timerRunning;  // the main timer
    QTimer *timerStatistic;

    QList<EnemyWord *> wordList;
    EnemyWord *lockedWord;

    QStringList wordTable;

    EnemyWord * findLockedWord(const QString &mid, const QString &c);

    QRect logoRect;
    QRect quitRect;

    bool showPauseMessage;
    bool showStudyMessage;
    bool isHovered = false; // To track if the mouse is hovering over the logo

    QTimer *blinkTimer;

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

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void gameStatusChanged(bool isStarted);
    void newHighScore();

};

#endif // GAMEWINDOWLABEL_H
