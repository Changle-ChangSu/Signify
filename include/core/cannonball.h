/**
 * @class CannonBall
 * @brief Simulates a cannonball moving towards a target at a constant speed after the user gesture the right sign language.
 *
 * The CannonBall class represents a cannonball moving in 2D space. It accepts a target position,
 * and through the `ballMove` method, moves towards the target at a set speed `BALL_SPEED`. The class contains
 * information such as the current position, activity status, and can be used for simulating a projectile in games.
 *
 * ### Member Variables
 * - `QPoint curPos`: Current position of the cannonball.
 * - `bool active`: Indicates whether the cannonball is still moving (i.e., it has not yet reached the target).
 *
 * ### Main Methods
 * - `CannonBall(const QPoint &pos, QObject *parent = nullptr)`: Constructor that initializes the cannonball’s position and sets its active status.
 * - `void ballMove(const QPoint &pos)`: Updates the cannonball's position towards the target. If the target is reached, `active` is set to `false`.
 * - `bool getActive()`: Returns the active status of the cannonball.
 * - `const QPoint &getCurPos()`: Returns the current position of the cannonball.
 *
 * ### Notes
 * - `BALL_SPEED` should be defined as a constant to ensure a consistent movement speed in the very beginning,
 *    the bigger the number, the faster the projectile move.
 * - The `ballMove` method assumes the target position remains constant, suitable for simple linear motion.
 *   In our default game, the word (or letter) brick drop in a constant speed, so it works.
 */

#ifndef CANNONBALL_H
#define CANNONBALL_H

#include <QObject>
#include <QPoint>

#define BALL_SPEED 40

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

// signals:

};

#endif // CANNONBALL_H
