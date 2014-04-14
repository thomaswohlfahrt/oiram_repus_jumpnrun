#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "item.h"
#include "movingItem.h"
#include "player.h"
#include "event.h"
#include "logic.h"
#include "environment.h"
#include "timer.h"


using namespace std;
class Application;

/**
 * @brief A class resembling one playable level. 

 * The map class is another central layer of abstraction in Oiram Repus.
 * After a map object is created its parent should call the run() method
 * on it to let it handle all of the following player interactions.
 */
class map
{
    public:
        /**
         * @brief Constructs an itemvector from a file.
         *
         * @param mapname The file that should be loaded into memory
         *
         * This is the base for loading a level into memory.
         * Since a level is constructed by a series of Item objects
         * the main purpose of this constructor is to deserialize the 
         * contents of the file specified by mapname into a vector of 
         * items which will be accessible via this->items.
         */
        map(string mapname);
        /**
         * @brief Clears a level from memory.
         *
         * The Destructor frees memory by deleting all items from this->items
         * and then freeing all sound-resources allocated through SDL.
         */
        ~map();
        /**
         * @brief Method to be run by other objects (Like Application) to start a level.
         *
         * @return Integer representing the outcome of the level.
         *
         * Run is the public method that transmits control over the screen to a map object 
         * and encapsulates all functionality needed to play through one stage of 
         * Oiram Repus.
         * Upon return run() returns an integer indicating wether the player has successfully 
         * finished the level, has aborted the level or has died fighting the hordes of monsters.
         */
        int run();
        /**
         * @brief constructs an item and puts it into an item vector.
         *
         * @param x x position where the item should appear on the map
         * @param y y position where the item should appear on the map.
         * @param mystr path specifying the sprite that should be used.
         * @param typ type of the item to be constructed.
         * @param itemVec reference to the item vector that should be filled.
         */
        static void createItems(int x, int y, string mystr,ITEM_TYP typ, vector<Item*> &itemVec);
        /**
         * @brief empties a given Item vector and frees all memory occupied by it.
         *
         * @param itemVec the Item vector that should be emptied.
         *
         */
        static void freeItems(vector<Item*> &itemVec);
        /**
         * @brief Serializes a given Item vector to a file on the disc.
         *
         * @param mapname Name of the file the vector should be serialized into
         * @param items The item vector to be serialized
         *
         * @return 0 upon errors, 1 for success 
         */
        static int saveItems(string mapname,vector<Item *> &items);
        /**
         * @brief Deserializes a file and fills a vector of Item objects.
         *
         * @param mapname Filename that should be read from.
         * @param items Target vector
         *
         * @return 0 for errors, 1 for success.
         */
        static int loadItems(string mapname,vector<Item *> &items);
        /**
         * @brief Let's the user save a vector of Item objects using a GUI.
         *
         * @param items The target vector
         *
         * @return 
         */
        static void saveWithMenue(vector<Item*> &items);
        /**
         * @brief Displays the menue that allows users to load a level.
         *
         * @return the name of the map to be loaded
         * This method takes control of the screen.
         * It searches for all available maps in the folder media/maps,
         * and then prints them onto the screen so the user can choose 
         * a map to load by clicking on it.
         */
        static string loadMapMenue();

        /**
         * @brief Helper function to flips images.
         *
         * @param surface Image to flip
         * @param flags Flags (FLIP_HORIZONTAL, FLIP_VERTICAL)
         *
         * @return The flipped image
         *
         * A helper function that will flip a image horizontally or vertically.
         * This is to reduce the amount of seperate image files needed for some
         * objects.
         */
        static SDL_Surface *flip_surface( SDL_Surface *surface, int flags );
        /**
         * @brief Helper function to access one pixel of an image as a 32 bit value
         *
         * @param surface image
         * @param x x offset
         * @param y y offset
         *
         * @return the 32 bit valued pixel (RRGGBBAA)
         */
        static Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
        /**
         * @brief Helper function to write a pixel to an image
         *
         * @param surface The target image
         * @param x x offset
         * @param y y offset
         * @param pixel a 32 bit pixel value. (RRGGBBAA)
         */
        static void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );


        /**
         * @brief A boolean to indicate wether the game is paused.
         */
        bool paused;
        /**
         * @brief A boolean to indicate wether the user has opted to quit the game.
         */
        int quit;
        std::vector<Mix_Chunk*> soundvector;
        std::vector<Item*> menuvector;

        /**
         * @brief The Quit Item object for the pause menu
         */
        Item* quitbox;
        /**
         * @brief The Save Item object for the pause menu
         */
        Item* savebox ;
        /**
         * @brief The resume Item object for the pause menu
         */
        Item* resumebox;
        Item* sel;
        bool level_done;
        void saveHighScore(map* tmap, Player* myPlayer);

        static void show_about();


    private:
        /**
         * @brief The member responsible for drawing Items to the screen
         */
        Environment env;
        /**
         * @brief The member responsible for handling game logic like moving and collisions
         */
        Logic log;
        /**
         * @brief The member responsible for handling user interactions.
         */
        Event evnt;

        /**
         * @brief A timer to limit the frame rate
         */
        Timer fps;

        /**
         * @brief A vector that contains all the Item objects of the current level.
         */
        vector<Item*> items;
        /**
         * @brief Loads the images needed to display the pause menue.
         */
        void loadMenueBoxes();

        /**
         * @brief Initializes the SDL mixer libraries
         */
        void init_sound();
        int getNextLevel(map* tmap);

        Mix_Music *main_theme;
        Mix_Chunk *jump;
        Mix_Chunk *dead;
        Mix_Chunk *shoot;
        Mix_Chunk *coinCollected;
        Mix_Chunk *extraLife;
        Mix_Chunk *fire;
        Mix_Chunk *gameover;
        Mix_Chunk *supermode;

        string mapName;
};
#endif // MAP_H
