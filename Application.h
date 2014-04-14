#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constants.h"
#include "item.h"
#include "timer.h"
#include "map.h"
#include "event.h"
#include "logic.h"
#include "environment.h"
#include "editor.h"



/**
 * @brief The application class is the highest level of abstraction.

 * By calling exec on an application object the visual part of Oiram Repus will be started.
 */
class Application{
    public:
        /**
         * @brief Takes care of initializing SDL and its subsystems.

         * This will also set up the screen properly
         */
        Application();
        ~Application();//TODO ->free?
        /**
         * @brief This call starts the application

         * This can be called in the main function to start the application.
         * as long
         */
        void exec();

    private:

        /**
         * @brief The surface screen is used to display all information to the user.
         */
        SDL_Surface* screen;

        /**
         * @brief Currentmap contains the map currently loaded into memory.

         * This map will be started when the start button is pressed.
         */
        map *currentmap;

        /**
         * @brief A vector pointing to the buttons which are displayed in the start menue.
         */
        std::vector<Item*> menuvector;
        /**
         * @brief Should this ever be set to true, the application will exit.
         */
        bool quit;

        /**
         * @brief This call takes care of handling the user interactions.
         *
         * If the user opts to start a game, this call will be blocking and will
         * take care of the further course of the game.
         * If the user starts the editor, it will pop up.
         */
        void handle_menu_input();
        /**
         * @brief This call takes care of displaying the start menu to the screen
         */
        void blit_menu();
        /**
         * @brief This is a helping call to initialize the items
         * needed to build the start menue.
         */
        void loadMainMenue();

        void selectLevel(bool newGame,string tmap);

        int aktSelection;

};
#endif // APPLICATION_H
