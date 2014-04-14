#include "application.h"
#include "editor.h"
using namespace std;

Application::Application() {
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1  )
        cerr << "SDL INIT Error !!! " << endl;
    else if(TTF_Init()== -1 )
        cerr << "TTF INIT Error !!! " << endl;
    SDL_WM_SetCaption(TITLE,NULL);
    this->screen = NULL;
    this->screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    this->loadMainMenue();
    this->quit=false;
    this->aktSelection=0;
}

Application::~Application() {
}

void Application::exec() {
    Timer fps;

    while(!this->quit){
        this->blit_menu();
        /* if the player starts the game, the following call is blocking*/

        editor::cap_frame(10,fps);
        this->handle_menu_input();
    }
}

void Application::blit_menu(){




    SDL_FillRect( this->screen, &this->screen->clip_rect, SDL_MapRGB( this->screen->format, 0xFF, 0xFF, 0xFF ) );
    for(int i = (int)this->menuvector.size()-1; i>=0 ;i--){
        Item* temp = this->menuvector[i];
        SDL_Rect ttt = temp->getRect();
        SDL_BlitSurface(temp->pictureVec[0],NULL,this->screen,&ttt);
    }
     SDL_Rect ttt = this->menuvector[4]->getRect();
    SDL_BlitSurface(this->menuvector[4]->pictureVec[0],NULL,this->screen,&ttt);
    SDL_Flip(this->screen);


}

void Application::handle_menu_input(){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    SDL_Event event;
    while( SDL_PollEvent( &event ) ){
        Item* startbutton=this->menuvector[0];
        Item* quitbutton=this->menuvector[3];
        Item* loadbutton=this->menuvector[1];
        Item* editorbutton=this->menuvector[2];
        if( event.type == SDL_MOUSEBUTTONDOWN ) {
            //If the left mouse button was pressed
            if( event.button.button == SDL_BUTTON_LEFT ) {
                //Get the mouse offsets
                int x = 0, y = 0;
                x = event.button.x;
                y = event.button.y;

                //If the mouse is over the button
                if(startbutton->contains(x,y)){
                    selectLevel(true,"");
                }

                if(loadbutton->contains(x,y)){
                    string tmap = map::loadMapMenue();
                    if(tmap != ""){
                        try{
                            selectLevel(false,tmap);
                        }
                        catch(...){
                            cout<< "error opening map"<<endl;
                            cout<< "Wrong filename?"<<endl;
                        }
                    }
                }
                if(editorbutton->contains(x,y)){
                    editor* edit = new editor;
                    edit->run();
                    delete edit;
                }
                if(quitbutton->contains(x,y))
                    this->quit = true;
            }
        }

           if( event.type == SDL_KEYDOWN) {

           if(  event.key.keysym.sym == SDLK_a ){
            map::show_about();
           }


           if(  event.key.keysym.sym == SDLK_DOWN ){
            Item* sel = (Item*)menuvector[4];
            aktSelection++;
            aktSelection=aktSelection%4;
            Item* tmp = (Item*)menuvector[aktSelection];
            sel->setY(tmp->y());

           }
           if(  event.key.keysym.sym == SDLK_UP ){
           Item* sel = menuvector[4];
           aktSelection--;
           if(aktSelection == -1)
                aktSelection = 3;
           Item* tmp = (Item*)menuvector[aktSelection];
           sel->setY(tmp->y());
           }

           if(  event.key.keysym.sym == SDLK_RETURN ){
            SDL_Event myevent;
            myevent.type = SDL_MOUSEBUTTONDOWN;
            myevent.button.button = SDL_BUTTON_LEFT;
            Item* tmp = (Item*)menuvector[aktSelection];
             int x = tmp->x()+1;
             int y = tmp->y()+1;
             myevent.button.x=x;
             myevent.button.y=y;
             SDL_PushEvent (&myevent);
             cout << "akt selec: "<< aktSelection << endl;

           }
          }
    }
}

void Application::loadMainMenue()
{

    string S_box = "media/menue/start.png";
    string Q_box = "media/menue/quit.png";
    string L_box = "media/menue/load.png";
    string E_box = "media/menue/editor.png";
    string msel_box = "media/menue/msel.png";
    string mbg_box = "media/menue/mbg.png";

    Item* startbox = new Item(412,150,MENU,S_box);
    Item* loadbox = new Item(412,220,MENU,L_box);
    Item* editorbox = new Item(412,290,MENU,E_box);
    Item* quitbox = new Item(412,360,MENU,Q_box);
    Item* msel = new Item(412,150,MENU,msel_box);
    Item* mbg = new Item(0,0,MENU,mbg_box);



    this->menuvector.push_back(startbox);
    this->menuvector.push_back(loadbox);
    this->menuvector.push_back(editorbox);
    this->menuvector.push_back(quitbox);
    this->menuvector.push_back(msel);
    this->menuvector.push_back(mbg);

    cout << "loaded"<< endl;


}


void Application::selectLevel(bool newGame, string tmap){
    int level=0;

    if(newGame==true){
        currentmap = new map("map_1");
        level = this->currentmap->run();
        delete this->currentmap;
    }else{
        map* ctmap = new map(tmap);
        level=ctmap->run();
        delete ctmap;
    }

    if(level == 2){
        currentmap = new map("map_2");
        level = this->currentmap->run();
        delete this->currentmap;
    }
    if(level == 3){
        currentmap = new map("map_3");
        level = this->currentmap->run();
        delete this->currentmap;
    }


}
