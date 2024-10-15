#ifndef ENEMYWORD_H
#define ENEMYWORD_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "cannonball.h"

class EnemyWord : public QObject
{
    Q_OBJECT

    bool destoryed;
    QString word;
    QString midWord;
    QPoint curPos;
    QPoint firePos;

    int curIndex;

    QList<CannonBall *> ballList;

    int half_w, half_h;

public:
    explicit EnemyWord(const QString &word, const QPoint &firePos, const QPoint &pos, QObject *parent = nullptr);
    ~EnemyWord();
    bool appendBall(const QString &c);
    void ballsApproach(void);
    QList<CannonBall *> * getBallList(void);
    const QPoint & getPos(void);
    void setPos(const QPoint &pos);
    const QString & getWord(void);
    const QString & getMidWord(void);
    bool getDestoryed(void);

    int getCurIndex(void);
    void setCurIndex(int index);
    int getHalfW(void);
    int getHalfH(void);


signals:

};

#endif // ENEMYWORD_H
