#ifndef PLAYER_H
#define PLAYER_H

#include "movingItem.h"

/**
 * @brief A class from which the player's avatar can be created.

 *
 */
class Player : public MovingItem
{
public:
    Player();

    /**
     * @brief Constructor which should typically be used to create a Player object
     *
     * @param x the desired x offset of the player
     * @param y the desired y offset of the player
     * @param p_type the desired type of the player
     * @param image_string path to the player's image
     */
    Player(int x, int y, ITEM_TYP p_type, const char* image_string);

    /**
     * @brief Simple getter for the current score
     *
     * @return the current score of the player
     */
    int getScore(void);

    /**
     * @brief Simple setter for player's score
     *
     * @param resetScore whether if the player's score should be reset
     * @param gain additional points to be added to the player's current score
     */
    void setScore(bool resetScore, int gain);

    /**
     * @brief Simple setter for player's life
     *
     * @param lives player's current life set to the input value
     */
    void setLife(int lives);

    /**
     * @brief Simple getter for the player's current life
     *
     * @return the player's current lives
     */
    int getLife();

    /**
     * @brief Simple getter for the Blocking Timer
     *
     * @return time passed after the blocking timer started
     */
    int getBlockingTime();

    /**
     * @brief Simple setter for the Blocking Timer
     *
     * @return time passed after the blocking timer started
     */
    void setBlockingTime(int time);

    /**
     * @brief Manipulate game's displayed clock
     *
     * @param input value added to the game's displayed clock
     */
    void addTime(int time);

    /**
     * @brief Simple getter for the game's clock (not remaining time)
     *
     * @return time passed after the level's launch
     */
    int getTime();

    /**
     * @brief Simple getter for the game's countdown clock
     *
     * @return game's set countdown clock
     *
     * This function does not return the remaining time!
     * Original set countdown clock, which was saved to the map file,
     * can be read by calling this function
     */
    int getCountdown();

    /**
     * @brief Simple setter for the game's countdown clock
     *
     * @return set game's countdown clock
     */
    void setCountdown(int time);


    Timer blockTimer;

    /**
     * @brief Object needed to calculate the remaining time of a level
     *
     */
    Timer displayClock;

    void looseLife();
    void respawn();
    bool invulnerable();
    bool looksLeft(){return lookingleft;}
    bool shoot();
    bool waitForBullet;
    bool isInSuperMode(){return this->superMode;}
    void setToSuperMode(){this->superMode = true;}
    int getHighScore();
    void setHighScore(int score);
    double friction;

private:
    bool superMode;
    int power;
    int score;
    int life;
    int blockingTime;
    int manipulatedTime;
    int countdown;
    int highScore;

    Timer tInvul;
    Timer shootTimer;

};

#endif // PLAYER_H
