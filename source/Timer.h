#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <vector>
#include <string>




/**
 * @brief A class representing a stopwatch.
 */
class Timer {
    private:
        //The clock time when the timer started
        int startTicks;

        //The ticks stored when the timer was paused
        int pausedTicks;

        //The timer status
        bool paused;
        bool started;

    public:
        //Initializes variables
        Timer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        //Gets the timer's time
        int get_ticks();

        //Checks the status of the timer
        bool is_started();
        bool is_paused();

};


#endif // TIMER_H
