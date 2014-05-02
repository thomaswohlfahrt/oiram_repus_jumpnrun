#ifndef CONSTANTS_H
#define CONSTANTS_H
/**
 * @file All constants used throughout the application are defined here.
**/ 
#include <iostream>
#include <string>

const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 600;
const int COMPLETE_WORLD_LENGTH = 30000;
const int COMPLETE_WORLD_HEIGTH = 2000;
const int PLAYER_BEGIN_POS_X = 29476;
const int PLAYER_BEGIN_POS_Y = 1800;
const int SCREEN_BPP    = 32;
const double WALKING_SPEED = 8;
const double JUMPING_SPEED = 20;
const double GRAVITY = 0.8;
const double GHOST_SPEED = 2;
const double CRAB_SPEED = 1;
const int BULLET_SPEED = 15;
const int BULLET_LIVETIME = 450;
const double PRINCESS_BULLET_SPEED = 4.4;
const int PRINCESS_BULLET_INTERVALL = 15000; //all 15 sec
const int PLAYER_BULLET_INTERVALL = 500;
const int TIME_BONUS=5000;


/**
 * The title that should appear in the window headerbar.
 */
const char TITLE[] = "BestGameEver";
const int FLIP_VERTICAL = 1;
const int FLIP_HORIZONTAL = 2;

/**
 * @brief Time the player will be invulnerable after spawning
 */
const int INVULNERABLE_TIME = 3000;

/**
 * @brief Player starting lives
 */
const int LIVES = 2;


/**
 * @brief Helper enum to distinguish between different object types
 */
enum ITEM_TYP {PLAYER, ENEMY, GHOST, FLOATING_OBJECT,
                MAP, BACKGROUND, MENU, COIN,
                PEACH, CRAB, FLOWER,
                STAR, HEART, DISPLAY,
                WATER, DO_NOT_PLOT,
                FLOATING_ISLAND, GOAL,GRRR,BULLET,
                FIGHT_CLOUD,PRINCESS_BULLET,TRAPSTONE     }; //...usw


#endif // CONSTANTS_H
