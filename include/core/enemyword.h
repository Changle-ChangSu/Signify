/**
 * @class EnemyWord
 * @brief Represents an enemy word that can launch cannonballs at a target position.
 *
 * The EnemyWord class manages a string (`word`) displayed as an "enemy" with a designated position.
 * Cannonballs can be launched from `firePos` towards a target position (`curPos`).
 * Each correct letter signed by the player increments a progress index (`curIndex`) in the word,
 * and if the entire word is correctly signed, it is marked as "destroyed".
 * The word can be long or short.
 *
 * ### Member Variables
 * - `QString word` - The full word representing this enemy.
 * - `QPoint curPos` - Current position of the enemy word.
 * - `QPoint firePos` - The initial position from which cannonballs are fired, i.e., the cannon's muzzle.
 * - `bool destoryed` - Indicates if the word has been completely signed and "destroyed."
 * - `QList<CannonBall *> ballList` - List of active cannonballs moving towards the target.
 *                                    For a word with many letters, there may be many projectiles flying towords it in the same time.
 * - `QString midWord` - Portion of the word successfully signed so far, i.e., the red letters.
 *
 * ### Main Methods
 * - `bool appendBall(const QString &c)` - Adds a cannonball if `c` matches the next character in `word`. Marks word as "destroyed" if fully matched.
 * - `void ballsApproach()` - Moves all cannonballs closer to `curPos` and removes those that reach it.
 * - `QList<CannonBall *> *getBallList()` - Returns a pointer to the list of active cannonballs.
 * - `const QPoint &getPos()` - Returns the current position of the word.
 *
 */

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

};

#endif // ENEMYWORD_H
