#include "editor.h"
#include "map.h"
#include "SDL/SDL_ttf.h"
#include <sstream>


editor::editor() {

    TTF_Init();
    SDL_Color temp_textColor = { 0, 255, 255,0 };
    this->textColor = temp_textColor;
    this->font = TTF_OpenFont( "media/ttf/animeace.ttf", 28 );
    if(this->font==NULL)
        cerr << "ttf_load error \n => Editor Konstruktor ERROR" << endl;
    this->font2 = TTF_OpenFont( "media/ttf/animeace.ttf", 15);
    if(this->font2==NULL)
        cerr << "ttf_load error \n => Editor Konstruktor ERROR" << endl;
}

editor::~editor() {
    //TTF_Quit();

    TTF_CloseFont(font);
    TTF_CloseFont(font2);
    map::freeItems(items);
    map::freeItems(plotted);
}
// put here all sprites which can be plotted:

void editor::load_Items() {

    /*INFO:
All items which you do not want to plot in editor
add to ItemVector "plotted"

All items which you want to plot in editor
add to ItemVector "items"

*/
    //STATIC
    {// Pos 0 immer Oiram!
        std::string tmp = "media/moving/Oiram.png";
        map::createItems(PLAYER_BEGIN_POS_X, PLAYER_BEGIN_POS_Y,
                         tmp,PLAYER,plotted);
    }
    {//Pos 1 immer peach!
        std::string tmp = "media/moving/peach.png";
        map::createItems(500,COMPLETE_WORLD_HEIGTH - 200, tmp,PEACH,plotted);
    }
    {//Pos 2 immer background!
        std::string tmp = "media/world/background_l2.png";
        map::createItems(0,0, tmp,BACKGROUND,plotted);
    }

    {//Pos 3 immer BULLET!
        std::string tmp = "media/attak/bullet.png";
        map::createItems(COMPLETE_WORLD_LENGTH+10,COMPLETE_WORLD_HEIGTH+10, tmp,BULLET,plotted);
    }
    {//Pos 4 immer CLOUD!
        std::string tmp = "media/attak/fight.png";
        map::createItems(COMPLETE_WORLD_LENGTH+10,COMPLETE_WORLD_HEIGTH+10, tmp,FIGHT_CLOUD,plotted);
    }
    {//Pos 5 immer princess bullet!
        std::string tmp = "media/attak/pincess_bullet.png";
        map::createItems(COMPLETE_WORLD_LENGTH+10,COMPLETE_WORLD_HEIGTH+10, tmp,PRINCESS_BULLET,plotted);
    }


    //MAP OBJECTS:
    {
        std::string tmp = "media/world/floor_m.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp = "media/world/floor_long.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/box_100x100.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/box_80x80.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/box_60x60.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/box2_80x80.png";
        map::createItems(0,0, tmp,TRAPSTONE,items);
    }
    {
        std::string tmp= "media/world/box3_80x80.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/floor.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/pillar.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/stone.png";
        map::createItems(0,0, tmp,MAP,items);
    }
    {
        std::string tmp= "media/world/water.png";
        map::createItems(0,0, tmp,WATER,items);
    }
    {
        std::string tmp= "media/world/float_island.png";
        map::createItems(0,0, tmp,FLOATING_ISLAND,items);
    }
    {
        std::string tmp= "media/world/do_not_plot.png";
        map::createItems(0,0, tmp,DO_NOT_PLOT,items);
    }
    {
        std::string tmp= "media/world/goal.png";
        map::createItems(0,0, tmp,GOAL,items);
    }

    //POWERUPS:
    {
        std::string tmp= "media/world/coin.png";
        map::createItems(0,0, tmp,COIN,items);
    }
    {
        std::string tmp= "media/power/star.png";
        map::createItems(0,0, tmp,STAR,items);
    }
    {
        std::string tmp= "media/power/heart.png";
        map::createItems(0,0, tmp,HEART,items);
    }



    //ENEMIES:
    {
        std::string tmp= "media/moving/crab.png";
        map::createItems(0,0, tmp,CRAB,items);
    }
    {
        std::string tmp= "media/moving/flower.png";
        map::createItems(0,0, tmp,FLOWER,items);
    }
    {
        std::string tmp = "media/moving/ghost.png";
        map::createItems(0,0, tmp,GHOST,items);
    }
    {
        std::string tmp = "media/moving/grrr.png";
        map::createItems(0,0, tmp,GRRR,items);
    }
    //Again MAP:
    {
        std::string tmp = "media/world/b80x80.png";
        map::createItems(0,0, tmp,MAP,items);
    }


}

void editor::run()
{

    SDL_Rect camera = {COMPLETE_WORLD_LENGTH-SCREEN_WIDTH, COMPLETE_WORLD_HEIGTH-SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT };
    Timer myfps;
    SDL_WM_SetCaption( "EDITOR", NULL );
    SDL_Surface* message;
    screen = NULL;

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    SDL_Rect itemWindow;
    itemWindow.x = 0; itemWindow.y = 0; itemWindow.w = 200; itemWindow.h = 200;

    load_Items();
    Item* help = new Item(0,0,BACKGROUND,"media/menue/help_editor.png");


    unsigned int itemNum = 0;
    bool RUN_EDITOR = true;

    bool rMousePressed = false;
    bool lMousePressed = false;
    Item* aktItem;

    int MSx=0,MSy=0;
    bool plotHelp = false;

    bool constantPlotMode = false;

    //MAIN LOOP EDITOR
    while(RUN_EDITOR){

        SDL_FillRect(screen,NULL, 0x000000);

        //START PLOTTED ITEMS
        for(unsigned int i = 0;i < plotted.size();i++){
            Item* tI = plotted[i];
            if( (tI->type != BACKGROUND) && (tI->type != PEACH) ){

                SDL_Rect a = tI->getRect();
                a.x -= camera.x;
                a.y -= camera.y;
                SDL_BlitSurface( tI->pictureVec[0], NULL, screen, &a);

            }


        }// END PLOTTED ITEMS

        {//Start ttf

            SDL_GetMouseState(&MSx,&MSy);
            MSx -= MSx%10;
            MSy -= MSy%10;

            {
                SDL_Rect xyPos = {SCREEN_WIDTH-150,10,0,0};
                stringstream tmpstr;
                tmpstr << "x:"<<MSx+camera.x;
                message = TTF_RenderText_Blended(font, tmpstr.str().c_str() , textColor );
                SDL_BlitSurface(message,NULL,screen,&xyPos);
                SDL_FreeSurface(message);
            }

            {
                SDL_Rect xyPos = {SCREEN_WIDTH-150,38,0,0};
                stringstream tmpstr;
                tmpstr << "y:"<<MSy+camera.y;
                message = TTF_RenderText_Blended(font, tmpstr.str().c_str() , textColor );
                SDL_BlitSurface(message,NULL,screen,&xyPos);
                SDL_FreeSurface(message);
            }
            {

                SDL_Rect xyPos = {20,SCREEN_HEIGHT-40,0,0};

                stringstream tmpstr;
                tmpstr << "Press 'h' for help.";
                message = TTF_RenderText_Blended(font, tmpstr.str().c_str() , textColor );
                SDL_BlitSurface(message,NULL,screen,&xyPos);
                SDL_FreeSurface(message);

            }

        }

        {
            //EVAL KEYBOARD STATES:
            Uint8 *keystate = SDL_GetKeyState(NULL);

            //START MOVE CAMERA
            if(keystate [SDLK_RIGHT] && (camera.x + SCREEN_WIDTH < COMPLETE_WORLD_LENGTH) )
                camera.x +=20;
            if(keystate [SDLK_LEFT] && (camera.x > 0) )
                camera.x -=20;
            if(keystate [SDLK_DOWN] && (camera.y + SCREEN_HEIGHT < COMPLETE_WORLD_HEIGTH) )
                camera.y +=20;
            if(keystate [SDLK_UP] && (camera.y > 0) )
                camera.y -=20;
            //END MOVE CAMERA

            //DISP HELP
            if(keystate[SDLK_h]){
                plotHelp = true;
            }
            else
                plotHelp = false;
            //END DISP HELP


        }//END EVAL KEYBOARD STATES


        //Items to choose
        SDL_BlitSurface( items[itemNum]->pictureVec[0], NULL, screen, &itemWindow);

        if(plotHelp == true){
            SDL_BlitSurface( help->pictureVec[0], NULL, screen, NULL);
        }

        SDL_Flip(screen);

        //PLOT ENDEND



       //NOW EVENT HANDLING
        while( SDL_PollEvent( &event ) )
        {
            switch(event.type){

            case SDL_KEYDOWN:{
                switch( event.key.keysym.sym ){
                case SDLK_UP: ;break;
                case SDLK_LEFT:  ;break;
                case SDLK_RIGHT:  ;break;
                case SDLK_n:(itemNum==items.size()-1) ? itemNum=0 : itemNum++;break;
                case SDLK_b:(itemNum==0) ? itemNum = items.size()-1 : itemNum--;break;
                case SDLK_l:{
                    string mapname = map::loadMapMenue();
                    if(mapname != ""){
                        map::freeItems(plotted);
                        map::loadItems(mapname,plotted);
                    }

                }
                    break;


                    //QUIT EDITOR
                case SDLK_ESCAPE:{
                    RUN_EDITOR = false;
                    SDL_FreeSurface(screen);
                    //FREE ITEMS VECTOR


                    //FREE PLOTTED VECTOR
                    map::freeItems(items);
                    map::freeItems(plotted);

                    //TTF_Quit();
                    SDL_WM_SetCaption( TITLE, NULL );
                }
                    break;
                    //END QUIT EDITOR

                case SDLK_RETURN:{
                    string mystring = startInputMapName(screen);
                    string mycountdown = startInputCountdown(screen);
                    cout<<"mapstring "<< mystring << endl;
                    cout<<"mycountdown "<< mycountdown << endl;
                    if (mystring.empty()!= true && mycountdown.empty() != true){

                        //Player time needs to be set to zero at saving the edited map
                        Player* myPlayer =(Player*)plotted[0];
                        myPlayer->displayClock.stop();
                        myPlayer->addTime(-myPlayer->getTime()); //getTime set to zero
                        myPlayer->setCountdown(atoi(mycountdown.c_str()));
                        myPlayer->setHighScore(0);
                        map::saveItems(mystring,plotted);

                        SDL_FillRect(screen,NULL, 0x000000);
                        SDL_Rect pos = {100,SCREEN_HEIGHT/2,0,0};
                        SDL_Surface* temps = TTF_RenderText_Blended(font, "Map sucessfully saved:-)", textColor );
                        SDL_BlitSurface(temps,NULL,screen,&pos);
                        SDL_FreeSurface(temps);
                        SDL_Flip(screen);
                        SDL_Delay(1500);


                        RUN_EDITOR = false;
                        while( SDL_PollEvent( &event ) ){
                            //just for cleaning all keyboard events
                        }
                    }
                }
                    break;



                default :{}break;
                }
            } break;

            case SDL_KEYUP:
            {

                switch( event.key.keysym.sym )
                {
                case SDLK_LEFT:  ; break;
                case SDLK_RIGHT: ; break;
                default : ; break;
                }

            }break;

            case SDL_MOUSEBUTTONDOWN:{

                if( event.button.button == SDL_BUTTON_LEFT )
                {
                    Item* it;
                    for(unsigned int i = 5;i<plotted.size();i++){
                        it = plotted[i];
                        if(it->contains(MSx+camera.x,MSy+camera.y)){
                            aktItem = it;
                            constantPlotMode=true;
                            break;
                        }

                    }

                    if(constantPlotMode == false){
                        Item* ti = items[itemNum];
                        map::createItems(MSx+camera.x,MSy+camera.y,ti->filePath,ti->type,plotted);
                    }
                    lMousePressed = true;

                }
                if( event.button.button == SDL_BUTTON_RIGHT )
                {   //because the first 6 elements are allways needed
                    for(unsigned int i=6;i<plotted.size();i++){
                        Item* it = plotted[i];
                        if(it->contains(MSx+camera.x,MSy+camera.y)){
                            delete plotted[i];
                            plotted.erase(plotted.begin()+i);
                        }

                    }
                    rMousePressed = true;


                }
                if( event.button.button == SDL_BUTTON_WHEELUP )
                {
                    (itemNum==items.size()-1) ? itemNum=0 : itemNum++;
                    //cout << "itemNum: " <<itemNum << endl;
                }
                if( event.button.button == SDL_BUTTON_WHEELDOWN )
                {
                    (itemNum==0) ? itemNum = items.size()-1 : itemNum--;
                    //cout << "itemNum: " <<itemNum << endl;
                }

            } break;

            case SDL_MOUSEBUTTONUP:{
                rMousePressed = false;
                constantPlotMode = false;
                lMousePressed = false;
            }break;

            }

            if(lMousePressed){

                if(constantPlotMode==true){
                    if(MSx+camera.x < aktItem->x()){
                        map::createItems(aktItem->x()-aktItem->w(),aktItem->y(),aktItem->filePath,aktItem->type,plotted);
                        aktItem = plotted[plotted.size()-1];
                    }
                    else if(MSx+camera.x > aktItem->x()+aktItem->w()){
                        map::createItems(aktItem->x()+aktItem->w(),aktItem->y(),aktItem->filePath,aktItem->type,plotted);
                        aktItem = plotted[plotted.size()-1];
                    }
                    else if(MSy+camera.y > aktItem->y()+aktItem->h()){
                        map::createItems(aktItem->x(),aktItem->y()+aktItem->h(),aktItem->filePath,aktItem->type,plotted);
                        aktItem = plotted[plotted.size()-1];
                    }
                    else if(MSy+camera.y < aktItem->y()){
                        map::createItems(aktItem->x(),aktItem->y()-aktItem->h(),aktItem->filePath,aktItem->type,plotted);
                        aktItem = plotted[plotted.size()-1];
                    }


                }
                else{
                    Item *tmp = plotted[plotted.size()-1];
                    tmp->setX(MSx+camera.x);
                    tmp->setY(MSy+camera.y);
                }
            }


            if(rMousePressed){
                Item* it;
                for(unsigned int i = 5;i<plotted.size();i++){
                    it = plotted[i];
                    if(it->contains(MSx+camera.x,MSy+camera.y)){
                        delete plotted[i];
                        plotted.erase(plotted.begin()+i);
                        break;
                    }
                }
            }

        }//End While PollEvent

        editor::cap_frame(60,myfps);

    }
    //ENDE MAIN LOOP

    delete help;

}



void editor::cap_frame(int required_fps, Timer &fps_timer)
{
    if( fps_timer.get_ticks() < 1000 / required_fps )
    {
        SDL_Delay( ( 1000 / required_fps ) - fps_timer.get_ticks() );
    }
    fps_timer.start();
}

string editor::startInputMapName(SDL_Surface *screen)
{
    SDL_Surface* text;
    SDL_Surface* t;
    SDL_Surface* t1;
    t = TTF_RenderText_Blended(font, "Map Name: ", textColor );
    t1 = TTF_RenderText_Blended(font, "Set Countdown Clock [s]: ", textColor );

    SDL_Rect pos = {200,SCREEN_HEIGHT/2,0,0};
    SDL_Rect pos2 = {400,SCREEN_HEIGHT/2,0,0};
    SDL_Rect pos3 = {200,SCREEN_HEIGHT/2+60,0,0};

    //Enable Unicode
    SDL_EnableUNICODE( SDL_ENABLE );

    mapStr = "";
    Timer timer;
    timer.start();
    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){

            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_RETURN && mapStr.empty() != true )
                {

                    running = false;

                }
                if( event.key.keysym.sym == SDLK_ESCAPE)
                {
                    mapStr= "";
                    running = false;
                }
                if( mapStr.length() <= 12 )//max
                {
                    //If the key is a space
                    if( event.key.keysym.unicode == (Uint16)' ' )
                    {
                        mapStr += (char)event.key.keysym.unicode;
                    }
                    //If the key is a number
                    else if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
                    {
                        mapStr += (char)event.key.keysym.unicode;
                    }
                    //If the key is a uppercase letter
                    else if( ( event.key.keysym.unicode >= (Uint16)'A' ) && ( event.key.keysym.unicode <= (Uint16)'Z' ) )
                    {
                        mapStr += (char)event.key.keysym.unicode;
                    }
                    //If the key is a lowercase letter
                    else if( ( event.key.keysym.unicode >= (Uint16)'a' ) && ( event.key.keysym.unicode <= (Uint16)'z' ) )
                    {
                        mapStr += (char)event.key.keysym.unicode;
                    }
                }

                if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( mapStr.length() != 0 ) )
                {
                    mapStr.erase( mapStr.length() - 1 );
                }
            }
        }

        SDL_FillRect(screen,NULL, 0x000000);


        SDL_BlitSurface(t,NULL,screen,&pos);
        SDL_BlitSurface(t1,NULL,screen,&pos3);

        text = TTF_RenderText_Blended( font, mapStr.c_str(), textColor );
        SDL_BlitSurface(text,NULL,screen,&pos2);
        SDL_FreeSurface(text);

        SDL_Flip(screen);
        cap_frame(30,timer);
    }

//    SDL_FreeSurface(text);
//    SDL_FreeSurface(t);
//    SDL_FreeSurface(t1);
    return mapStr;
}

string editor::startInputCountdown(SDL_Surface *screen)
{
    SDL_Surface* text;
    SDL_Surface* text2;
    SDL_Surface* t1;
    t1 = TTF_RenderText_Blended(font, "Set Countdown Clock [s]: ", textColor );
    SDL_Surface* t;
    t = TTF_RenderText_Blended(font, "Map Name: ", textColor );
    SDL_Rect pos = {200,SCREEN_HEIGHT/2,0,0};
    SDL_Rect pos2 = {400,SCREEN_HEIGHT/2,0,0};
    SDL_Rect pos3 = {200,SCREEN_HEIGHT/2+60,0,0};
    SDL_Rect pos4 = {700,SCREEN_HEIGHT/2+60,0,0};


    //Enable Unicode
    SDL_EnableUNICODE( SDL_ENABLE );

    Timer timer;
    mapClock = "";
    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){

            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_RETURN && mapClock.empty() != true )
                {
                    running = false;
                }
                if( event.key.keysym.sym == SDLK_ESCAPE)
                {
                    mapClock = "";
                    running = false;
                }
                if( mapClock.length() <= 5 )//max
                {

                    //If the key is a number
                    if( ( event.key.keysym.unicode >= (Uint16)'0' ) && ( event.key.keysym.unicode <= (Uint16)'9' ) )
                    {
                        mapClock += (char)event.key.keysym.unicode;
                    }

                }

                if( ( event.key.keysym.sym == SDLK_BACKSPACE ) && ( mapClock.length() != 0 ) )
                {
                    mapClock.erase( mapClock.length() - 1 );
                }
            }
        }

        SDL_FillRect(screen,NULL, 0x000000);


        SDL_BlitSurface(t,NULL,screen,&pos);

        SDL_BlitSurface(t1,NULL,screen,&pos3);

        text = TTF_RenderText_Blended( font, mapStr.c_str(), textColor );
        SDL_BlitSurface(text,NULL,screen,&pos2);
        SDL_FreeSurface(text);

        text2 = TTF_RenderText_Blended( font, mapClock.c_str(), textColor );
        SDL_BlitSurface(text2,NULL,screen,&pos4);
        SDL_FreeSurface(text2);

        SDL_Flip(screen);
        cap_frame(30,timer);
    }

//    SDL_FreeSurface(t);
//    SDL_FreeSurface(t1);
//    SDL_FreeSurface(text);
//    SDL_FreeSurface(text2);
    SDL_EnableUNICODE( SDL_DISABLE );
    return mapClock;
}



