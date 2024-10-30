#include "gamewidget.h"  // 头文件
// #include "homepage.h"

/**
 * @brief Constructs a GameWidget, initializing layout, controls, and connecting signals.
 * @param parent The parent widget of GameWidget, default is nullptr for independent window.
 */
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    layout = new QGridLayout(this);

    for(int i=0; i<8; i++) {
        keyPressFlag[i] = false;
    }  // Tracking the status of up to 8 keys

    lblGameWindow = new GameWindowLabel(this);
    // Game window display. It is passed "this", indicating that its parent window is GameWidget.

    // --------------------------------------Set Level Chossing SpinBox---------------------------------------------

    spinMaxLvl = new QSpinBox(this);
    spinMaxLvl->setRange(1, MAXLEVEL);  // MAXLEVEL is defined in the header file
    spinMaxLvl->setAlignment(Qt::AlignCenter);
    spinMaxLvl->setPrefix("Choose Level: ");
    spinMaxLvl->setFixedHeight(30);
    spinMaxLvl->setFocusPolicy(Qt::NoFocus);  // Disables keyboard focus
    spinMaxLvl->setStyleSheet(
        "QSpinBox {"
        "    background-color: #f0f0f0;"
        "    border-radius: 0px;"         // Optional: Rounded corners
        "    border-left-width: 2px;"
        "    border-right-width: 1px;"
        "    border-bottom-width: 2px;"
        "    border-style: solid;"
        "}"
    );

    // Reads maximum level from config file
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    spinMaxLvl->setValue(settings.value("MaxLvl", 1).toInt());

    // Saves updated maximum level to config file when changed
    connect(spinMaxLvl, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
        QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
        settings.setValue("MaxLvl", value);
    });

    // --------------------------------------Set Accuracy Display---------------------------------------------

    lblStatistics = new QLabel("Accuracy: 0% ");
    lblStatistics->setFixedHeight(30);
    lblStatistics->setAlignment(Qt::AlignCenter);
    lblStatistics->setStyleSheet(
        "QLabel {"
        "    border-bottom-width: 2px;"
        "    border-style: solid;"
        "    border-radius: 0px;"
        "}"
    );

    // backbutton = new QPushButton("", this);
    // backbutton->setStyleSheet("background-color: transparent;""border-radius: 0px;");
    // backbutton->hide();
    // 连接返回按钮的点击信号到槽函数
    // connect(backbutton, &QPushButton::clicked, this, &GameWidget::goBack);

    // --------------------------------------Set Pause Button---------------------------------------------

    pauseButton = new QPushButton("Pause/Resume", this);
    pauseButton->setFixedHeight(30);
    // pauseButton->setAlignment(Qt::AlignCenter);
    pauseButton->setStyleSheet(
        "QPushButton {"
        "    border-right: 2px solid #000000;"
        "    border-bottom: 2px solid #000000;"
        "    border-top: 0px;"
        "    border-left: 1px solid #000000;;"
        "    padding: 5px;"
        "    border-radius: 0px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e5f1fb;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #e0e0e0;"
        "}"
    );
    connect(pauseButton, &QPushButton::clicked, this, &GameWidget::togglePause);
    connect(lblGameWindow, &GameWindowLabel::gameStatusChanged, this, &GameWidget::onGameStatusChanged);

    // Initialization
    pauseButton->setEnabled(lblGameWindow->begin);

    // -----------------------------------------------------------------------------------------------------

    // add these buttons into the widget
    layout->addWidget(lblGameWindow, 0, 0, 1, 3);
    layout->addWidget(spinMaxLvl, 1, 0);
    layout->addWidget(lblStatistics, 1, 1);
    layout->addWidget(pauseButton, 1, 2);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setFixedSize(810, 720);
    setWindowTitle("Signify");

    inputTimer.start();  // Initialize input timer and start, to prevent repeated input

}

/**
 * @brief Returns the maximum level selected in the spin box.
 * @return Current maximum level.
 */
int GameWidget::getMaxLvl(void) {
    return spinMaxLvl->value();
}

/**
 * @brief Handles key release events to track player input. (Currently not in use)
 * @param ev The released key event.
 */
void GameWidget::keyReleaseEvent(QKeyEvent *ev) {
    if(lblGameWindow->isPaused) {
        // qDebug() << ev->key() << lblGameWindow->isPaused;
        return;
    }

    if(lblGameWindow->isRunning() == false) {
        switch(ev->key()) {
        // case Qt::Key_Space:
        //     connect(pauseButton, &QPushButton::clicked, this, &GameWidget::togglePause);
        case Qt::Key_A:
            keyPressFlag[0] = false;
            break;
        case Qt::Key_S:
            keyPressFlag[1] = false;
            break;
        case Qt::Key_D:
            keyPressFlag[2] = false;
            break;
        case Qt::Key_F:
            keyPressFlag[3] = false;
            break;
        case Qt::Key_J:
            keyPressFlag[0 + 4] = false;
            break;
        case Qt::Key_K:
            keyPressFlag[1 + 4] = false;
            break;
        case Qt::Key_L:
            keyPressFlag[2 + 4] = false;
            break;
        case Qt::Key_Semicolon:
            keyPressFlag[3 + 4] = false;
            break;
        default:
            // qDebug() << ev->key();
            break;
        }
    }
}

/**
 * @brief Handles key press events to track player input and trigger game actions.
 * @param ev The pressed key event.
 */
void GameWidget::keyPressEvent(QKeyEvent *ev) {
    if(lblGameWindow->isPaused) {
        // qDebug() << ev->key() << lblGameWindow->isPaused;
        return;
    }
    if(lblGameWindow->isRunning() == false) {
        switch(ev->key()) {
        case Qt::Key_A:
            keyPressFlag[0] = true;
            break;
        case Qt::Key_S:
            keyPressFlag[1] = true;
            break;
        case Qt::Key_D:
            keyPressFlag[2] = true;
            break;
        case Qt::Key_F:
            keyPressFlag[3] = true;
            break;
        case Qt::Key_J:
            keyPressFlag[0 + 4] = true;
            break;
        case Qt::Key_K:
            keyPressFlag[1 + 4] = true;
            break;
        case Qt::Key_L:
            keyPressFlag[2 + 4] = true;
            break;
        case Qt::Key_Semicolon:
            keyPressFlag[3 + 4] = true;
            break;
        default:
            qDebug() << ev->key();
            break;
        }

        // if((keyPressFlag[0] && keyPressFlag[1] && keyPressFlag[2] && keyPressFlag[3]) ||
        //         (keyPressFlag[0+4] && keyPressFlag[1+4] && keyPressFlag[2+4] && keyPressFlag[3+4])) {
        //     lblGameWindow->setRunning(true);
        // }
    } else {
        if(ev->isAutoRepeat())
            return;
        lblGameWindow->getKey(ev->text());
        qDebug() << ev->key();
    }
}


/**
 * @brief Handles hand sign events by converting them to virtual key presses.
 * @param letter The letter identified by hand sign recognition.
 */
void GameWidget::handSignEvent(QString letter) {
    // qDebug() << "Received hand sign letter in gamewidget:" << letter;

    if (inputTimer.elapsed() < INPUTDELAY) {
        qDebug() << "Input too fast, ignoring this signal.";
        return;  // Ignores input if too soon since last input
    }
    inputTimer.restart();

    // Handling sign languge input
    if (letter.length() == 1) {
        QChar c = letter[0].toLower();

        if (c >= 'a' && c <= 'z') {
            // generate virtual keyPressEvent
            int key = Qt::Key_A + (c.unicode() - 'a');  // mapping the sign language with the keys
            QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier, letter);

            // calling keyPressEvent to process the virtual keyPressEvent
            keyPressEvent(&keyEvent);
        } else {
            qDebug() << "Unsupported letter for key event:" << letter;
        }
    }
}


/**
 * @brief Updates the accuracy statistics display based on correct and incorrect counts.
 * @param rightCnt The count of correct actions.
 * @param wrongCnt The count of incorrect actions.
 */
void GameWidget::updateStatistic(int rightCnt, int wrongCnt) {
    // qDebug() << rightCnt << wrongCnt;
    if(rightCnt == 0 && wrongCnt == 0)
    {
        lblStatistics->setText("Accuracy: 0% (0 / 0)");
    }
    else
    {
        lblStatistics->setText(QString().asprintf("Accuracy: %.1f%% ( %d / %d )", 100.0 * rightCnt / (rightCnt + wrongCnt), rightCnt, rightCnt + wrongCnt));
    }
}

// void GameWidget::goBack() // Depreciated
// {
//     // 发送返回信号
//     emit backClicked();
// }


/**
 * @brief Slot to enable or disable the pause button based on the game start status.
 * @param isStarted Boolean indicating if the game has started.
 */
void GameWidget::onGameStatusChanged(bool isStarted) {
    pauseButton->setEnabled(isStarted);  // 根据游戏状态启用或禁用 pauseButton
}

/**
 * @brief Returns a pointer to the game window label displaying the game content.
 * @return Pointer to `lblGameWindow`.
 */
GameWindowLabel* GameWidget::getGameWindow() {
    return this->lblGameWindow;
}

/**
 * @brief Destructor to clean up resources.
 */
GameWidget::~GameWidget()
{
}

