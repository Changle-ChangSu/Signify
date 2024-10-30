#include "enemyword.h"


/**
 * @brief Constructs an EnemyWord instance with a given word, firing position, and initial position.
 * @param word The word to be signed to "destroy" this enemy.
 * @param firePos The position from which cannonballs will be fired.
 * @param pos The initial position of the word.
 * @param parent The parent QObject, used for Qt's memory management.
 */
EnemyWord::EnemyWord(const QString &word, const QPoint &firePos, const QPoint &pos, QObject *parent) : QObject(parent)
{
    this->firePos = firePos;
    curPos = pos;
    this->word = word;
    curIndex = 0;
    destoryed = false;
    // related with the size of the word brick
    half_w = word.size() * 12 + 10;
    half_h = 25;
}

/**
 * @brief Returns the half-width of the word area for display or collision.
 * @return Half-width value.
 */
int EnemyWord::getHalfW(void) {
    return half_w;
}
/**
 * @brief Returns the half-height of the word area for display or collision.
 * @return Half-height value.
 */
int EnemyWord::getHalfH(void) {
    return half_h;
}

/**
 * @brief Attempts to add a cannonball if `c` matches the next character in `word`.
 * @param c The character signed by the player.
 * @return True if `c` matches the next character in `word`, false otherwise.
 *
 * If `c` is correct, a cannonball is launched from `firePos` toward `curPos`.
 * If the entire word is correctly signed, the word is marked as destroyed.
 */
bool EnemyWord::appendBall(const QString &c) {
    bool ret;
    // If the character matches, progress in the word
    if(c == word[curIndex]) {
        curIndex++;
        // Word is fully signed, mark as destroyed
        if(curIndex == word.size()) {
            destoryed = true;
            curIndex = 0;
        }
        ret = true;

    } else {
        // Incorrect character is signed, reset progress
        midWord = word.mid(0, curIndex);

        curIndex = 0;
        return false;
    }
    // Create and move a new cannonball toward the target position
    CannonBall *ball = new CannonBall(firePos);
    for(int i=0; i<8; i++) {
        ball->ballMove(curPos);
    }
    ballList.append(ball);

    return ret;
}

/**
 * @brief Moves all cannonballs in `ballList` closer to `curPos`, removing any that have reached it.
 */
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

/**
 * @brief Destructor to clean up any active cannonballs.
 */
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

/**
 * @brief Returns a pointer to the list of active cannonballs.
 * @return Pointer to `ballList`.
 */
QList<CannonBall *> * EnemyWord::getBallList(void) {
    return &ballList;
}

/**
 * @brief Returns the current position of the word.
 * @return Reference to `curPos`.
 */
const QPoint & EnemyWord::getPos(void) {
    return curPos;
}

/**
 * @brief Sets the current position of the word.
 * @param pos The new position for the word.
 */
void EnemyWord::setPos(const QPoint &pos) {
    curPos = pos;
}

/**
 * @brief Returns the complete target word.
 * @return Reference to `word`.
 */
const QString & EnemyWord::getWord(void) {
    return word;
}
/**
 * @brief Returns the portion of the word correctly signed so far.
 * @return Reference to `midWord`.
 */
const QString & EnemyWord::getMidWord(void) {
    return midWord;
}
/**
 * @brief Returns the current index indicating progress in signing the word.
 * @return Current index in `word`.
 */
int EnemyWord::getCurIndex(void) {
    return curIndex;
}
/**
 * @brief Sets the current index to a specific value.
 * @param index The new index value.
 */
void EnemyWord::setCurIndex(int index) {
    curIndex = index;
}
/**
 * @brief Indicates whether the word has been completely signed and destroyed.
 * @return True if the word has been fully signed; otherwise, false.
 */
bool EnemyWord::getDestoryed(void) {
    return destoryed;
}
