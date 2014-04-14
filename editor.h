#ifndef EDITOR_H
#define EDITOR_H

#include "SDL/SDL.h"

#include "item.h"
#include "movingItem.h"
#include "player.h"
#include "timer.h"
#include "constants.h"

#include <vector>

class Application;

using namespace std;

/**

  @brief A class for creating new maps and editing existing ones with the mouse

  This is an almost standalone class, which has its own event handling and
  own blitting methods.
  It only relies on proper initialization of the SDL.
  In this project this already is done in Application ( SDL_INIT(SDL_INIT_EVERYTHING) )
  Additionally it uses some static funktions of the map class:
    -map::createItem(...)
    -map::loadItems(...)
    -map::saveItems(...)

  HOW TO USE:

    1)  When starting the editor, the camera is in the bottom-right corner of the complete map.
        You also can see Oriam on his starting position.

    2)  Select different Items by scrolling with your mouse of with the keys "n" or "b", then
        just place them on the screen with your left mouse button.

    3)  If you don't release your button immediately, the Item will be placed in a 10 pxl Raster,
        which makes it much easier to place Objects in a nice manner.

    4)  Don't forget the "Goal" Item - otherwhise you cannot complete the level

    5)  Press "Return" for saving, enter the mapname, again "Return", then the map Time and again "return.
        Your map will be saved.

    6)  If you want to reedit a map just press "l" on the keyboard and select the map with your mouse.

    7)  You can erase all plotted Item objects by clicking on them with your right mouse button.

    8)  For you orientation, on the top-right of the screen, you can see the actual coordinates of your mouse.

**/



class editor
{
public:

    /**
    @brief Constructor Editor.
    Makes shure all the Fonts are loaded, with a defined colour,
    necessaray for the texts in the Editor
    **/
    editor();

    /**
    @brief Destructor Editor.
    Makes shure all the Fonts are released and not anymore in the memory.
    It will also release all the plotted and loaded Items with their sprites,
    because all the information is in the map file after saving.
    **/
    ~editor();

    /**
    @brief Main method of the Editor.
    - The loop will only be left if "ESC" is pressed, or the map was saved.
    - You will get an message if it was successful.
    - It is possible to overwrite all your own maps, so be careful!
    - you cannot overwrite the kampagne maps, because they have an "_" in their name
    **/
    void run();

    /**
    @brief own funktion for limiting frames
    - because actions like moving camera are fps based
   @param fps_timer You shoud create a Timer object outside of the main loop
                    then just hand it over to this method once in your loop
    **/
    static void cap_frame(int required_fps, Timer &fps_timer);

private:

    /**
    @brief all possible Items for the Game are Loaded

    -Every Item needed for the Game has to be listed here
    with its correct relative Path
    -The method fills the Editor Attributes vector "items" and "plotted"
    with the correct Items

    **/
    void load_Items();

    /**
    @brief The Editor has its own Event Handling
    **/
    SDL_Event event;

        /**
    @brief The attribute "screen"  the SDL_Surface which can be seen in the window
    **/
    SDL_Surface* screen;

    /**
    @brief The attribute "camera"  is responsible for moving around in the map via keyboard
    **/
    SDL_Rect camera;

    /**
    @brief All available items for plotting are stored in items
    **/
    vector<Item*> items;

        /**
    @brief All plotted items are stored here, plus some allways needed ones (e.g. Oiram + Princess)
    **/
    vector<Item*> plotted;


    /**
    @brief Method for getting the Mapname from User
    - Implemented with a seperate Loop where only the
    appropriate keys are considered, if one wants to enter a mapname
    for saving
    **/
    string startInputMapName(SDL_Surface *screen);

    /**
    @brief Method for getting the Map Time from User
    - Only Numbers are allowed and Return for confirmation
    **/
    string startInputCountdown(SDL_Surface *screen);

    /**
    @brief Font for displaying screen information like X/Y-Pos etc.
    - set in the constructor
    **/
    TTF_Font *font;

    /**
    @brief Bigger Font for Loading/Saving
    - set in the constructor
    **/
    TTF_Font *font2;

    /**
    @brief Color for the font (here: mixture of blue - green)
    - set in the constructor
    **/
    SDL_Color textColor;

    /**
    @brief string storage for the name of the map you want to save
    - set in the save-menue
    **/
    string mapStr;

    /**
    @brief string storage for the level time in your saved map
    - set in the save-menue
    **/
    string mapClock;

};
#endif // EDITOR_H
