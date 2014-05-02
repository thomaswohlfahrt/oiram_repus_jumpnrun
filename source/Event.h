#ifndef EVENT_H
#define EVENT_H

#include "item.h"
#include "movingItem.h"
#include "player.h"

#include "constants.h"
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

class map;

/**
 * @brief A class that deals with user inputs while playing.

 * //TODO
 */
class Event {

public:
    Event();

    /**
     * @brief This method checks for pressed keys and reacts to keystrokes.
     *
     * @param items the map's vector of Items
     * @param tmap the self
     
     * This method will trigger the following events:
     * Right key pressed: Move right
     * Left key pressed : Move left
     * Up key pressed when player is standing: Jump
     * Space bar pressed: Shoot a bullet
     * Escape: Show the pause menu or exit the level
     */ 
    void run(std::vector<Item*> &items,map *tmap);
private:

    
    /**
     * @brief method that handles keystrokes during the pause menu
     *
     * @param event the event that should be handled
     * @param tmap pointer to the level
     * @param items pointer to the items in the level

     * This method will trigger the following events:
     * Escape : quit the level
     * Mouseclicks: Take care of the buttons clicked.
     */
void   handle_menu(SDL_Event event,map *tmap, std::vector<Item *> &items);

};

#endif // EVENT_H
