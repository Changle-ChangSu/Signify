#include <QFile>
#include <random>

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

    setStyleSheet("background-color:#5095Bb");  // 设置背景颜色


    timerCannon = new QTimer(this);
    timerCannon->setSingleShot(true);  // 设置为单次触发，即触发一次后自动停止。
    connect(timerCannon, &QTimer::timeout, this, [=](){
        cannonLen = 220;
        update();
    });  // 当定时器超时时，重置炮管长度为 220 并调用 update() 函数刷新窗口。这用于处理发射动画和子弹发射过程。


    // ====================================设置游戏主计时器timerRunning==========================================

    timerRunning = new QTimer(this);  // 游戏的主定时器，每 30 毫秒触发一次，用于处理敌人（单词）的移动和状态更新。
    connect(timerRunning, &QTimer::timeout, this, [=](){
        if(begin == false) {
            return;
        }  // 如果游戏没有开始，则不执行

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
            if(word->getDestoryed() == false && word->getPos().y() > gameDeadLine) {
//                if(lockedWord == word) {
//                    lockedWord = NULL;
//                }
//                wordList.removeAt(i);
//                delete word;

                lockedWord = NULL;
//                int size = wordList.size();
//                for(int i=0; i<size; i++) {
//                    EnemyWord *word = wordList[0];
//                    wordList.removeAt(i);
//                    delete word;
//                }
                foreach(EnemyWord *w, wordList)
                {
                    if(w)
                    {
                        wordList.removeOne(w);
                        delete w;
                        w = nullptr;
                    }
                }
                begin = false;
            }
        }

        update();
    });


    timerRunning->start(30);  // 控制游戏速度（变速齿轮）初始是30ms一次，包括下落速度、炮弹速度等所有速度


    // ===================================设置单词计时器timerStatistic========================================

    timerStatistic = new QTimer(this);
    connect(timerStatistic, &QTimer::timeout, this, [=](){
        if(begin == false)
            return;

        level = ((GameWidget *)parent)->getMaxLvl();
        generateWords(level);

    });
    timerStatistic->start(1000);  // 每一秒钟出一个单词（真空期）
}

// =========================================================================================================

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

    //painter.drawRect(fireRect);

    QPen pen = painter.pen();
    pen.setWidth(4);
    painter.setPen(pen);

    gameDeadLine = fireRect.top() - 20;
    painter.setBrush(Qt::lightGray);
    //painter.drawRect(0, gameDeadLine, rect.width(), 500);
    //painter.drawLine(0, gameDeadLine, rect.right(), gameDeadLine);

    pen.setWidth(4);
    painter.setPen(pen);


    //painter.drawLine(firePoint, targetPoint);
    QFont font = painter.font();
    font.setFamily("Courier New");
    font.setPixelSize(30);
    font.setBold(true);
    painter.setFont(font);

    painter.drawRoundedRect(levelRect, 20, 20);
    painter.drawRoundedRect(scoreRect, 20, 20);
    painter.drawText(levelRect, Qt::AlignCenter, "lvl:" + QString::number(level));
    painter.drawText(scoreRect, Qt::AlignCenter, QString::number(score));

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

    double angle = atan(1.0 * (targetPoint.x() - firePoint.x()) / (firePoint.y() - targetPoint.y()));

    painter.save();
    painter.translate(firePoint);
    painter.rotate(angle * 180 / 3.1415926);
    QRect rectCannon;
    rectCannon.setTop(0 - cannonLen);
    rectCannon.setBottom(0 + 220);
    rectCannon.setLeft(0 - 18);
    rectCannon.setRight(0 + 18);
    painter.drawRoundedRect(rectCannon, 8, 8);
    painter.restore();

    //painter.setBrush(Qt::black);
    painter.drawEllipse(fireRect);


    if(begin == false) {
        QRect rectImg = rect;
        rectImg.setTop(160);
        rectImg.setBottom(rect.height() - 310);

        font.setPixelSize(40);
        painter.setFont(font);
        painter.drawText(rect, Qt::AlignCenter,
                         "请使用标准指法进行打字练习"
                         "\n\n"
                         "\n\n"
                         "\n\n"
                         "\n\n"
                         "\n"
                         "左下角设置难度等级\n"
                         "同时按下asdf或jkl;开始游戏"
                         "\n\n");

        // 启用抗锯齿(反走样)
        painter.setRenderHint(QPainter::Antialiasing, true);
        // 指定要绘制的图片（将图片路径替换为有效的图片路径）
        painter.drawPixmap(rectImg, QPixmap(":/type.jpg"));
    }

}

bool GameWindowLabel::isRunning(void) {
    return begin;
}

void GameWindowLabel::setRunning(bool begin) {
    this->begin = begin;
    if(begin) {
        level = 1;
        score = 0;
        rightCnt = wrongCnt = 0;

        cannonLen = 220;
        lockedWord = NULL;

        generateWords(level);
    }
}

void GameWindowLabel::getKey(const QString &c) {
    if(c == "")
        return;
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
