#ifndef CANNONBALL_H
#define CANNONBALL_H

#include <QObject>
#include <QPoint>

#define BALL_SPEED 30
// 40ms前进一次
class CannonBall : public QObject
{
    Q_OBJECT

    bool active;
    QPoint curPos;

public:
    explicit CannonBall(const QPoint &pos, QObject *parent = nullptr);
    void ballMove(const QPoint &pos);
    bool getActive(void);
    const QPoint & getCurPos(void);

signals:

};

#endif // CANNONBALL_H
