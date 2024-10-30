#include "mainwindow.h"

/**
 * @brief Constructs a MainWindow, sets up navigation, pages, and connects signals.
 * @param parent The parent widget of MainWindow, default is nullptr for independent window.
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // Set window flags to disable maximize button and hide the title bar
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTitleHint);
    setAttribute(Qt::WA_TranslucentBackground); // allowing transparent background

    // Initialize QStackedWidget for page management
    stackedWidget = new QStackedWidget(this);

    // Initialize the other pages
    homePage = new HomePage(this);   // currently deprecated (not in use)
    welcomePage = new Welcome(this);
    studyWidget = new StudyWidget(this);
    gameWidget = new GameWidget(this);
    recordWidget = new RecordWidget(this);

    // Add pages to the QStackedWidget
    stackedWidget->addWidget(homePage);   // currently deprecated (not in use)
    stackedWidget->addWidget(welcomePage);
    stackedWidget->addWidget(studyWidget);
    stackedWidget->addWidget(gameWidget);
    stackedWidget->addWidget(recordWidget);

    // Set initial page to Welcome page
    stackedWidget->setCurrentWidget(welcomePage);

    // =========================Initialize the application bar on the top===============================

    // set customized close, minimize and maximize button
    btnMinimize = new QPushButton("—", this);
    btnMaximize = new QPushButton("□", this);
    btnClose = new QPushButton("X", this);

    btnMinimize->setFixedSize(35, 25);
    btnMaximize->setFixedSize(35, 25);
    btnClose->setFixedSize(35, 25);

    btnMinimize->setStyleSheet(
        "QPushButton {"
        "   border: none;"
        "   background-color: transparent;"
        "   font-size: 16px;"
        "   padding: 0;"
        "   border-radius: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 0, 0, 0.1);"
        "}"
        );

    // disabled maximize button and set its color as gray
    btnMaximize->setEnabled(false);
    btnMaximize->setStyleSheet(
        "QPushButton {"
        "   border: none;"
        "   background-color: transparent;"
        "   font-size: 14px;"
        "   padding: 0;"
        "   color: gray;"
        "}"
        );

    btnClose->setStyleSheet(
        "QPushButton {"
        "   border: none;"
        "   background-color: transparent;"
        "   font-size: 16px;"
        "   padding: 0;"
        "   border-radius: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 0, 0, 0.5);"
        "}"
        );

    // enable their functions
    connect(btnMinimize, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(btnClose, &QPushButton::clicked, this, &QWidget::close);

    // Create a QWidget for deploying the window bar
    QWidget *buttonWidget = new QWidget(this);
    buttonWidget->setStyleSheet("background-color: transparent;"
                                "border-top-right-radius: 14px;");

    // Create layout for buttons and arrange in the main layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 10, 10, 0);
    buttonLayout->setSpacing(20);
    buttonLayout->addStretch();
    buttonLayout->addWidget(btnMinimize);
    buttonLayout->addWidget(btnMaximize);
    buttonLayout->addWidget(btnClose);

    buttonWidget->setLayout(buttonLayout);

    // =========================Initialize navigation bar on the left===============================

    // -----------title-------------

    navigationBar = new QFrame(this);
    navigationBar->setFrameShape(QFrame::NoFrame);
    navigationBar->setFixedWidth(272);
    navigationBar->setStyleSheet("background-color: #F0F3F6; "
                                 "border-top-left-radius: 14px;"
                                 "border-bottom-left-radius: 14px;"
                                 "padding: 0; margin: 0;");

    // Set up navigation layout
    navLayout = new QVBoxLayout(navigationBar);
    navLayout->setContentsMargins(21, 15, 38, 0);

    // Add icon and title to navigation bar
    QWidget *titleWidget = new QWidget(this);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);

    // Set icon label with specific left margin and alignment
    QLabel *iconLabel = new QLabel(this);
    QPixmap iconPixmap(":/mainwindow/images/mainwindow/signify.png");
    iconLabel->setPixmap(iconPixmap.scaled(60, 60, Qt::KeepAspectRatio));

    // Add icon to layout with left margin
    titleLayout->addWidget(iconLabel);

    // Set spacing between icon and title
    titleLayout->addSpacing(18);

    // Configure navTitle label for "Signify" text
    QLabel *navTitle = new QLabel("Signify", this);
    navTitle->setStyleSheet("font-size: 34px; color: black;");
    navTitle->setMinimumWidth(100);

    // Add title to layout
    titleLayout->addWidget(navTitle);

    // Set layout alignment and margins
    titleLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    titleLayout->setContentsMargins(0, 0, 0, 66);
    titleWidget->setLayout(titleLayout);

    // Add title widget to navigation bar layout
    navLayout->addWidget(titleWidget);

    // -----------buttons-------------

    // Initialize Welcome buttons
    btnWelcome = new QPushButton("  Home", this);
    btnWelcome->setCheckable(true);
    btnWelcome->setStyleSheet(
        "QPushButton {"
        "background-color: transparent;"
        "color: #737B89;"
        "border-radius: 10px;"
        "padding: 10px 15px;"
        "font-size: 28px;"
        "text-align: left;"
        "}"
        "QPushButton:hover {"
        "background-color: #E4E8EC;"
        "}"
        "QPushButton:checked {"
        "background-color: #FC3D4A;"
        "color: white;"
        "}"
        );
    btnWelcome->setIcon(QIcon(":/mainwindow/images/mainwindow/welcome_icon_gray.png"));
    btnWelcome->setIconSize(QSize(30, 30));
    btnWelcome->setFixedHeight(60);
    connect(btnWelcome, &QPushButton::toggled, [=](bool checked){
        btnWelcome->setIcon(checked ? QIcon(":/mainwindow/images/mainwindow/welcome_icon_white.png") : QIcon(":/mainwindow/images/mainwindow/welcome_icon_gray.png"));
    });

    navLayout->addWidget(btnWelcome);

    // Initialize Study buttons
    btnStudy = new QPushButton("  Study", this);
    btnStudy->setCheckable(true);
    btnStudy->setStyleSheet(btnWelcome->styleSheet());
    btnStudy->setIcon(QIcon(":/mainwindow/images/mainwindow/study_icon_gray.png"));
    connect(btnStudy, &QPushButton::toggled, [=](bool checked){
        btnStudy->setIcon(checked ? QIcon(":/mainwindow/images/mainwindow/study_icon_white.png") : QIcon(":/mainwindow/images/mainwindow/study_icon_gray.png"));
    });
    btnStudy->setIconSize(btnWelcome->iconSize());
    btnStudy->setFixedHeight(btnWelcome->height());
    navLayout->addWidget(btnStudy);

    // Initialize Game buttons
    btnGame = new QPushButton("  Game", this);
    btnGame->setCheckable(true);
    btnGame->setStyleSheet(btnWelcome->styleSheet());
    btnGame->setIcon(QIcon(":/mainwindow/images/mainwindow/game_icon_gray.png"));
    connect(btnGame, &QPushButton::toggled, [=](bool checked){
        btnGame->setIcon(checked ? QIcon(":/mainwindow/images/mainwindow/game_icon_white.png") : QIcon(":/mainwindow/images/mainwindow/game_icon_gray.png"));
    });
    btnGame->setIconSize(btnWelcome->iconSize());
    btnGame->setFixedHeight(btnWelcome->height());
    navLayout->addWidget(btnGame);

    // Initialize Record buttons
    btnRecord = new QPushButton("  Record", this);
    btnRecord->setCheckable(true);
    btnRecord->setStyleSheet(btnWelcome->styleSheet());
    btnRecord->setIcon(QIcon(":/mainwindow/images/mainwindow/record_icon_gray.png"));
    connect(btnRecord, &QPushButton::toggled, [=](bool checked){
        btnRecord->setIcon(checked ? QIcon(":/mainwindow/images/mainwindow/record_icon_white.png") : QIcon(":/mainwindow/images/mainwindow/record_icon_gray.png"));
    });
    btnRecord->setIconSize(btnWelcome->iconSize());
    btnRecord->setFixedHeight(btnWelcome->height());
    navLayout->addWidget(btnRecord);

    navLayout->addStretch();

    // create a button group to carry all buttons
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);

    btnGroup->addButton(btnWelcome);
    btnGroup->addButton(btnStudy);
    btnGroup->addButton(btnGame);
    btnGroup->addButton(btnRecord);

    // default selecting welcome button
    btnWelcome->setChecked(true);


    // ------------button connections-------------

    connect(homePage, &HomePage::practiceClicked, this, &MainWindow::showGamePage);  // currently deprecated (not in use)
    connect(btnStudy, &QPushButton::clicked, this, &MainWindow::showStudyPage);
    connect(btnGame, &QPushButton::clicked, this, &MainWindow::showGamePage);
    connect(btnWelcome, &QPushButton::clicked, this, &MainWindow::showWelcomePage);
    connect(btnRecord, &QPushButton::clicked, this, &MainWindow::showRecordPage);


    // =======================Initialize the application's window format=============================

    // Create and set up main layout with navigation bar and pages
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(navigationBar);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    centralLayout->addWidget(buttonWidget);
    centralLayout->addWidget(stackedWidget);

    // Create a container "widget" as a carrier for "centralLayout" and set the background color
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color: #f7f9fc;"
                                 "border-top-right-radius: 14px;"
                                 "border-bottom-right-radius: 14px;");
    centralWidget->setLayout(centralLayout);

    mainLayout->addWidget(centralWidget);

    // apply the layout
    this->setLayout(mainLayout);


    // =====================================Other Connecting functions===========================================

    connect(welcomePage->getDataReceiver(), &DataReceiver::handSignDetected, gameWidget, &GameWidget::handSignEvent);
    connect(gameWidget->getGameWindow(), &GameWindowLabel::newHighScore, recordWidget, &RecordWidget::setTextHighScore);
    connect(studyWidget, &StudyWidget::checkedCountChanged, recordWidget, &RecordWidget::setTextLearningProgress);
    connect(welcomePage, &Welcome::processStarted, this, &MainWindow::setModelProcess);


    modelProcess = new QProcess(this);
}


/**
 * @brief Handles mouse press events to start custom window drag.
 * @param event Mouse press event.
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

/**
 * @brief Handles mouse move events to update window position during drag.
 * @param event Mouse move event.
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

/**
 * @brief Handles mouse release events to stop window drag.
 * @param event Mouse release event.
 */
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}


/**
 * @brief Centers the window on the screen on initial display.
 * @param event Show event.
 */
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    // Gets the geometry of the current screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // Compute the left and right center horizontal position of the window (x-coordinate)
    int x = (screenGeometry.width() - this->width()) / 2;

    // Align the top of the window to the top of the screen (y coordinate 0)
    int y = (screenGeometry.height() - this->height()) / 2;

    // Move the window to the calculated (x, y) position
    this->move(x, y);
}

/**
 * @brief Switches to different views in stacked widget.
 */
// currently deprecated (not in use)
void MainWindow::showHomePage()
{
    // 切换回 HomePage
    stackedWidget->setCurrentWidget(homePage);
    homePage->setFocus();
}
void MainWindow::showStudyPage()
{
    // 切换到 StudyWidget
    stackedWidget->setCurrentWidget(studyWidget);
    studyWidget->setFocus();
}
void MainWindow::showGamePage()
{
    // 切换到 GameWidget
    stackedWidget->setCurrentWidget(gameWidget);
    gameWidget->setFocus();
}
void MainWindow::showWelcomePage() {
    stackedWidget->setCurrentWidget(welcomePage);
    welcomePage->setFocus();
}
void MainWindow::showRecordPage() {
    stackedWidget->setCurrentWidget(recordWidget);
    recordWidget->setFocus();
}

/**
 * @brief Destructor for MainWindow, safely terminates external processes if running.
 */
MainWindow::~MainWindow() {
    if (modelProcess && modelProcess->state() == QProcess::Running) {
        modelProcess->kill();
        modelProcess->waitForFinished();
    }
}

/**
 * @brief Sets and monitors the external process - the gesture recognition model.
 * @param process Pointer to the QProcess instance.
 */
void MainWindow::setModelProcess(QProcess *process) {
    qDebug() << "Received process in MainWindow";
    modelProcess = process;

    if (modelProcess->state() == QProcess::Running) {
        qDebug() << "Model process is running.";
    } else {
        qDebug() << "Model process is not running.";
    }
}

/**
 * @brief Handles close events to ensure safe termination of external processes.
 * @param event Close event.
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    if (modelProcess->state() == QProcess::Running) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Program Still Running");
        msgBox.setText("The model.exe process is still running.\n"
                       "Please switch to the camera interface and press 'q' to close it "
                       "before trying to close this application again.");
        msgBox.setInformativeText("Note: The model may take up several seconds to shut down completely. \n"
                                  "If this message appears again, please be patient and retry after waiting.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        event->ignore();
    } else {
        event->accept();
    }
}




