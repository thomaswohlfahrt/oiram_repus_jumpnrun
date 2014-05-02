#ifndef MOVINGITEM_H
#define MOVINGITEM_H

#include "item.h"

/**
 * @brief In addition to Item in this class information about velocity and additional sprites can be stored.
 */
class MovingItem : public Item {
public:
        MovingItem();

        /**
         * @brief Convenience constructor for MovingItem objects
         *
         * @param x the desired x offset of the item
         * @param y the desired y offset of the item
         * @param p_type the desired type of the item
         * @param image_string path to the items image
         */
        MovingItem(int x, int y, ITEM_TYP p_type, std::string image_string);

        /**
         * @brief Simple getter for the velocity
         *
         * @return an items velocity
         */
        double vx(){return velX;}
        /**
         * @brief Simple getter for the velocity
         *
         * @return an items velocity
         */
        double vy(){return velY;}

        /**
         * @brief Simple setter for the velocity
         *
         * @param x new desired velocity

         * This will also change the looking left or right attribute accordingly.

         */
        void setVx(double x);
        /**
         * @brief Simple setter for the velocity
         *
         * @param x new desired velocity
         */
        void setVy(double x){velY = x;}



        /**
         * @brief Checks if the player is currently standing
         *
         * @return wether the item is standing 
         */
        bool isStanding(){return standingOnFloor;}

        /**
         * @brief Changes the isstanding attribute
         *
         * @param x wether the item is standing or not
         */
        void setStanding(bool x){standingOnFloor = x;}

        /**
         * @brief Kills an item.

         */
        void kill(){isAlive = false;}


protected:

        bool lookingleft;
        bool isAlive;
        bool standingOnFloor;
        double velX;
        double velY;



};

#endif // MOVINGITEM_H
