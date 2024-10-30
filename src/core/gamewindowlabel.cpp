#include "gamewindowlabel.h"
#include "gamewidget.h"


/**
 * @brief Constructor for GameWindowLabel class, initializing game parameters, UI settings, and timers.
 * @param parent The parent widget of GameWindowLabel, typically a GameWidget instance.
 */
GameWindowLabel::GameWindowLabel(QWidget *parent) : QLabel(parent)
{

    level = 1;
    score = 0;
    rightCnt = wrongCnt = 0;

    cannonLen = 220;
    lockedWord = NULL;
    begin = false;
    // emit gameStatusChanged(begin);
    isPaused = false;
    showPauseMessage = true;
    showStudyMessage = true;

    // controlling the blink effect of pause notices
    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, &GameWindowLabel::toggleMessages);

    blinkTimer->setInterval(1000);

    // ====================================Generating dropping words============================================

    // // 使用随机数生成器生成 500 个随机数并插入 wordTable
    // std::random_device rd;  // 用于生成随机种子
    // std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
    // std::uniform_int_distribution<> distrib(1, 99999); // 设置随机数范围，假设范围是 1 到 99999

    // for(int i=0; i<500; i++) {
    //     int randomNumber = distrib(gen);
    //     wordTable.append(QString::number(randomNumber));
    // }

    // // 随机读取 1000 个单词并插入 wordTable
    // QFile file(QCoreApplication::applicationDirPath() + "/words.txt");
    // QStringList allWords;  // 存储从文件中读取的所有单词

    // // 1. 读取文件并存储所有单词到 allWords 列表
    // if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QTextStream in(&file);

    //     while (!in.atEnd()) {
    //         QString line = in.readLine().trimmed();  // 去除可能的空格或换行符
    //         if (!line.isEmpty()) {
    //             allWords.append(line);  // 将每行内容（即单词）加入 allWords 列表
    //         }
    //     }
    //     file.close();
    // } else {
    //     qDebug() << "open words.txt failed!";
    // }

    // // 2. 检查文件中是否有足够的单词
    // if (allWords.size() < 1000) {
    //     qDebug() << "Not enough words in the file!";
    // } else {
    //     // 3. 使用随机数生成器随机选择 1000 个单词
    //     std::random_device rd;  // 用于生成随机种子
    //     std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
    //     std::uniform_int_distribution<> distrib(0, allWords.size() - 1);  // 生成 0 到 allWords.size()-1 之间的随机数

    //     QSet<int> selectedIndices;  // 用于存储已经选过的索引，防止重复选择

    //     while (selectedIndices.size() < 1000) {
    //         int randomIndex = distrib(gen);  // 生成随机索引
    //         if (!selectedIndices.contains(randomIndex)) {
    //             selectedIndices.insert(randomIndex);  // 记录选中的索引，防止重复
    //             wordTable.append(allWords[randomIndex]);  // 随机选择单词并插入 wordTable
    //         }
    //     }

    //     // qDebug() << "Successfully added 1000 random words to wordTable!";
    // }

    // ================================Generating Dropping Letters==============================================

    // Randomly generate letters and add into wordTable
    std::random_device rd;
    std::mt19937 gen(rd());

    // Z is expelled from the letter list because of its particularity.
    std::uniform_int_distribution<> distrib(0, 24);
    const QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXY";
    for(int i = 0; i < 25; i++) {
        int randomIndex = distrib(gen);
        wordTable.append(alphabet[randomIndex]);
    }

    // ===========================================================================================================

    effect = new QSoundEffect(this);
    effect->setSource(QUrl("qrc:/effect/sounds/eject.wav"));
    effect->setLoopCount(1);
    effect->setVolume(0.2f);

    effectBoom = new QSoundEffect(this);
    effectBoom->setSource(QUrl("qrc:/effect/sounds/wordBoom.wav"));
    effectBoom->setLoopCount(1);
    effectBoom->setVolume(30.0f);

    setStyleSheet("background-color:#f7f9fc");

    // Simulate the recoil of a cannon
    timerCannon = new QTimer(this);
    timerCannon->setSingleShot(true);  // This parameter is set to trigger once.
    connect(timerCannon, &QTimer::timeout, this, [=](){
        cannonLen = 220;
        update();
    });  // When the timer expires, reset the barrel length to 220 and call the update() function to refresh the window. This is used to handle the firing animation and the bullet firing process.


    // ====================================Set the game main timer timerRunning==========================================

    timerRunning = new QTimer(this);  // Used to handle enemy (word) movement and status updates

    // Connect the GameWidget's togglePause signal to the GameWindowLabel's onTogglePause slot function
    connect((GameWidget *)parent, &GameWidget::togglePause, this, &GameWindowLabel::onTogglePause);

    connect(timerRunning, &QTimer::timeout, this, [=](){
        if(begin == false || isPaused == true) {
            return;
        }

        // word drop a pixel forward
        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            word->setPos(QPoint(word->getPos().x(), word->getPos().y() + 1));
            word->ballsApproach();
        }

        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            if(word->getDestoryed() == true && word->getBallList()->size() == 0) {
                wordList.removeOne(word);
                score += word->getWord().size() * level;  // Score adding rules: score += word size * grade
                delete word;
                word = NULL;
                effectBoom->play();
                continue;
            }

            // If the word reaches the deadline, the game fails.
            if(word->getDestoryed() == false && word->getPos().y() > gameDeadLine) {
                this->setRunning(false);
            }
        }

        update();
    });

    timerRunning->start(GAMESPEED);


    // ===================================Set the word timer timerStatistic========================================

    timerStatistic = new QTimer(this);
    connect(timerStatistic, &QTimer::timeout, this, [=](){
        if(begin == false)
            return;

        level = ((GameWidget *)parent)->getMaxLvl();
        generateWords(level);

    });
    timerStatistic->start(INTERVAL);  // generate a set of word/letter per INTERVAL
}

// ============================================Paint the cannon===================================================


/**
 * @brief Custom paint event for rendering the game interface and visual elements.
 * @param e The paint event that triggers this function.
 */
void GameWindowLabel::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter painter(this);

    QRect rect = this->rect();

    QRect scoreRect;
    QRect levelRect;

    levelRect.setTop(rect.bottom() - 80);
    levelRect.setBottom(rect.bottom() - 15);
    levelRect.setLeft(20);
    levelRect.setRight(20 + rect.width() / 4);

    scoreRect.setTop(rect.bottom() - 80);
    scoreRect.setBottom(rect.bottom() - 15);
    scoreRect.setLeft(rect.right() - 20 - rect.width() / 4);
    scoreRect.setRight(rect.right() - 20);

    QRect fireRect;

    fireRect.setTop(rect.bottom() - 80);
    fireRect.setBottom(rect.bottom() + 200);
    fireRect.setLeft(rect.width() / 2 - 140);
    fireRect.setRight(rect.width() / 2 + 140);
    firePoint = fireRect.center();

    // painter.drawRect(fireRect);

    QPen pen = painter.pen();
    pen.setWidth(4);
    painter.setPen(pen);

    gameDeadLine = fireRect.top() - 20;
    painter.setBrush(Qt::lightGray);
    painter.drawRect(0, gameDeadLine, rect.width(), 500);
    painter.drawLine(0, gameDeadLine, rect.right(), gameDeadLine);

    pen.setWidth(2);
    painter.setPen(pen);

    // painter.drawLine(firePoint, targetPoint);

    painter.save();
    QFont font = painter.font();
    font.setFamily("Courier New");
    font.setPixelSize(30);
    font.setBold(true);
    painter.setFont(font);

    painter.drawRoundedRect(levelRect, 20, 20);
    painter.drawRoundedRect(scoreRect, 20, 20);
    painter.drawText(levelRect, Qt::AlignCenter, "Level:" + QString::number(level));
    painter.drawText(scoreRect, Qt::AlignCenter, "Score:" + QString::number(score));

    font.setPixelSize(40);
    painter.setFont(font);

    for(int i=0; i<wordList.size(); i++) {
        EnemyWord *word = wordList[i];

        QList<CannonBall *> *ballList = word->getBallList();
        for(int j=0; j<ballList->size(); j++) {
            QRect r;
            r.setTop((*ballList)[j]->getCurPos().y() - 13);
            r.setBottom((*ballList)[j]->getCurPos().y() + 13);
            r.setLeft((*ballList)[j]->getCurPos().x() - 13);
            r.setRight((*ballList)[j]->getCurPos().x() + 13);
            painter.drawEllipse(r);
        }

    }

    pen.setColor(Qt::red);
    painter.setPen(pen);

    for(int i=0; i<wordList.size(); i++) {
        EnemyWord *word = wordList[i];
        QRect wordRect;

        if(word->getDestoryed() == false) {
            continue;
        }

        wordRect.setTop(word->getPos().y() - word->getHalfH());
        wordRect.setBottom(word->getPos().y() + word->getHalfH());
        wordRect.setLeft(word->getPos().x() - word->getHalfW());
        wordRect.setRight(word->getPos().x() + word->getHalfW());
        painter.drawRoundedRect(wordRect, 10, 10);
        QString wordStr = word->getWord();
        painter.drawText(wordRect, Qt::AlignCenter, wordStr);
    }
    pen.setColor(Qt::black);
    painter.setPen(pen);

    for(int i=0; i<wordList.size(); i++) {
        EnemyWord *word = wordList[i];
        QRect wordRect;

        if(word->getDestoryed() == true) {
            continue;
        }

        wordRect.setTop(word->getPos().y() - word->getHalfH());
        wordRect.setBottom(word->getPos().y() + word->getHalfH());
        wordRect.setLeft(word->getPos().x() - word->getHalfW());
        wordRect.setRight(word->getPos().x() + word->getHalfW());
        painter.drawRoundedRect(wordRect, 10, 10);
        QString wordStr = word->getWord();
        painter.drawText(wordRect, Qt::AlignCenter, wordStr);
        if(word == lockedWord) {
            int curIndex = word->getCurIndex();
            if(curIndex) {
                pen.setColor(Qt::red);
                painter.setPen(pen);
                QString s = wordStr.mid(0, curIndex);
                for(int j=curIndex; j<wordStr.size(); j++) {
                    s += " ";
                }
                painter.drawText(wordRect, Qt::AlignCenter, s);
                pen.setColor(Qt::black);
                painter.setPen(pen);
            }
        }
    }
    painter.restore();

    double angle = atan(1.0 * (targetPoint.x() - firePoint.x()) / (firePoint.y() - targetPoint.y()));

    painter.save();
    painter.translate(firePoint);
    painter.rotate(angle * 180 / 3.1415926535);
    QRect rectCannon;
    rectCannon.setTop(0 - cannonLen);
    rectCannon.setBottom(0 + 220);
    rectCannon.setLeft(0 - 18);
    rectCannon.setRight(0 + 18);
    painter.drawRoundedRect(rectCannon, 8, 8);
    painter.restore();

    //painter.setBrush(Qt::black);
    painter.drawEllipse(fireRect);


    // ========================================Paint the Game's Cover===============================================

    if (this->begin == false) {
        setFontDefault();

        // "CodeAllNight Group Present"
        QFont smallFont = font;
        smallFont.setPixelSize(16);
        painter.setFont(smallFont);

        QFontMetrics smallMetrics(smallFont);
        int smallTextHeight = smallMetrics.height();

        QRect smallTextRect(rect.left(), rect.top() + 20, rect.width(), smallTextHeight);
        painter.drawText(smallTextRect, Qt::AlignTop | Qt::AlignHCenter, "CodeAllNight Group Present");

        // "Signify Strike"
        QFont boldFont = font;
        boldFont.setPixelSize(50);
        boldFont.setBold(true);
        painter.setFont(boldFont);

        QFontMetrics boldMetrics(boldFont);
        int boldTextHeight = boldMetrics.height();

        QPen penShadow(Qt::gray);
        painter.setPen(penShadow);

        QRect boldTextShadowRect(rect.left(), rect.top() + smallTextHeight + 20, rect.width(), boldTextHeight);
        painter.drawText(boldTextShadowRect.adjusted(2, 2, 2, 2), Qt::AlignHCenter, "Signify Strike");

        QPen penMain(Qt::black);
        painter.setPen(penMain);
        painter.drawText(boldTextShadowRect, Qt::AlignHCenter, "Signify Strike");
        painter.setRenderHint(QPainter::Antialiasing, true);

        // "Start Button:"
        logoRect = QRect(rect.left(), rect.top() + smallTextHeight + boldTextHeight + 70, rect.width(), 60);
        QPixmap pixmap(isHovered ? ":/game/images/game/GameLogo2.png" : ":/game/images/game/GameLogo2.png");
        QPixmap scaledPixmap = pixmap.scaled(logoRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int xOffset = (logoRect.width() - scaledPixmap.width()) / 2;
        int yOffset = (logoRect.height() - scaledPixmap.height()) / 2;
        painter.drawPixmap(logoRect.left() + xOffset, logoRect.top() + yOffset, scaledPixmap);

        // "Game Instruction:"
        QFont instructionTitleFont = font;
        instructionTitleFont.setPixelSize(20);
        instructionTitleFont.setBold(true);
        painter.setFont(instructionTitleFont);

        QRect instructionTitleRect(rect.left()+20, rect.top() + smallTextHeight + boldTextHeight + 170, rect.width()-20, 50);
        painter.drawText(instructionTitleRect, Qt::AlignCenter | Qt::AlignTop, "Game Instruction");

        // Restore the font style to normal description text
        QFont instructionFont = font;
        instructionFont.setPixelSize(15);
        instructionFont.setBold(false);
        painter.setFont(instructionFont);

        // Paint caption
        QRect instructionTextRect(rect.left()+20, rect.top() + smallTextHeight + boldTextHeight + 210, rect.width()-40, 290);
        painter.drawText(instructionTextRect, Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap,
                         "> In this game, you are manipulating a canon.\n"
                         "> In order to eliminate the coming enemy word bricks, you need to gesture sign languages in front of the camera.\n"
                         "> If your gesture is right, the cannon will fire and shoot down the words letter by letter.\n"
                         "> You will win scores according to the length of the word you shoot down and the level you choose.\n"
                         "> Your best performance will be seen in the Record Page.\n"
                         "> You can choose to shoot all words appear on your sight, no order requirement.\n"
                         "> But to a single word, wrong sequence or wrong gesture will empty your accumulated blow to this word.\n"
                         "> Feel free to click the pause button in the bottom right corner and go to the Study page to learn when playing.\n"
                         );
    }


    // =======================================Paint pause prompt page====================================================

    if (isPaused == true) {
        quitRect = QRect(10, gameDeadLine - 60, 200, 60);

        QFont font = painter.font();
        font.setPixelSize(20);
        painter.setFont(font);

        // “Game Paused Sign”
        if (showPauseMessage) {
            painter.setPen(Qt::red);
            painter.drawText(quitRect, Qt::AlignLeft, "Game Paused\n※ Click here to QUIT");
        }

        // “Feel free to go to the study page”
        if (showStudyMessage) {
            painter.setPen(Qt::blue);
            painter.drawText(QRect(width() - 330, gameDeadLine - 35, 320, 30), Qt::AlignRight, "Feel free to go to the study page!");
        }
    }

}

/**
 * @brief Checks if the game is currently running.
 * @return A boolean indicating whether the game is in progress (true) or not (false).
 */
bool GameWindowLabel::isRunning(void) {
    return begin;
}

/**
 * @brief Starts or stops the game and handles game initialization and cleanup.
 * @param begin A boolean value indicating whether to start (true) or stop (false) the game.
 */
void GameWindowLabel::setRunning(bool begin) {
    this->setFocus();
    this->begin = begin;
    emit gameStatusChanged(begin);

    // If the game is swiched to begin (begin = true)
    if(this->begin) {
        level = 1;
        score = 0;
        rightCnt = wrongCnt = 0;

        timerRunning->start(GAMESPEED);  // start the game timer
        timerStatistic->start(INTERVAL);
        blinkTimer->stop();  // stop the pause game sign blinker

        cannonLen = 220;
        lockedWord = NULL;

        generateWords(level);
    } else {  // If the game is swiched to stop (begin = false)
        // ---------------------save the higest score (if achieved)-------------------------
        // read the original higest score from the config file
        QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
        int savedHighScore = settings.value("HighScore", 0).toInt();
        // compare it with the current score.
        if (score > savedHighScore) {
            settings.setValue("HighScore", score);
            emit newHighScore();
        }

        // reset all the variables
        this->score = 0;
        this->rightCnt = this->wrongCnt = 0;

        this->lockedWord = NULL;
        this->setPaused(false);

        foreach(EnemyWord *w, wordList)
        {
            if(w)
            {
                wordList.removeOne(w);
                delete w;
                w = nullptr;
            }
        }

    }
}

/**
 * @brief Sets the pause status of the game.
 * @param isPaused A boolean indicating whether to set the game to a paused state (true) or
 *                 resume it (false).
 */
void GameWindowLabel::setPaused(bool isPaused) {
    this->isPaused = isPaused;
}


/**
 * @brief Processes player input for targeting and firing at enemy words.
 * @param c A QString containing the character entered by the player to target words.
 */
void GameWindowLabel::getKey(const QString &c) {
    if(c == ""){
        return;
    }
    cannonLen = 200;
    update();
    effect->play();
    timerCannon->start(150);

    QString mid = "";
    bool right;
    while(1) {
        if(lockedWord == NULL) {
            lockedWord = findLockedWord(mid, c);
        }
        if(lockedWord) {
            targetPoint = lockedWord->getPos();
            if(false == (right = lockedWord->appendBall(c))) {
                mid = lockedWord->getMidWord();
                lockedWord = NULL;
            } else {
                if(lockedWord->getDestoryed() == true) {
                    lockedWord = NULL;
                }
                break;
            }
        } else {
            break;
        }
    }

    if(right)
        rightCnt++;
    else
        wrongCnt++;

    ((GameWidget *)parent())->updateStatistic(rightCnt, wrongCnt);
}


/**
 * @brief Finds and locks the closest matching enemy word based on player input.
 * @param mid The currently matched portion of a word as the player types.
 * @param c The character entered by the player to match with a word.
 * @return A pointer to the closest matching enemy word (`EnemyWord *`), or `nullptr` if no match.
 */
EnemyWord * GameWindowLabel::findLockedWord(const QString &mid, const QString &c) {
    // Search for a word that best matches the input and lock onto it
    int h_min;
    EnemyWord *lword = NULL;
    if(mid != "") {
        h_min = 0;
        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            int y = word->getPos().y();

            if(word->getDestoryed() == false && word->getWord().mid(0, mid.size() + 1) == mid + c && y > h_min) {
                h_min = y;
                lword = word;
                word->setCurIndex(mid.size());
            }
        }
    }

    if(lword)
        return lword;

    h_min = 0;
    for(int i=0; i<wordList.size(); i++) {
        EnemyWord *word = wordList[i];
        int y = word->getPos().y();

        if(word->getDestoryed() == false && word->getWord()[0] == c && y > h_min) {
            h_min = y;
            lword = word;
            word->setCurIndex(0);
        }
    }
    return lword;
}

/**
 * @brief Generates a set of words/letters that fall downwards on the screen.
 * @param cnt The number of words/letters to generate for the current level.
 */
void GameWindowLabel::generateWords(int cnt) {
    if(begin == false)
        return;
    if(cnt + wordList.size() > level)
        cnt = level - wordList.size();

    int retry = 0;
    srand(QTime::currentTime().msec());
    for(int i=0; i<cnt; i++) {
        int w = rand() % width();
        int h = rand() % height() - height();
        int index = rand() % wordTable.size();
        EnemyWord *word = new EnemyWord(wordTable[index], firePoint, QPoint(w, h));
        bool isValid = true;
        if(word->getPos().x() - word->getHalfW() < 0 ||
                word->getPos().x() + word->getHalfW() > width()) {
            i--;
            isValid = false;
            retry++;
        } else {
            for(int j=0; j<wordList.size(); j++) {
                EnemyWord *w = wordList[j];

                if(abs(w->getPos().x() - word->getPos().x()) <= w->getHalfW() + word->getHalfW() + 2 && \
                        abs(w->getPos().y() - word->getPos().y()) <= w->getHalfH() + word->getHalfH() + 2) {
                    i--;
                    isValid = false;
                    retry++;
                    break;
                }
            }
        }

        if(isValid) {
            wordList.append(word);
        } else {
            delete word;
        }

        if(retry > 100)
            break;
    }

    while(1) {
        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            word->setPos(QPoint(word->getPos().x(), word->getPos().y() + 1));
            if(word->getPos().y() > 0 - word->getHalfH()) {
                return;
            }
        }
    }
}

/**
 * @brief Toggles the pause state of the game.
 */
void GameWindowLabel::onTogglePause() {
    if (isPaused) {  // If the current status is "paused" (isPaused == true), the game will resume
        timerRunning->start(GAMESPEED);  // restart the timer
        timerStatistic->start(INTERVAL);
        blinkTimer->stop();  // stop the blinker
        showPauseMessage = true;
        showStudyMessage = true;
        update();
    } else {  // If the current status is "resume" (isPaused == false), the game will pause
        timerRunning->stop();  // stop the timer
        timerStatistic->stop();
        blinkTimer->start();  // start the blinker
    }
    isPaused = !isPaused;  // change the game status
}

/**
 * @brief Sets the default font for the game.
 */
void GameWindowLabel::setFontDefault() {
    int fontId_regular = QFontDatabase::addApplicationFont(":/HamonyOS/fonts/HarmonyOS Sans/HarmonyOS_Sans_TC/HarmonyOS_Sans_TC_Regular.ttf");

    if (fontId_regular == -1) {
        qDebug() << "Failed to load font!";
    } else {
        QString fontFamily_regular = QFontDatabase::applicationFontFamilies(fontId_regular).at(0);

        QFont customFont;
        customFont.setFamily(fontFamily_regular);
        // customFont.setPixelSize(12);
        // customFont.setBold(false);

        // globalize the font format
        QApplication::setFont(customFont);
    }
}

/**
 * @brief Handles mouse press events to start the game or show a quit confirmation.
 * @param event The mouse press event.
 */
void GameWindowLabel::mousePressEvent(QMouseEvent *event) {
    // get the position of mouse click
    QPoint clickPos = event->pos();

    if (this->begin == false && logoRect.contains(clickPos)) {
        // if the click position is within the logoRect, then show the message box.
        QMessageBox msgBox;
        msgBox.setWindowTitle("Notice");
        msgBox.setText("Please ensure you have opened the camera!");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        // if the click position is within the logoRect, then begin the game.
        this->setRunning(true);
    }
    // if the click position is within the quitRect, then quit the game.
    if (this->isPaused == true && quitRect.contains(clickPos)) {
        showQuitConfirmation();
    }
    QLabel::mousePressEvent(event);
}

/**
 * @brief Toggles visibility of pause and study hints on the screen.
 */
void GameWindowLabel::toggleMessages() {
    showPauseMessage = !showPauseMessage;
    showStudyMessage = !showStudyMessage;
    update();
}


/**
 * @brief Displays a quit confirmation dialog.
 */
void GameWindowLabel::showQuitConfirmation() {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Quit", "Are you sure you want to quit the game? \nYour best performance will be saved to the Record page.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        this->setRunning(false);
    }
}

/**
 * @brief Handles the mouse enter event to show an alternate logo image.
 *
 * @param event The enter event.
 */
// Currently not in use!
void GameWindowLabel::enterEvent(QEnterEvent *event) {
    QPoint mousePos = event->pos();
    // Check if the mouse enters the logo area
    if (logoRect.contains(mousePos)) {
        isHovered = true;
        update(); // Trigger a repaint to show the new logo
    }
    QLabel::enterEvent(event);
}

/**
 * @brief Handles the mouse leave event to revert the logo to the original image.
 * @param event The leave event.
 */
// Currently not in use!
void GameWindowLabel::leaveEvent(QEvent *event) {
    isHovered = false; // Reset hover state
    update(); // Trigger a repaint to show the original logo
    QLabel::leaveEvent(event);
}
