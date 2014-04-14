#ifndef LOGIC_H
#define LOGIC_H

#include "SDL/SDL.h"
#include <string>
#include "movingItem.h"
#include "player.h"
#include <iterator>

class map;

using namespace std;

/**
 * @brief A class responsible for moving Item objects, detecting collisions and resolving them.

 * Various methods for handling collisions between different objects exist. Depending on 
 * the type of objects which collide it might be important to know wether the collision took place
 * in X or in Y direction. 
 * 
 */
class Logic
{
public:
    Logic();
/**
 * @brief Method that encapsulates the hands on game logic such as movement.

 * In order to move an Item object its position is first changed preliminarily.
 * Then collisions are checked. In case there are no collisions the Item is not moved back.
 * Incase the object shows a collision either in x or y direction, appropriate action is then 
 * taken by specialized methods.
 */
    void run(vector<Item*> &items,map *tmap);

private:
    map* mapPointer;
    vector<Item*> *itemPointer;
    bool standing;

/**
 * @brief Convenience function to move an item by its delta
 */
    void moveX(MovingItem *myitem);
/**
 * @brief Convenience function to move an item by its delta
 */
    void moveY(MovingItem *myitem);
/**
 * @brief Convenience function to move an item back by its delta
 */
    void moveXBack(MovingItem *myitem);
/**
 * @brief Convenience function to move an item back by its delta
 */
    void moveYBack(MovingItem *myitem);
/**
 * @brief Convenience function to check if two bounding rectangles are overlapping
 * @returns wether a collision has happened
 */
    int checkCollision(Item *myitem1, Item *myitem2 );


/**
 * @brief Move a crab object and check for collisions
 */
    void handleCrab(int pos,vector<Item*> &items);
/**
 * @brief Move a grr object and check for collisions
 */

    void handleGrrr(int pos,vector<Item*> &items);
/**
 * @brief Move a Cloud object and check for collisions
 */
    void handleFightCloud(int pos,vector<Item *> &items);
/**
 * @brief Move a bullet object and check for collisions
 */

    void handleBullet(int pos,vector<Item*> &items);
/**
 * @brief Move a bullet object check for timeouts and handle collisions
 */

    void handlePrincessBullet(int pos,vector<Item*> &items);

/**
 * @brief Move a bullet object and check for collisions
 */
    void handleGhost(int pos,vector<Item*> &items);

/**
 * @brief Move a bullet object and check for collisions
 */
    void handleTrapStone(int pos,vector<Item*> &items);

/**
 * @brief Move the player and handle collisions appropriately
 */
    void handlePlayer(Item *tmp,vector<Item*> &items);

/**
 * @brief Handle a specific type of collision
 */
    void collisionPlayerMapX(Player *tmp, int pos, vector<Item *> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionPlayerMapY(Player *tmp, int pos, vector<Item *> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionPlayerGhostXY(Player *tmp,int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionPlayerCrabX(Player *tmp, int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionPlayerCrabY(Player *tmp,int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionPlayerCoinXY(Player *tmp,int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionHeartXY(Player *tmp,int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionStarXY(Player *tmp,int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionPrincesBulletXY(Player *tmp,int pos,vector<Item*> &items);
/**
 * @brief Handle a specific type of collision
 */
    void collisionWaterYX(Player *tmp);
/**
 * @brief Handle a specific type of collision
 */
    void collisionGoalXY();
/**
 * @brief Handle a specific type of collision
 */
    void collisionGrrrXY(Player* tmp);


};

#endif // LOGIC_H

