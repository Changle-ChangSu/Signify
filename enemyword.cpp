#include "enemyword.h"

EnemyWord::EnemyWord(const QString &word, const QPoint &firePos, const QPoint &pos, QObject *parent) : QObject(parent)
{
    this->firePos = firePos;
    curPos = pos;
    this->word = word;
    curIndex = 0;
    destoryed = false;

    half_w = word.size() * 12 + 10;
    half_h = 25;
}

int EnemyWord::getHalfW(void) {
    return half_w;
}
int EnemyWord::getHalfH(void) {
    return half_h;
}

bool EnemyWord::appendBall(const QString &c) {
    bool ret;
    // 如果c是正确的，则添加ball
    // 否则就不添加
    //qDebug() << c << word[curIndex];
    if(c == word[curIndex]) {
        curIndex++;
        if(curIndex == word.size()) {
            destoryed = true;
            curIndex = 0;
        }
        ret = true;

    } else {
        midWord = word.mid(0, curIndex);

        curIndex = 0;
        return false;
    }

    CannonBall *ball = new CannonBall(firePos);
    for(int i=0; i<8; i++) {
        ball->ballMove(curPos);
    }
    ballList.append(ball);

    return ret;
}

void EnemyWord::ballsApproach(void) {
    for(int i=0; i<ballList.size(); i++) {
        ballList[i]->ballMove(curPos);
        if(ballList[i]->getActive() == false) {
            CannonBall *ball = ballList[i];
            ballList.removeOne(ball);
            delete ball;
            ball = NULL;
        }
    }
}

EnemyWord::~EnemyWord() {
    foreach(CannonBall *b, ballList)
    {
        if(b)
        {
            ballList.removeOne(b);
            delete b;
            b = nullptr;
        }
    }
}

QList<CannonBall *> * EnemyWord::getBallList(void) {
    return &ballList;
}

const QPoint & EnemyWord::getPos(void) {
    return curPos;
}

void EnemyWord::setPos(const QPoint &pos) {
    curPos = pos;
}

const QString & EnemyWord::getWord(void) {
    return word;
}
const QString & EnemyWord::getMidWord(void) {
    return midWord;
}
int EnemyWord::getCurIndex(void) {
    return curIndex;
}
void EnemyWord::setCurIndex(int index) {
    curIndex = index;
}

bool EnemyWord::getDestoryed(void) {
    return destoryed;
}
