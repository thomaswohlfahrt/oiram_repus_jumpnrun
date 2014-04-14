#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include <vector>
#include <string>
#include "item.h"
#include "movingItem.h"
#include "player.h"
#include "constants.h"
#include "timer.h"
#include <sstream>
#include <iomanip>

class map;

using namespace std;


/**
 * @brief A class responisble for drawing the landscape and Item objects.
 **/
class Environment
{
public:

    /**
    @brief Environment constructor
    The font and font colors is set.
    The main graphical variable screen is initialized.
    The dimensions of the camera is set according to the dimensions of the screen
    **/
    Environment();

    /**
    @brief The main function of the class Environment
    @param &items The vector that contains all the Item objects of the current level.
    @param tmap The map that is going to be shown.
    Takes care of all graphical operations
    1) Finds out the items, which should be blitted seperate from other normal objects.
        (Background, Player, Peach)
    2) Sets the camera, that follows the player.
    3) Shows the background surface at first.
    4) Displays all other standard objects (blocks, enemies etc.) with a 'for' loop.
    5) Blits the player with his current position and animation frame.
    6) Blits the princess at the right hand side of the screen. ('Follower effect').
    7) Shows the score.
    8) Refreshes the screen at the end (before the next frame).
    **/
    void show(vector<Item*> &items,map *tmap);

    /**
     * @brief Shows the pause menu.
     **/
    void show_pause(map *tmap);

    /**
     * @brief own function for limiting frames
       - because actions like moving camera are fps based.
     **/
    void cap_frame(int required_fps, Timer fps_timer);

    /**
     * @brief This function is responsible for the summary page when the level is finished.
     **/
    void show_sucess(string levelNum, Player *myPlayer, map* tmap);

    /**
     * @brief This function is responsible for the main menu page when the game is started.
     **/
    void show_load();


private:
    /**
     * @brief shows the effect when shot
     **/
    void apply_attak_surface(vector<Item*> &items);
    /**
     * @brief shows the background in first place
     **/
    void apply_bg_surface(Item *bg);
    /**
     * @brief Blits the player with his current position and animation frame.
     **/
    void apply_player_surface(Player *player);
    /**
     * @brief blits the coins according to their positions, before they are taken
     **/
    void apply_coin_surfaces(Item* tmpItem);
    /**
     * @brief blits the powerups according to their positions, before they are taken
     **/
    void apply_powerup_surfaces(Item* tmpItem);
    /**
     * @brief blits the moving items according to their current positions and animation frames
     **/
    void apply_moving_surfaces(vector<Item*> &items);
    /**
     * @brief blits the princess at the right hand side of the screen. ('Follower effect')
     **/
    void apply_peach_surface(Item *peach);
    void apply_map_surfaces(vector<Item*> &items);
    /**
     * @brief shows the current score
     **/
    void apply_show_score(Player *player);
    /**
     * @brief sets the camera, that follows the player
     **/
    void set_camera(Player* target);
    /**
     * @brief refreshes the screen at the end (before the next frame)
     **/
    void refresh_screen();
    /**
     * @brief clears the memory after the main game loop
     **/
    void clear_surfaces(vector<Item*> &items);
    /**
     * @brief shows the game HUD
     **/
    void loadDisplay();

    //Attributes:
     map* mapPointer;
     SDL_Rect camera;
     SDL_Surface *screen;
     int counter;
     SDL_Color textColor;
     SDL_Color textColor2;
     SDL_Color textColor3;
     TTF_Font *font;
     SDL_Surface* scoreText;
     SDL_Surface* lifeText;
     SDL_Surface* timeText;
     SDL_Surface* highScoreText;
     std::vector<Item*> displayvector;

     SDL_Surface* gameMessage;
     Timer gameMessageTimer;


};

#endif // ENVIRONMENT_H
