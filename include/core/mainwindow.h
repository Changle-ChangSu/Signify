/**
 * @class MainWindow
 * @brief The main application window providing navigation and managing various pages.
 *
 * The MainWindow class is the main application window that hosts different pages using a QStackedWidget.
 * It includes a left navigation bar for switching between pages, title bar controls, and functions
 * to handle window events, such as custom drag functionality, centering, and tracking external processes.
 *
 * ### Key Components
 * - `QStackedWidget *stackedWidget` - Manages and displays multiple pages within the main window.
 * - `HomePage *homePage` - Represents the home page of the application.
 * - `Welcome *welcomePage` - Represents the welcome page.
 * - `StudyWidget *studyWidget` - The study page widget.
 * - `GameWidget *gameWidget` - Game page widget for gameplay.
 * - `RecordWidget *recordWidget` - Page for displaying records and progress.
 * - `QFrame *navigationBar` - Navigation bar with buttons to switch between pages.
 * - `QPushButton *btnMinimize`, `*btnMaximize`, `*btnClose` - Title bar buttons for minimizing, maximizing, and closing the window.
 * - `QProcess *modelProcess` - External process management, such as running background tasks or models.
 *
 * ### Main Methods
 * - `void showHomePage()`, `void showStudyPage()`, `void showGamePage()` - Methods to switch to different pages.
 * - `void setModelProcess(QProcess *process)` - Sets and manages an external process.
 *
 * ### Signals and Events
 * - `void closeEvent(QCloseEvent *event)` - Manages safe closing of the window, ensuring external processes are terminated.
 * - `void mousePressEvent(QMouseEvent *event)`, `void mouseMoveEvent(QMouseEvent *event)`, `void mouseReleaseEvent(QMouseEvent *event)` - Custom window drag support.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QFrame>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include "homepage.h"
#include "gamewidget.h"
#include "studywidget.h"
#include "welcome.h"
#include "recordwidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    QFrame *navigationBar;
    QVBoxLayout *navLayout;
    QHBoxLayout *mainLayout;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void btnWelcomeClicked();
    void btnStudyClicked();
    void btnGameClicked();
    void btnRecordClicked();

private slots:
    void showWelcomePage();
    void showHomePage();      // currently deprecated (not in use)
    void showStudyPage();
    void showGamePage();
    void showRecordPage();

public slots:
    void setModelProcess(QProcess *process);  // handling external process


private:
    QStackedWidget *stackedWidget;
    void showEvent(QShowEvent *event) override;  // centralize the window

    Welcome *welcomePage;
    HomePage *homePage;
    StudyWidget *studyWidget;
    GameWidget *gameWidget;
    RecordWidget *recordWidget;

    QPushButton *btnWelcome;
    QPushButton *btnHome;  // currently deprecated (not in use)
    QPushButton *btnStudy;
    QPushButton *btnGame;
    QPushButton *btnRecord;

    // Custom window bar
    QPushButton *btnMinimize;
    QPushButton *btnMaximize;
    QPushButton *btnClose;

    // Custom window drag support
    bool dragging;
    QPoint dragPosition;

    QProcess *modelProcess;  // The QProcess for storaging model.exe

protected:
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif // MAINWINDOW_H
