#include "cannonball.h"

CannonBall::CannonBall(const QPoint &pos, QObject *parent) : QObject(parent)
{
    curPos = pos;
    active = true;
}

void CannonBall::ballMove(const QPoint &pos) {
    int dist;
    int X = pos.x() - curPos.x();
    int Y = pos.y() - curPos.y();
    if((dist = sqrt(X * X + Y * Y)) <= BALL_SPEED + 5) {
        curPos.setX(pos.x());
        curPos.setY(pos.y());
        active = false;
        return;
    }

    int diff_x = 1.0 * X * BALL_SPEED / dist;
    int diff_y = 1.0 * Y * BALL_SPEED / dist;

    curPos.setX(curPos.x() + diff_x);
    curPos.setY(curPos.y() + diff_y);
}

bool CannonBall::getActive(void) {
    return active;
}
const QPoint & CannonBall::getCurPos(void) {
    return curPos;
}
