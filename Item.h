#ifndef ITEM_H
#define ITEM_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

#include "constants.h"
#include "timer.h"




/**
 * @brief Objects of this class carry all information about static and moving objects and their interactions.
 */
class Item {
public:

    ITEM_TYP type;
/**
 * @brief Constructor which should typically be used to create an Item object
 *
 * @param x x offset at which the Item should appear
 * @param y y offset at which the Item should appear
 * @param p_type the type of Item we are building
 * @param image_string the path to the image that represents the item
 * Convenience constructor that fills in all necessary attributes for the item.
 *
 */
    Item();
    Item(int x, int y, ITEM_TYP p_type, std::string image_string);
/**
 * @brief Destructs an Item
 * Clears all images used by the Item
 */
    ~Item();
    SDL_Rect getRect(void){return hitbox;}

    int x(){return hitbox.x;}
    int y(){return hitbox.y;}
    int w(){return hitbox.w;}
    int h(){return hitbox.h;}
    void setX(int i){ hitbox.x = i;}
    void setY(int i){ hitbox.y = i;}

/**
 * @brief Convenience function to test if a set of x y coordinates is in an Item object's boundaries or not.
 *
 * @param x x offset
 * @param y y offset
 *
 * @return True or false depending on weather the set of coordinates is in or not.
 */
    bool contains(int x, int y);

    void addPic(std::string picName);
    int getNumOfSprites(){return numOfSprites;}
    void setNumOfSprites(int num){numOfSprites = num;}

    int getItemTime(){return ownTimer.get_ticks();}
    std::vector<SDL_Surface*> pictureVec;
    std::string filePath;
    Timer ownTimer;


protected:


    int numOfSprites;

    SDL_Rect hitbox; // Sint16 x,y ; Uint16 w,h ;
    // vector ?

/**
 * @brief Helper function to load an image from a png file and optimize it
 *
 * @param img
 *
 * @return 
 */
    SDL_Surface* loadPNG(std::string img);
    
};

#endif
