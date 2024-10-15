#include <QFile>

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

    for(int i=0; i<10; i++) {
        wordTable.append(QString::number(i));
    }

    QFile file(QCoreApplication::applicationDirPath() + "/words.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
          QString line = in.readLine();
          if(line != "" && !wordTable.contains(line)) {
              wordTable.append(line);
          }
        }
        file.close();
    } else {
        //qDebug() << "open words.txt failed!";
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.flush();
            file.close();
        }
    }


    effect = new QSoundEffect(this);
    effect->setSource(QUrl("qrc:///type.wav"));
    effect->setLoopCount(1);  //循环次数
    effect->setVolume(1.0f); //音量  0~1之间

    effectBoom = new QSoundEffect(this);
    effectBoom->setSource(QUrl("qrc:///wordBoom.wav"));
    effectBoom->setLoopCount(1);  //循环次数
    effectBoom->setVolume(1.0f); //音量  0~1之间

    setStyleSheet("background-color:#5095Bb");


    timerCannon = new QTimer(this);
    timerCannon->setSingleShot(true);
    connect(timerCannon, &QTimer::timeout, this, [=](){
        cannonLen = 220;
        update();
    });

    timerRunning = new QTimer(this);
    connect(timerRunning, &QTimer::timeout, this, [=](){
        if(begin == false) {
            return;
        }

        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            word->setPos(QPoint(word->getPos().x(), word->getPos().y() + 1));
            word->ballsApproach();
        }

        for(int i=0; i<wordList.size(); i++) {
            EnemyWord *word = wordList[i];
            if(word->getDestoryed() == true && word->getBallList()->size() == 0) {
                wordList.removeOne(word);
                score += word->getWord().size();
                delete word;
                word = NULL;
                //effectBoom->play();
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


    timerRunning->start(20);


    timerStatistic = new QTimer(this);
    connect(timerStatistic, &QTimer::timeout, this, [=](){
        if(begin == false)
            return;

        level = ((GameWidget *)parent)->getMaxLvl();
        generateWords(level);

    });
    timerStatistic->start(1 * 1000);
}

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
    //effect->play();
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
