/**
 * @class GameWidget
 * @brief A main widget class for the game interface, managing gameplay elements, layout, and controls.
 *
 * The GameWidget class provides a central interface for the game, displaying game-related visuals and controls.
 * It manages the game layout, difficulty level selection, pause/resume functionality, accuracy tracking,
 * and custom hand sign events. The widget allows players to interact with the game through sign language inputs
 * and tracks player statistics in real time.
 *
 * ### Member Variables
 * - `QGridLayout *layout` - Layout manager for arranging game components in a grid.
 * - `GameWindowLabel *lblGameWindow` - Label displaying the main game view.
 * - `QSpinBox *spinMaxLvl` - Spin box for selecting game difficulty levels.
 * - `QLabel *lblStatistics` - Label showing the player's accuracy stats.
 * - `QPushButton *backbutton` - Button for returning to the previous screen (currently deprecated).
 * - `QPushButton *pauseButton` - Button for toggling pause/resume.
 * - `QElapsedTimer inputTimer` - Timer for managing input delay in hand sign events.
 * - `bool keyPressFlag[8]` - Array tracking the state of 8 specific keys (currently deprecated).
 *
 * ### Main Methods
 * - `void keyPressEvent(QKeyEvent *ev)` - Processes keyboard press events.
 * - `void keyReleaseEvent(QKeyEvent *ev)` - Processes keyboard release events.
 * - `void updateStatistic(int rightCnt, int wrongCnt)` - Updates accuracy display based on player performance.
 * - `void handSignEvent(QString letter)` - Processes input from hand sign recognition as virtual key events.
 * - `void goBack()` - Emits a signal to navigate back to the previous screen.
 *
 * ### Signals
 * - `void backClicked()` - Signal emitted when the back button is clicked.
 *
 */

#ifndef GameWidget_H
#define GameWidget_H

#define INPUTDELAY 1000
// Gestures are detected every 1000ms to prevent repeated input
#define MAXLEVEL 10
// Max level

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QCoreApplication>
#include <QSettings>
#include <QElapsedTimer>
#include <QGraphicsDropShadowEffect>
#include "gamewindowlabel.h"


class GameWidget : public QWidget
{
    Q_OBJECT

    QGridLayout *layout;
    GameWindowLabel *lblGameWindow;  // The GameWidget is the parent object of the GameWindowLabel
    QSpinBox *spinMaxLvl;
    QLabel *lblStatistics;
    // QPushButton *backbutton;  // currently deprecated (not in use)
    QPushButton *pauseButton;
    QPushButton *resumeButton;
    QPushButton *quitButton;
    bool keyPressFlag[8];  // currently deprecated (not in use)
    QElapsedTimer inputTimer;  // preventing repeated input in a very short period of time.

    // void processSignLetterInput(QString letter);

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    int getMaxLvl(void);
    GameWindowLabel* getGameWindow();
    void updateStatistic(int rightCnt, int wrongCnt);  // Updates accuracy display in time.

protected:
    // key event: We firstly developed a typing game and then modified it into a gesture game, by mapping gestures with keys.
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;

public slots:
    // Turning handSignEvent into keyPressEvent and eventually realize the "gesture typing game".
    void handSignEvent(QString letter);

signals:
    // void backClicked();  // currently deprecated (not in use)
    void togglePause();  // emit pause signal

private slots:
    // void goBack();  // currently deprecated (not in use)
    void onGameStatusChanged(bool isStarted);  // Enable or disable the pauseButton based on the game state


};
#endif // GameWidget_H
