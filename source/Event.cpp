#include "event.h"

#include "map.h"

Event::Event()
{
}


void Event::handle_menu(SDL_Event event,map *tmap,vector<Item*> &items){
    Player *myplayer;
    myplayer = (Player*)items[0];
    if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        tmap->paused=false;

    if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN){
        if(tmap->sel->y()==tmap->resumebox->y()){
            tmap->sel->setY(tmap->savebox->y());
        }
        else if(tmap->sel->y()==tmap->savebox->y()){
            tmap->sel->setY(tmap->quitbox->y());
        }
        else if(tmap->sel->y()==tmap->quitbox->y()){
            tmap->sel->setY(tmap->resumebox->y());
        }

    }

    if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP){
        if(tmap->sel->y() == tmap->resumebox->y()){
            tmap->sel->setY(tmap->quitbox->y());
        }
        else if(tmap->sel->y()==tmap->quitbox->y()){
            tmap->sel->setY(tmap->savebox->y());
        }
        else if(tmap->sel->y()==tmap->savebox->y()){
            tmap->sel->setY(tmap->resumebox->y());
        }

    }
    if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN){
        SDL_Event myevent;
        myevent.type = SDL_MOUSEBUTTONDOWN;
        myevent.button.button = SDL_BUTTON_LEFT;
        myevent.button.x=tmap->sel->getRect().x+1;
        myevent.button.y=tmap->sel->getRect().y+1;
        SDL_PushEvent (&myevent);

    }


    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            int x = 0, y = 0;
            x = event.button.x;
            y = event.button.y;
            if(tmap->resumebox->contains(x,y)){
                tmap->paused = false;
                myplayer->displayClock.unpause();
                Mix_ResumeMusic();
            }

            if(tmap->quitbox->contains(x,y))
            {

                tmap->quit= true;
            }


            if(tmap->savebox->contains(x,y))
            {
                map::saveWithMenue(items);
            }


        }
    }

}
int sign(int i){
    if(!i)return 0;
    if((i>0)-1)
        return -1;
    return 1;
}

void Event::run(std::vector<Item*> &items, map *tmap){
    SDL_Event event;
    Player *myplayer;
    myplayer = (Player*)items[0];

    Uint8 *keystates = SDL_GetKeyState( NULL );
    if(keystates [SDLK_LEFT])
        myplayer->setVx(-WALKING_SPEED) ;
    else if(keystates [SDLK_RIGHT])
        myplayer->setVx (WALKING_SPEED) ;
    else if(myplayer->vx()!=0 &&sign(myplayer->vx())*myplayer->vx() < myplayer->friction)
        myplayer->setVx(0);
    else if(myplayer->vx())
        myplayer->setVx(myplayer->vx()-sign(myplayer->vx())*myplayer->friction);

    while( SDL_PollEvent( &event ) ) {
        if((tmap->paused) ) handle_menu(event,tmap,items);
        else{
            switch(event.type){//POLL
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    tmap->paused = true;
                    Mix_PauseMusic();
                    myplayer->displayClock.pause();
                    break;
                case SDLK_SPACE:myplayer->shoot();
                default : break;
                case SDLK_UP:
                    if( myplayer->isStanding()) {
                        myplayer->setVy(-JUMPING_SPEED);
                        Mix_PlayChannel( -1, tmap->soundvector[1], 0 );
                        myplayer->setStanding(false);
                        myplayer->friction=0.5;
                    }
                    break;
                }
                break;
            }//POLL END
        }
    }
}
