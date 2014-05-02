#include "map.h"
#include "application.h"


map::map(string mapname)
{
    mapName = mapname;
    quit = 0;
    env.show_load();

    this->init_sound();
    int loaded = map::loadItems(mapname,this->items);
    if(loaded == 0)
        quit = 1;

    paused = false;
    loadMenueBoxes();
    level_done = 0;



    SDL_Delay(1000);


}

map::~map()
{

    map::freeItems(items);


    //Free the sound effects
    Mix_FreeChunk( jump );
    Mix_FreeChunk( dead );
    Mix_FreeChunk( shoot );
    Mix_FreeChunk( coinCollected );
    Mix_FreeChunk ( extraLife);
    Mix_FreeChunk( fire );
    Mix_FreeChunk( supermode );
    Mix_FreeChunk ( gameover);

    //Free the music
    Mix_FreeMusic( main_theme );

    //Quit SDL_mixer
    Mix_CloseAudio();

    cout << "Sounds cleaned" << endl;

}

int map::run(){

    while(!this->quit){
        fps.start();
        evnt.run(items,this);
        if(this->paused){
            env.show_pause(this);
        }else{
            log.run(items,this); env.show(items,this);
        }
        env.cap_frame(60, fps);

        if(this->level_done){
            Player* myPlayer = (Player*) items[0];
            int nextLevel=0;
            env.show_sucess("THE LEVEL",myPlayer,this);
            nextLevel=getNextLevel(this);
            return nextLevel;

            /*
            if(mapName == "map_1"){
                env.show_sucess("map 1",myPlayer,this);
                nextLevel=getNextLevel();
                return nextLevel;
            }

            else if(mapName == "map_2"){
                env.show_sucess("map 2",myPlayer,this);
                return 2;
            }

            else if(mapName == "map_3"){
                env.show_sucess("map 3",myPlayer,this);
                return 3;
            }

            else{
                env.show_sucess("your own map",myPlayer,this);
                return -1;
            }
            */
        }
    }
    return 0;

}


void map::createItems(int x, int y, string mystr,ITEM_TYP typ, vector<Item*> &itemVec)
{
    string myFilePath = mystr;

    /*-----------------------------------------------------------------------------
     *  We First switch by item type to determine the appropriate action to be taken
     *  then construct the needed object, and before adding it to the item vector
     *  we generate the flipped images that are needed for walking in two directions.
     *-----------------------------------------------------------------------------*/
    switch(typ){
    case PLAYER:{

        Player* tmp = new Player(x,y,typ,myFilePath.c_str());   //PIC0
        myFilePath.insert(myFilePath.length()-4,"1");
        tmp->addPic(myFilePath);                                //PIC1

        myFilePath = mystr;
        myFilePath.insert(myFilePath.length()-4,"2");
        tmp->addPic(myFilePath);                                //PIC2

        SDL_Surface* flipped = map::flip_surface(tmp->pictureVec[0],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped);                      //PIC3

        flipped = map::flip_surface(tmp->pictureVec[1],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped);                      //PIC4

        flipped = map::flip_surface(tmp->pictureVec[2],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped);                      //PIC5

        //Now supermode:


        myFilePath = mystr;
        myFilePath.insert(myFilePath.length()-4,"3");
        tmp->addPic(myFilePath);                                //PIC6
        myFilePath = mystr;
        myFilePath.insert(myFilePath.length()-4,"4");
        tmp->addPic(myFilePath);                                //PIC7
        myFilePath = mystr;
        myFilePath.insert(myFilePath.length()-4,"5");
        tmp->addPic(myFilePath);                                //PIC8

        flipped = map::flip_surface(tmp->pictureVec[6],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped);                     //PIC9

        flipped = map::flip_surface(tmp->pictureVec[7],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped);                     //PIC10

        flipped = map::flip_surface(tmp->pictureVec[8],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped);                     //PIC11

        tmp->filePath = mystr;
        itemVec.push_back((Item*)tmp);
        break;
    }

    case CRAB:{

        MovingItem* tmp = new MovingItem(x,y,typ,myFilePath.c_str());
        myFilePath.insert(myFilePath.length()-4,"1");
        tmp->addPic(myFilePath);

        SDL_Surface* flipped1 = map::flip_surface(tmp->pictureVec[0],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped1);

        SDL_Surface* flipped2 = map::flip_surface(tmp->pictureVec[1],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped2);

        tmp->setVx(-CRAB_SPEED);

        tmp->filePath = mystr;
        itemVec.push_back((Item*)tmp);
        break;
    }

    case COIN:{

        Item* tmp = new Item(x,y,typ,myFilePath.c_str());
        myFilePath.insert(myFilePath.length()-4,"1");
        tmp->addPic(myFilePath);

        tmp->filePath = mystr;
        itemVec.push_back((Item*)tmp);
        break;
    }


    case GHOST:{
        MovingItem* tmp = new MovingItem(x,y,typ,myFilePath.c_str());
        SDL_Surface* flipped1 = map::flip_surface(tmp->pictureVec[0],FLIP_HORIZONTAL);
        tmp->pictureVec.push_back(flipped1);

        tmp->setVy(-GHOST_SPEED);

        tmp->filePath = mystr;
        itemVec.push_back((Item*)tmp);
        break;
    }
    case BULLET:{
        MovingItem* tmp = new MovingItem(x,y,typ,myFilePath.c_str());
        tmp->filePath = mystr;
        itemVec.push_back((Item*)tmp);
        break;
    }
    case PRINCESS_BULLET:{
        MovingItem* tmp = new MovingItem(x,y,typ,myFilePath.c_str());
        tmp->filePath = mystr;
        itemVec.push_back((Item*)tmp);
        break;
    }
    case GRRR:{
        MovingItem* tmp = new MovingItem(x,y,typ,myFilePath.c_str());
        tmp->filePath = mystr;
        tmp->setVy(-GHOST_SPEED);
        itemVec.push_back((Item*)tmp);
        break;
    }


    default: //ONLY NOT MOVING and one Picture needed:
        Item* tmp = new Item(x,y,typ,mystr.c_str());
        tmp->filePath = mystr;
        itemVec.push_back(tmp);
        break;
    }

}

void map::freeItems(vector<Item *> &itemVec)
{
    while(!itemVec.empty()){
        Item* titem = itemVec[itemVec.size()-1];
        //Properly delete the item
        if(titem->type == PLAYER){
            Player* tmp = (Player*)titem;
            delete tmp;
        }
        else if (titem->type == GHOST || titem->type == FLOATING_OBJECT){
            MovingItem* tmp = (MovingItem*)titem;
            delete tmp;
        }
        else{
            delete titem;
        }
        //Decrease the item vector by one Element
        itemVec.pop_back();
    }
}

int map::saveItems(string mapname, vector<Item*> &items)
{
    string tempstr = "media/maps/";
    tempstr += mapname;
    cout<<"saving in path: "<<tempstr << endl;
    ofstream mystream(tempstr.c_str(),ios::out);
    if(!mystream){
        cerr << "error writing to file"<< endl;
        return 0;
    }

    for(unsigned int i = 0;i< items.size();i++){
        Item* tmp = items[i];
        mystream << tmp->type <<";"<< tmp->x() << ";" << tmp->y() << ";"<< tmp->filePath << "#";
    }

    Player* myPlayer = (Player*)(items[0]);

    mystream << "Score" << myPlayer->getScore() <<"!#";
    mystream << "Life" << myPlayer->getLife() << "!#";
    mystream << "Time" << myPlayer->getTime() << "!#";
    mystream << "Countdown" << myPlayer->getCountdown() <<"!#";
    mystream << "HI" << std::setw(10) << std::setfill('0') << myPlayer->getHighScore() << "!#";
    mystream << "NEXT0" << "!#";
    mystream.close();
    return 1;

}

int map::loadItems(string mapname, vector<Item *> &items)
{
    string tempstr = "media/maps/";
    tempstr += mapname;
    ifstream mystream(tempstr.c_str(),ios::in);
    if(!mystream){
        cerr << "error readint from file"<< endl;
        cerr <<"could not open: "<< tempstr << endl;
        return 0;
    }

    ITEM_TYP typ;
    int x;
    int y;
    string filepath;

    stringstream filestring;
    filestring << mystream.rdbuf();
    mystream.close();
    string all = filestring.str();
    int num1=0,num2=0, num3=0, num4=0, num5=0, num6=0,num7=0,num8=0, num9=0, num10=0, num11=0, num12=0;
    while(1){

        num2 = all.find(";",num1);
        if(num2 == -1)
            break;
        typ = (ITEM_TYP)atoi(string(all,num1,num2-num1).c_str() );
        //cout << typ << endl;

        num1 = num2+1;
        num2 = all.find(";",num1);
        x = atoi(string(all,num1,num2-num1).c_str());
        //cout << x << endl;

        num1 = num2+1;
        num2 = all.find(";",num1);
        y = atoi(string(all,num1,num2-num1).c_str());
        //cout << y << endl;

        num1 = num2+1;
        num2 = all.find("#",num1);
        filepath = string(all,num1,num2-num1);
        //cout << filepath << endl;

        num1=num2+1;
        createItems(x,y,filepath,typ,items);
    }

    // load score & lives & time
    int savedScore=0;
    int savedLife=0;
    int savedTime=0;
    int savedCountdown=0;
    int savedHighScore=0;

    num3=all.find("Score",0);
    num4=all.find("!",num3);
    num5=all.find("Life",num3);
    num6=all.find("!",num5);
    num7=all.find("Time",num5);
    num8=all.find("!",num7);
    num9=all.find("Countdown",num8);
    num10=all.find("!",num9);
    num11=all.find("HI",num10);
    num12=all.find("!",num11);

    savedScore=atoi(string(all,num3+5,num4).c_str());
    savedLife=atoi(string(all,num5+4,num6).c_str());
    savedTime=atoi(string(all,num7+4,num8).c_str());
    savedCountdown=atoi(string(all,num9+9,num10).c_str());
    savedHighScore=atoi(string(all,num11+2,num12).c_str());

    if(num3==-1){savedScore=0;}
    if(num5==-1){savedLife=LIVES;}
    if(num7==-1){savedTime=0;}
    if(num9==-1){savedCountdown=300;}
    if(num11==-1) {savedHighScore=0;}

    Player* tmpPlayer = (Player*)items[0];

    tmpPlayer->setScore(false,savedScore);
    tmpPlayer->setLife(savedLife);
    tmpPlayer->addTime(savedTime);
    tmpPlayer->setCountdown(savedCountdown);
    tmpPlayer->setHighScore(savedHighScore);

    return 1;
}

void map::saveWithMenue(vector<Item *> &items) {
    //Initialize SDL
    SDL_Surface* text,*mname,*screen;
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    SDL_Color textColor = { 0, 255, 100, 0 };
    TTF_Font *font = TTF_OpenFont( "media/ttf/animeace.ttf", 28 );
    if(font==NULL)
        cerr << "ttf_load error at saving" << endl;
    SDL_WM_SetCaption( "SaveMenue", NULL );
    SDL_Event event;
    SDL_Rect pos = {200,SCREEN_HEIGHT/2,0,0};
    SDL_Rect pos2 = {400,SCREEN_HEIGHT/2,0,0};
    SDL_EnableUNICODE( SDL_ENABLE );
    string mapStr = "";
    for(bool running = true;running== true;){
        while(SDL_PollEvent(&event)){

            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_RETURN && mapStr.empty() != true )
                {
                    /*-----------------------------------------------------------------------------
                     * If the user presses enter and has entered a map name
                     * save the item vector to a file named mapStr
                     *-----------------------------------------------------------------------------*/
                    map::saveItems(mapStr,items);
                    SDL_FillRect(screen,NULL, 0x000000);
                    mname = TTF_RenderText_Blended(font, "Map sucessfully saved:-)", textColor );
                    SDL_BlitSurface(mname,NULL,screen,&pos);
                    SDL_FreeSurface(mname);
                    SDL_Flip(screen);
                    SDL_Delay(1000);
                    running = false;
                }
                if( event.key.keysym.sym == SDLK_ESCAPE)
                {
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
        //Update the screen
        SDL_FillRect(screen,NULL, 0x000000);
        mname = TTF_RenderText_Blended(font, "Map Name: ", textColor );
        SDL_BlitSurface(mname,NULL,screen,&pos);
        SDL_FreeSurface(mname);
        text = TTF_RenderText_Blended( font, mapStr.c_str(), textColor );
        SDL_BlitSurface(text,NULL,screen,&pos2);
        SDL_FreeSurface(text);
        SDL_Flip(screen);
    }
    // Clear up SDL
    //cout << "clear after saving " <<endl;
    //TTF_CloseFont(font);
    //SDL_FreeSurface(mname);
    //SDL_FreeSurface(text);
    //SDL_FreeSurface(screen);
    SDL_EnableUNICODE( SDL_DISABLE );
    SDL_WM_SetCaption( TITLE, NULL );
}

string map::loadMapMenue() {

    SDL_WM_SetCaption( "LoadMenue", NULL );
    TTF_Init();
    Timer mytimer;
    // Call the OS to output all files in the directory to a text file
#if defined (__WIN32__)
    system("dir media\\maps\\ /b > available_levels.txt");
#else
    system("dir media/maps/  > available_levels.txt");
#endif
    ifstream mystream("available_levels.txt",ios::in);
    if(!mystream){
        cout << "error reading from file \"available levels\"" << endl;
        return 0;
    }
    stringstream filestring;
    filestring << mystream.rdbuf();
    string all = filestring.str();
    istringstream iss(all);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));
    for(unsigned int i =0 ; i < tokens.size();i++){
        std::cout << tokens[i]<<endl;
    }
    //end search mymap


    SDL_Surface* screen = NULL;

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    SDL_Color textColor = { 0, 255, 100,0 };
    TTF_Font *font = TTF_OpenFont( "media/ttf/animeace.ttf", 28 );
    TTF_Font *font_small = TTF_OpenFont( "media/ttf/animeace.ttf", 16 );
    if(font==NULL)
        cout << "ttf_load error at loading" << endl;


    SDL_Event event;

    SDL_Surface* text;
    SDL_Surface* arrow = TTF_RenderText_Blended(font_small, "==> ", textColor );
    int aktText=0;

    SDL_Rect pos2 = {400,SCREEN_HEIGHT-200,0,0};

    //Enable Unicode
    //SDL_EnableUNICODE( SDL_ENABLE );

    //create available map surfaces:
    vector<SDL_Surface*> foundMaps;
    vector<SDL_Rect> mapRects;
    SDL_Surface* temp;


    for(unsigned int i=0;i<tokens.size();i++){
        if(i==14){

            temp = TTF_RenderText_Blended(font_small, "TOO MANY MAPS FOUND", textColor );
            SDL_Rect ttt = {200,100 + i*30,temp->w,temp->h};
            mapRects.push_back(ttt);
            foundMaps.push_back(temp);
            cout<< "too many maps found=>delete in media/maps/... !! "<<endl;
            break;
        }

        temp = TTF_RenderText_Blended(font_small, tokens[i].c_str(), textColor );
        SDL_Rect ttt = {200,100 + i*30,temp->w,temp->h};
        mapRects.push_back(ttt);
        foundMaps.push_back(temp);
    }

    string mapStr = "";
    bool done= false;

    for(bool running = true;running == true;){
        while(SDL_PollEvent(&event)){
            if( event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_DOWN ){
                    aktText++;aktText=aktText%foundMaps.size();
                }
                if(event.key.keysym.sym == SDLK_UP ){
                    aktText--; if(aktText == -1)aktText = foundMaps.size()-1;
                }
                if(event.key.keysym.sym == SDLK_RETURN ){
                    mapStr=tokens[aktText];
                    running=false;
                    done=true;
                }

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

                    for(unsigned int i = 0;i<mapRects.size();i++){
                        //If mous in map name:
                        SDL_Rect trect = mapRects[i];
                        if(x>trect.x && x<trect.x+trect.w && y>trect.y && y<trect.y+trect.h){
                            mapStr = tokens[i];
                            done = true;
                            running = false;
                        }

                    }
                }
            }

            if( event.type == SDL_KEYDOWN )
            {
                if( event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }

            }

        }


        SDL_FillRect(screen,NULL, 0x000000);


        for(unsigned int i=0;i<foundMaps.size();i++){
            if(i==aktText){
                SDL_Rect t1 = mapRects[i];
                t1.x -= arrow->w;
                SDL_BlitSurface(arrow,NULL,screen,&t1);
            }
            SDL_BlitSurface(foundMaps[i],NULL,screen,&mapRects[i]);
        }//TODO FREE SURFACES!!!


        //mname = TTF_RenderText_Blended(font, "Map Name: ", textColor );
        //SDL_BlitSurface(mname,NULL,screen,&pos);

        text = TTF_RenderText_Blended( font, mapStr.c_str(), textColor );
        SDL_BlitSurface(text,NULL,screen,&pos2);
        SDL_FreeSurface(text);

        SDL_Flip(screen);
        editor::cap_frame(10,mytimer);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(font_small);
    //    SDL_FreeSurface(mname);
    //    SDL_FreeSurface(text);
    //    SDL_FreeSurface(screen);
    //SDL_EnableUNICODE( SDL_DISABLE );
    SDL_WM_SetCaption( TITLE, NULL );

    while(!foundMaps.empty()){
        SDL_FreeSurface(foundMaps[foundMaps.size()-1]);
        foundMaps.pop_back();
    }
    SDL_FreeSurface(arrow);
    cout <<"mapstr: "<< mapStr << endl;

    if(done){

        return mapStr;
    }
    else{
        return "";
    }
}

SDL_Surface *map::flip_surface( SDL_Surface *surface, int flags ) {
    SDL_Surface *flipped = NULL;

    if( surface->flags & SDL_SRCCOLORKEY )
    {
        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
    }
    else
    {
        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
    }

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }


    for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
    {
        for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
        {

            Uint32 pixel = map::get_pixel32( surface, x, y );


            if( ( flags & FLIP_VERTICAL ) && ( flags & FLIP_HORIZONTAL ) )
            {
                map::put_pixel32( flipped, rx, ry, pixel );
            }
            else if( flags & FLIP_HORIZONTAL )
            {
                map::put_pixel32( flipped, rx, y, pixel );
            }
            else if( flags & FLIP_VERTICAL )
            {
                map::put_pixel32( flipped, x, ry, pixel );
            }
        }
    }


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }


    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( flipped, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }


    return flipped;
}

void map::init_sound() {

    //Mix_Init(MIX_INIT_OGG);

    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
    main_theme = Mix_LoadMUS( "media/sound/main_theme.wav" );
    if(main_theme ==NULL){
        cout << "main_theme load error" << endl;
    }
    jump = Mix_LoadWAV( "media/sound/jump.wav");
    if(jump ==NULL){
        cout << "jump load error" << endl;
    }
    dead = Mix_LoadWAV( "media/sound/dead.wav");
    if(dead ==NULL){
        cout << "dead load error" << endl;
    }
    shoot = Mix_LoadWAV( "media/sound/shoot.wav");
    if(shoot ==NULL){
        cout << "shoot load error" << endl;
    }
    coinCollected = Mix_LoadWAV("media/sound/coinCollected.wav");
    if(coinCollected ==NULL){
        cout << "coinCollected load error" << endl;
    }
    extraLife=Mix_LoadWAV("media/sound/extralife.wav");
    if(extraLife ==NULL){
        cout << "extraLife load error" << endl;
    }

    fire=Mix_LoadWAV("media/sound/shoot.wav");
    if(extraLife ==NULL){
        cout << "fire load error" << endl;
    }

    supermode=Mix_LoadWAV("media/sound/supermode.wav");
    if(extraLife ==NULL){
        cout << "supermode load error" << endl;
    }

    gameover=Mix_LoadWAV("media/sound/gameover.wav");
    if(extraLife ==NULL){
        cout << "game over load error" << endl;
    }

    Mix_Chunk* tmp_theme = (Mix_Chunk*)main_theme;

    this->soundvector.push_back(tmp_theme);  //0
    this->soundvector.push_back(jump); // 1
    this->soundvector.push_back(dead); // 2
    this->soundvector.push_back(shoot); // 3
    this->soundvector.push_back(coinCollected); // 4
    this->soundvector.push_back(extraLife); // 5
    this->soundvector.push_back(fire); // 6
    this->soundvector.push_back(supermode); // 7
    this->soundvector.push_back(gameover); // 8

    Mix_Music* tmp = (Mix_Music*) soundvector[0];
    Mix_PlayMusic(tmp, 1);
}

Uint32 map::get_pixel32(SDL_Surface *surface, int x, int y)
{
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void map::put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

void map::loadMenueBoxes()
{
    char Q_box[] = "media/menue/quit.png";
    char SA_box[] = "media/menue/save.png";
    char R_box[] = "media/menue/resume.png";
    string msel_box = "media/menue/msel.png";

    this->quitbox = new Item(412,360,MENU,Q_box);
    this->savebox = new Item(412,290,MENU,SA_box);
    this->resumebox = new Item(412,150,MENU,R_box);
    this->sel = new Item(412,150,MENU,msel_box);

}

void map::saveHighScore(map* tmap, Player* myPlayer){
    stringstream highScoreStr;
    string tempstr = "media/maps/";
    tempstr += tmap->mapName;

    fstream fout;
    stringstream filestring;
    int num1=0;

    fout.open(tempstr.c_str(),fstream::binary | fstream::out | fstream::in);
    filestring << fout.rdbuf();
    string all = filestring.str();

    num1=all.find("HI",0);

    fout.seekp(num1+2);

    highScoreStr << std::setw(10) << std::setfill('0') << myPlayer->getHighScore();
    fout.write(highScoreStr.str().c_str(),10);
    fout.close();
}

int map::getNextLevel(map* tmap){
    int num1=0, num2=0, nextLevel=0;
    string tempstr = "media/maps/";
    tempstr += tmap->mapName;
    ifstream mystream(tempstr.c_str(),ios::in);

    stringstream filestring;
    filestring << mystream.rdbuf();
    mystream.close();
    string all = filestring.str();

    num1=all.find("NEXT",0);
    num2=all.find("!",num1);

    nextLevel=atoi(string(all,num1+4,num2).c_str());
    return nextLevel;
}

void map::show_about()
{
    SDL_Surface* screen = NULL;
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    SDL_Color textColor = { 0, 0, 0,0 };
    TTF_Font *font = TTF_OpenFont( "media/ttf/animeace.ttf", 28 );
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xF0, 0xF0, 0xF0 ) );
    SDL_Surface* z1 = TTF_RenderText_Blended(font, "Creators of this marvellous Game:" , textColor );
    SDL_Surface* z1_5 = TTF_RenderText_Blended(font, "______________________________" , textColor );
    SDL_Surface* z2 = TTF_RenderText_Blended(font, "Aydinli Berke" , textColor );
    SDL_Surface* z3 = TTF_RenderText_Blended(font, "Dincer Yigit" , textColor );
    SDL_Surface* z4 = TTF_RenderText_Blended(font, "Wohlfahrt Thomas" , textColor );
    SDL_Surface* z5 = TTF_RenderText_Blended(font, "Wozny Sebastian" , textColor );
    SDL_Surface* z6 = TTF_RenderText_Blended(font, "Have fun and enjoy playing:-)" , textColor );

    SDL_Rect t1 = {200, 100};
    SDL_Rect t1_5 = {200, 120};
    SDL_Rect t2 = {200, 200};
    SDL_Rect t3 = {200, 250};
    SDL_Rect t4 = {200, 300};
    SDL_Rect t5 = {200, 350};
    SDL_Rect t6 = {200, 450};

    SDL_BlitSurface(z1,NULL,screen,&t1);
    SDL_BlitSurface(z1_5,NULL,screen,&t1_5);
    SDL_BlitSurface(z2,NULL,screen,&t2);
    SDL_BlitSurface(z3,NULL,screen,&t3);
    SDL_BlitSurface(z4,NULL,screen,&t4);
    SDL_BlitSurface(z5,NULL,screen,&t5);
    SDL_BlitSurface(z6,NULL,screen,&t6);

    SDL_FreeSurface(z1);
    SDL_FreeSurface(z1_5);
    SDL_FreeSurface(z2);
    SDL_FreeSurface(z3);
    SDL_FreeSurface(z4);
    SDL_FreeSurface(z5);
    SDL_FreeSurface(z6);


    SDL_Flip(screen);

    SDL_Event event;
    bool running = true;
    while(running){
        SDL_Delay(100);
        while( SDL_PollEvent( &event ) ) {
            switch(event.type){
            case SDL_KEYDOWN:running = false; break;
            case SDL_MOUSEBUTTONDOWN: running = false;break;

            }
        }


    }
    SDL_FreeSurface(screen);

}
