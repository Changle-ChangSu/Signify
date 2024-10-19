#include <QFile>
#include <random>
#include <QFontDatabase>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QApplication>
#include "gamewindowlabel.h"
#include "gamewidget.h"


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

    // 创建定时器，控制闪烁效果
    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, &GameWindowLabel::toggleMessages);

    // 设置定时器间隔时间为 500 毫秒
    blinkTimer->setInterval(800);

    // ====================================制造单词列表，即下落的单词============================================

    // 使用随机数生成器生成 500 个随机数并插入 wordTable
    std::random_device rd;  // 用于生成随机种子
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
    std::uniform_int_distribution<> distrib(1, 99999); // 设置随机数范围，假设范围是 1 到 99999

    for(int i=0; i<500; i++) {
        int randomNumber = distrib(gen);
        wordTable.append(QString::number(randomNumber));
    }

    // 随机读取 1000 个单词并插入 wordTable
    QFile file(QCoreApplication::applicationDirPath() + "/words.txt");
    QStringList allWords;  // 存储从文件中读取的所有单词

    // 1. 读取文件并存储所有单词到 allWords 列表
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();  // 去除可能的空格或换行符
            if (!line.isEmpty()) {
                allWords.append(line);  // 将每行内容（即单词）加入 allWords 列表
            }
        }
        file.close();
    } else {
        qDebug() << "open words.txt failed!";
    }

    // 2. 检查文件中是否有足够的单词
    if (allWords.size() < 1000) {
        qDebug() << "Not enough words in the file!";
    } else {
        // 3. 使用随机数生成器随机选择 1000 个单词
        std::random_device rd;  // 用于生成随机种子
        std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎
        std::uniform_int_distribution<> distrib(0, allWords.size() - 1);  // 生成 0 到 allWords.size()-1 之间的随机数

        QSet<int> selectedIndices;  // 用于存储已经选过的索引，防止重复选择

        while (selectedIndices.size() < 1000) {
            int randomIndex = distrib(gen);  // 生成随机索引
            if (!selectedIndices.contains(randomIndex)) {
                selectedIndices.insert(randomIndex);  // 记录选中的索引，防止重复
                wordTable.append(allWords[randomIndex]);  // 随机选择单词并插入 wordTable
            }
        }

        // qDebug() << "Successfully added 1000 random words to wordTable!";
    }

    // =================================================================================================

    effect = new QSoundEffect(this);
    effect->setSource(QUrl("qrc:///game_sound_effect/sounds/eject.wav"));
    effect->setLoopCount(1);  //循环次数
    effect->setVolume(0.2f); //音量  0~1之间

    effectBoom = new QSoundEffect(this);
    effectBoom->setSource(QUrl("qrc:///game_sound_effect/sounds/wordBoom.wav"));
    effectBoom->setLoopCount(1);  //循环次数
    effectBoom->setVolume(30.0f); //音量  0~1之间

    setStyleSheet("background-color:#f7f9fc");  // 设置背景颜色


    timerCannon = new QTimer(this);
    timerCannon->setSingleShot(true);  // 设置为单次触发，即触发一次后自动停止。
    connect(timerCannon, &QTimer::timeout, this, [=](){
        cannonLen = 220;
        update();
    });  // 当定时器超时时，重置炮管长度为 220 并调用 update() 函数刷新窗口。这用于处理发射动画和子弹发射过程。


    // ====================================设置游戏主计时器timerRunning==========================================

    timerRunning = new QTimer(this);  // 游戏的主定时器，每 30 毫秒触发一次，用于处理敌人（单词）的移动和状态更新。

    // 连接 GameWidget 的 togglePause 信号到 GameWindowLabel 的 onTogglePause 槽函数
    connect((GameWidget *)parent, &GameWidget::togglePause, this, &GameWindowLabel::onTogglePause);

    connect(timerRunning, &QTimer::timeout, this, [=](){
        if(begin == false || isPaused == true) {
            return;
        }  // 如果游戏没有开始或者暂停了，则不执行

        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            word->setPos(QPoint(word->getPos().x(), word->getPos().y() + 1));
            word->ballsApproach();
        }

        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            if(word->getDestoryed() == true && word->getBallList()->size() == 0) {
                wordList.removeOne(word);
                score += word->getWord().size() * level;  // 加分规则：单词大小*等级
                delete word;
                word = NULL;
                effectBoom->play();
                continue;
            }

            // 如果单词跑到deadline上了，则游戏失败
            if(word->getDestoryed() == false && word->getPos().y() > gameDeadLine) {
//                if(lockedWord == word) {
//                    lockedWord = NULL;
//                }
//                wordList.removeAt(i);
//                delete word;

                // lockedWord = NULL;
//                int size = wordList.size();
//                for(int i=0; i<size; i++) {
//                    EnemyWord *word = wordList[0];
//                    wordList.removeAt(i);
//                    delete word;
//                }
                // foreach(EnemyWord *w, wordList)
                // {
                //     if(w)
                //     {
                //         wordList.removeOne(w);
                //         delete w;
                //         w = nullptr;
                //     }
                // }
                // begin = false;
                // emit gameStatusChanged(begin);
                this->setRunning(false);
            }
        }

        update();
    });


    timerRunning->start(GAMESPEED);  // 控制游戏速度（变速齿轮）初始是30ms一次，包括下落速度、炮弹速度等所有速度


    // ===================================设置单词计时器timerStatistic========================================

    timerStatistic = new QTimer(this);
    connect(timerStatistic, &QTimer::timeout, this, [=](){
        if(begin == false)
            return;

        level = ((GameWidget *)parent)->getMaxLvl();
        generateWords(level);

    });
    timerStatistic->start(INTERVAL);  // 每一秒钟出一组单词（真空期）
}

// =========================================================================================================


// 绘图事件：负责绘制页面上的所有图形
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


    // ========================================绘制游戏封面===============================================

    if (this->begin == false) {
        setFontDefault();
        // 设置 "CodeAllNight Group Present" 样式
        QFont smallFont = font;
        smallFont.setPixelSize(16);  // 小字体
        painter.setFont(smallFont);

        // 计算 "CodeAllNight Group Present" 文本高度
        QFontMetrics smallMetrics(smallFont);
        int smallTextHeight = smallMetrics.height();

        // 绘制 "CodeAllNight Group Present"
        QRect smallTextRect(rect.left(), rect.top() + 20, rect.width(), smallTextHeight);
        painter.drawText(smallTextRect, Qt::AlignTop | Qt::AlignHCenter, "CodeAllNight Group Present");

        // 设置 "Signify Strike" 样式
        QFont boldFont = font;
        boldFont.setPixelSize(50);  // 大字体
        boldFont.setBold(true);     // 加粗
        painter.setFont(boldFont);

        // 计算 "Signify Strike" 文本高度
        QFontMetrics boldMetrics(boldFont);
        int boldTextHeight = boldMetrics.height();

        // 绘制带阴影效果的主标题
        QPen penShadow(Qt::gray);   // 灰色阴影
        painter.setPen(penShadow);

        QRect boldTextShadowRect(rect.left(), rect.top() + smallTextHeight + 20, rect.width(), boldTextHeight);
        painter.drawText(boldTextShadowRect.adjusted(2, 2, 2, 2), Qt::AlignHCenter, "Signify Strike");  // 阴影位置略偏

        QPen penMain(Qt::black);    // 黑色主标题
        painter.setPen(penMain);
        painter.drawText(boldTextShadowRect, Qt::AlignHCenter, "Signify Strike");

        // 启用抗锯齿(反走样)
        painter.setRenderHint(QPainter::Antialiasing, true);

        // 绘制Logo图片并保持比例
        logoRect = QRect(rect.left(), rect.top() + smallTextHeight + boldTextHeight + 30, rect.width(), 200);
        QPixmap pixmap(":/images/images/GameLogo.png");
        QPixmap scaledPixmap = pixmap.scaled(logoRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int xOffset = (logoRect.width() - scaledPixmap.width()) / 2;
        int yOffset = (logoRect.height() - scaledPixmap.height()) / 2;
        painter.drawPixmap(logoRect.left() + xOffset, logoRect.top() + yOffset, scaledPixmap);

        // 设置 "Game Instruction:" 样式
        QFont instructionTitleFont = font;
        instructionTitleFont.setPixelSize(20);  // 增大字号
        instructionTitleFont.setBold(true);     // 加粗
        painter.setFont(instructionTitleFont);

        // 计算 "Game Instruction" 的位置并绘制
        QRect instructionTitleRect(rect.left()+20, rect.top() + smallTextHeight + boldTextHeight + 210, rect.width()-20, 50);
        painter.drawText(instructionTitleRect, Qt::AlignCenter | Qt::AlignTop, "Game Instruction");

        // 恢复字体样式为普通的说明文字
        QFont instructionFont = font;
        instructionFont.setPixelSize(15);       // 普通大小
        instructionFont.setBold(false);         // 不加粗
        painter.setFont(instructionFont);

        // 绘制说明文字
        QRect instructionTextRect(rect.left()+20, rect.top() + smallTextHeight + boldTextHeight + 250, rect.width()-40, 290);
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


    // =======================================绘制暂停提示页面====================================================

    if (isPaused == true) {
        quitRect = QRect(10, gameDeadLine - 60, 200, 60);

        QFont font = painter.font();
        font.setPixelSize(20);  // 设置字体大小
        painter.setFont(font);

        // 绘制“Game Paused”提示（根据闪烁状态显示）
        if (showPauseMessage) {
            painter.setPen(Qt::red);  // 红色字体
            painter.drawText(quitRect, Qt::AlignLeft, "Game Paused\n※ Click here to QUIT");
        }

        // 绘制“Feel free to go to the study page”提示（根据闪烁状态显示）
        if (showStudyMessage) {
            painter.setPen(Qt::blue);  // 红色字体
            painter.drawText(QRect(width() - 330, gameDeadLine - 35, 320, 30), Qt::AlignRight, "Feel free to go to the study page!");
        }
    }

}

bool GameWindowLabel::isRunning(void) {
    return begin;
}

void GameWindowLabel::setRunning(bool begin) {
    this->setFocus();
    this->begin = begin;
    emit gameStatusChanged(begin);
    if(this->begin) {
        level = 1;
        score = 0;
        rightCnt = wrongCnt = 0;

        timerRunning->start(GAMESPEED);  // 恢复计时器
        timerStatistic->start(INTERVAL);
        // 如果游戏恢复，停止定时器，并确保提示信息不再显示
        blinkTimer->stop();

        cannonLen = 220;
        lockedWord = NULL;

        generateWords(level);
    } else {

        // 保存新的新的最高分============================================
        QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
        // 读取已保存的最高分数，默认为 0
        int savedHighScore = settings.value("HighScore", 0).toInt();
        // 比较当前分数和已保存的最高分数
        if (score > savedHighScore) {
            // 如果当前分数更高，则将其保存为新的最高分数
            settings.setValue("HighScore", score);

            // 可以添加一些 UI 提示，如 "新的最高分数!"
            // qDebug() << "New high score: " << score;
        } else {
            // 如果当前分数没有超过最高分数
            // qDebug() << "Current score: " << score << ", High score: " << savedHighScore;
        }

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


void GameWindowLabel::setPaused(bool isPaused) {
    this->isPaused = isPaused;
}




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

EnemyWord * GameWindowLabel::findLockedWord(const QString &mid, const QString &c) {
    // 找一个最佳的单词并锁定
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

// timer 20ms 1px down
// 1000 * 20ms = 20s.s
// 20s * 2的时间单词下落下来如果能全部打完
// 统计40s的时间能打多少单词，就是相当的等级
// 统计10s的时间， * 4 就是相当的等级
// 切换等级：
// 等最后一单词显示出来，等5秒进行下一轮
// 生成单词的时候要往下平移至第一个单词显示
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


void GameWindowLabel::onTogglePause() {
    if (isPaused) {
        timerRunning->start(GAMESPEED);  // 恢复计时器
        timerStatistic->start(INTERVAL);
        // 如果游戏恢复，停止定时器，并确保提示信息不再显示
        blinkTimer->stop();
        showPauseMessage = true;
        showStudyMessage = true;
        update();  // 触发重绘
    } else {
        timerRunning->stop();  // 暂停计时器
        timerStatistic->stop();
        // 如果游戏暂停，启动定时器
        blinkTimer->start();
    }
    isPaused = !isPaused;  // 切换暂停状态
    // qDebug() << isPaused << begin;
}


void GameWindowLabel::setFontDefault() {
    // 加载全局默认自定义字体
    int fontId_regular = QFontDatabase::addApplicationFont(":/HarmonyOS_Sans_TC/HarmonyOS-Sans/HarmonyOS Sans/HarmonyOS_Sans_TC/HarmonyOS_Sans_TC_Regular.ttf");

    if (fontId_regular == -1) {
        qDebug() << "Failed to load font!";
    } else {
        // 获取字体系列名称
        QString fontFamily_regular = QFontDatabase::applicationFontFamilies(fontId_regular).at(0);

        // 创建 QFont 对象
        QFont customFont;
        customFont.setFamily(fontFamily_regular);
        // customFont.setPixelSize(12);  // 你可以根据需要调整字体大小
        // customFont.setBold(false);    // 根据需要设置粗体

        // 设置全局字体
        QApplication::setFont(customFont);
    }
}


void GameWindowLabel::mousePressEvent(QMouseEvent *event) {
    // 获取鼠标点击的坐标
    QPoint clickPos = event->pos();

    // 检查点击是否在 Logo 区域内
    if (this->begin == false && logoRect.contains(clickPos)) {
        // 如果点击在Logo内，开始游戏
        this->setRunning(true);
    }

    // 检查点击是否在 QUIT 区域内
    if (this->isPaused == true && quitRect.contains(clickPos)) {
        // 如果点击在QUIT内，弹窗询问是否要结束游戏，若选择是，则结束游戏
        showQuitConfirmation();

    }
    // 如果需要，还可以调用父类的默认事件处理
    QLabel::mousePressEvent(event);
}


void GameWindowLabel::toggleMessages() {
    showPauseMessage = !showPauseMessage;  // 切换“Game Paused”显示状态
    showStudyMessage = !showStudyMessage;  // 切换“Feel free to go to the study page”显示状态
    update();  // 触发重绘，刷新界面
}


// 显示退出确认弹窗
void GameWindowLabel::showQuitConfirmation() {
    // 创建一个 QMessageBox 问题对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Quit", "Are you sure you want to quit the game? \nYour best performance will be saved to the Record page.",
                                  QMessageBox::Yes | QMessageBox::No);

    // 根据用户的选择执行不同操作
    if (reply == QMessageBox::Yes) {
        // 如果用户选择“是”，退出游戏
        this->setRunning(false);
    } else {
        // 如果用户选择“否”，关闭弹窗，继续游戏
        // QMessageBox::question 是模态的，自动关闭，无需手动处理
    }
}
