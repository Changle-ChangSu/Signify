#include "cannonball.h"

/// Constructor, initializes the cannonball’s starting position and active status.
/// @param pos Initial position of the cannonball.
/// @param parent Parent object (used for Qt’s object management).
CannonBall::CannonBall(const QPoint &pos, QObject *parent) : QObject(parent)
{
    curPos = pos;
    active = true;
}

/// Updates the cannonball's position towards the target, simulating movement.
/// @param pos Target position.
/// If the cannonball reaches the target, `active` is set to `false`.
void CannonBall::ballMove(const QPoint &pos) {
    int dist;
    int X = pos.x() - curPos.x();
    int Y = pos.y() - curPos.y();

    // If the current distance between the projectile and the word brick is less than 5,
    // then we assume the projectile has hit the word brick.
    if((dist = sqrt(X * X + Y * Y)) <= BALL_SPEED + 5) {
        curPos.setX(pos.x());
        curPos.setY(pos.y());
        active = false;
        return;
    }

    // If not hitting, the projectile will move ahead.
    int diff_x = 1.0 * X * BALL_SPEED / dist;
    int diff_y = 1.0 * Y * BALL_SPEED / dist;

    curPos.setX(curPos.x() + diff_x);
    curPos.setY(curPos.y() + diff_y);
}

/// Returns the current activity status of the cannonball.
/// @return `true` if the cannonball is still moving, otherwise `false`.
bool CannonBall::getActive(void) {
    return active;
}
/// Returns the current position of the cannonball.
/// @return The current `QPoint`, representing the cannonball’s position.
const QPoint & CannonBall::getCurPos(void) {
    return curPos;
}
