#include "environment.h"
#include "map.h"





Environment::Environment()
{

    counter=0;
    SDL_Color temp_textColor = { 34,139,34,0 };
    SDL_Color temp_textColor2 = { 0,100,0,0 };
    SDL_Color temp_textColor3 = { 255,0,0,0 };
    textColor = temp_textColor;
    textColor2 = temp_textColor2;
    textColor3 = temp_textColor3;

    font = TTF_OpenFont( "media/ttf/Balker.ttf", 60 );
    if(font==NULL)
        cout << "TTF_OPEN ERROR: " << "media/ttf/Balker.ttf" << endl;

    camera.w = SCREEN_WIDTH;
    camera.h = SCREEN_HEIGHT;

    screen = NULL;
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Fill the screen (white)
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );


    loadDisplay();

}


void Environment::show(vector<Item*> &items, map *tmap){
    SDL_FillRect(screen,NULL, 0x000000);


    //The 3 constant Items:
    Player* temp_player = (Player*)(items[0]);
    Item* temp_peach = items[1];
    Item* temp_bg = items[2];


    set_camera(temp_player);


    //  1) TODO-> moving bg with player
    apply_bg_surface(temp_bg);

    //  2)  ALL OBJECTS except PEACH and PLAYER
    for(unsigned int i = 2; i< items.size() ;i++){
        Item* titem = items[i];

        if(titem->type==MAP ||
                titem->type==GHOST ||
                titem->type == FLOATING_OBJECT ||
                titem->type == WATER ||
                titem->type == GOAL ||
                titem->type == GRRR ||
                titem->type == BULLET ){

            SDL_Rect ttt = titem->getRect();
            ttt.x -= camera.x;
            ttt.y -= camera.y;

            SDL_BlitSurface( titem->pictureVec[0], NULL, screen, &ttt);
        }



        if(titem->type == CRAB ){//TODO change sprites

            SDL_Rect ttt = titem->getRect();
            ttt.x -= camera.x;
            ttt.y -= camera.y;
            MovingItem* tmpcrab=(MovingItem*)titem;



            if(tmpcrab->ownTimer.get_ticks()>150){
                tmpcrab->ownTimer.start();

                if(tmpcrab->vx() <= 0)
                {
                    tmpcrab->setNumOfSprites((tmpcrab->getNumOfSprites()) + 1);

                    if((tmpcrab->getNumOfSprites()) >1)
                    {
                        tmpcrab->setNumOfSprites(0);
                    }

                }
                if(tmpcrab->vx() > 0){
                    tmpcrab->setNumOfSprites((tmpcrab->getNumOfSprites()) + 1);

                    if((tmpcrab->getNumOfSprites()) >3)
                    {
                        tmpcrab->setNumOfSprites(2);
                    }

                }
            }


            SDL_BlitSurface( titem->pictureVec[tmpcrab->getNumOfSprites()], NULL, screen, &ttt);

        }


        if(titem->type == FLOWER){ //SIMILAR LIKE CRAB OR COIN
            SDL_Rect ttt = titem->getRect();
            ttt.x -= camera.x;
            ttt.y -= camera.y;
            SDL_BlitSurface( titem->pictureVec[0], NULL, screen, &ttt);
        }

        if(titem->type == COIN){
            apply_coin_surfaces(titem);
        }

        if(titem->type == HEART || titem->type == STAR){
            apply_powerup_surfaces(titem);
        }
    }

    //  3)  playdR:
    apply_player_surface(temp_player);

    //  4)   Peach:
    apply_peach_surface(temp_peach);

    apply_attak_surface(items);

    //  5)  Show Score
    apply_show_score(temp_player);


    SDL_Flip( screen );

}


//-----------------------------------FUNCTIONS IN SHOW()------------------------------------------------------//

//exceptional blitting - background
void Environment::apply_bg_surface(Item *bg){
    int bgwidth = bg->getRect().w;
    int offset=0;
    for(int i = 0;i<COMPLETE_WORLD_LENGTH/bgwidth +1;i++){
        offset = camera.x/1.25+i*bgwidth;
        if(offset >= camera.x-bgwidth){
            offset -= camera.x;
            break;
        }
    }
    SDL_Rect pos1 = { offset, 0, 0, 0};
    SDL_Rect pos2 = { offset+bgwidth, 0, 0, 0};
    //   cout << "bg1.x" << offset << endl;
    //   cout << "cam.x" << camera.x << endl;

    SDL_BlitSurface( bg->pictureVec[0], NULL, screen, &pos1 );
    SDL_BlitSurface( bg->pictureVec[0], NULL, screen, &pos2 );
}

//exceptional blitting - player (with camera)
void Environment::apply_player_surface(Player *player){
    SDL_Rect offsetpl;
    static unsigned int invulCounter=0;
    static bool look = true;

    offsetpl.x = player->getRect().x - camera.x;
    offsetpl.y = player->getRect().y - camera.y;



    //set the spriteframe
    bool plot;
    if(look != player->looksLeft()){
        look = player->looksLeft();
        plot = true;
    }
    else{
        plot = false;
    }

    if((player->vx() < 0 && invulCounter%5==0) || (player->vx() < 0 && plot))
    {
        player->setNumOfSprites((player->getNumOfSprites()) + 1);

        if((player->getNumOfSprites()) >1)
        {
            player->setNumOfSprites(0);
        }
        if(player->isStanding()==false){
            player->setNumOfSprites(2);
        }

    }


    if((player->vx() > 0 && invulCounter%5==0) || (player->vx() > 0 && plot))
    {
        player->setNumOfSprites(player->getNumOfSprites() + 1);

        if( (player->getNumOfSprites() >4) || (player->getNumOfSprites() < 3))
        {
            player->setNumOfSprites(3);
        }

        if(player->isStanding()==false){
            player->setNumOfSprites(5);
        }
    }

    if(player->vx()==0){
        int gnos = player->getNumOfSprites();

        if(player->isStanding()==true){
            if(gnos <=2){
                player->setNumOfSprites(0);
            }
            else{
                player->setNumOfSprites(3);
            }
        }
        else{
            if(gnos <=2){
                player->setNumOfSprites(2);
            }
            else{
                player->setNumOfSprites(5);
            }
        }
    }

    //zur auswahl der sprites in SuperMode:-)
    int superFlag;
    if(player->isInSuperMode()==true)
        superFlag= 6 ;
    else
        superFlag = 0;



    if(player->invulnerable()==false )
        SDL_BlitSurface( player->pictureVec[player->getNumOfSprites()+superFlag], NULL, screen, &offsetpl );

    if(player->invulnerable()==true && invulCounter%5 ==0 )
        SDL_BlitSurface( player->pictureVec[player->getNumOfSprites()+superFlag], NULL, screen, &offsetpl );


    invulCounter++;
}

//exceptional blitting - coins
void Environment::apply_coin_surfaces(Item* tmpItem){

    counter++;
    if(counter>12)
    {
        tmpItem->setNumOfSprites((tmpItem->getNumOfSprites())+1);


        if(tmpItem->getNumOfSprites() >1){
            tmpItem->setNumOfSprites(0);}

        counter=0;
    }


    SDL_Rect ttt = tmpItem->getRect();
    ttt.x -= camera.x;
    ttt.y -= camera.y;
    SDL_BlitSurface( tmpItem->pictureVec[tmpItem->getNumOfSprites()], NULL, screen, &ttt);


}

void Environment::apply_powerup_surfaces(Item* tmpItem){

    SDL_Rect ttt = tmpItem->getRect();
    ttt.x -= camera.x;
    ttt.y -= camera.y;
    SDL_BlitSurface( tmpItem->pictureVec[0], NULL, screen, &ttt);

}

//responsible for blitting other surfaces TODO
void Environment::apply_moving_surfaces(vector<Item*> &items){
    unsigned int ind;

    //blit other items
    for(ind=0; ind<items.size(); ind++)
    {
        //if((items[ind]->type != BACKGROUND) && (items[ind]->type != PLAYER) && (items[ind]->isVisible() == true) && items[ind]->type != COIN)

        if( (items[ind]->type==GHOST) || (items[ind]->type==ENEMY) || (items[ind]->type==FLOATING_OBJECT) )
        {
            SDL_Rect ttt = items[ind]->getRect();
            ttt.x -= camera.x;
            ttt.y -= camera.y;
            SDL_BlitSurface( items[ind]->pictureVec[0], NULL, screen, &ttt);
        }
    }


}

//blitting princess
void Environment::apply_peach_surface(Item *peach){
    peach->setX(camera.x + SCREEN_WIDTH-peach->w());
    peach->setY(camera.y+150);

    SDL_Rect ttt = peach->getRect();
    //    tt.x = SCREEN_WIDTH - peach->getRect().w;
    //    tt.y = 150;

    ttt.x -= camera.x;
    ttt.y -= camera.y;

    SDL_BlitSurface( peach->pictureVec[0], NULL, screen, &ttt );
}

//sets the camera that follows the input item (default: player)
void Environment::set_camera(Player *target){



    //Center the camera over the dot
    camera.x = ( target->getRect().x + target->getRect().w / 2 ) - SCREEN_WIDTH / 2;
    camera.y = ( target->getRect().y + target->getRect().h / 2 ) - (SCREEN_HEIGHT / 2-30);

    //Keep the camera in bounds.
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > COMPLETE_WORLD_LENGTH- camera.w )
    {
        camera.x = COMPLETE_WORLD_LENGTH - camera.w;
    }
    if( camera.y > COMPLETE_WORLD_HEIGTH - camera.h )
    {
        camera.y = COMPLETE_WORLD_HEIGTH - camera.h;
    }
}

//refresh the screen after blitting
void Environment::refresh_screen(){

    SDL_Flip( screen );
}



void Environment::cap_frame(int required_fps, Timer fps_timer){
    if( fps_timer.get_ticks() < 1000 / required_fps )
    {
        SDL_Delay( ( 1000 / required_fps ) - fps_timer.get_ticks() );
    }
}

void Environment::show_sucess(string levelNum, Player *myPlayer, map* tmap)
{
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );

    bool newHighScore=false;
    mapPointer=tmap;

//    string win = "You completed ";
//    win += levelNum;

    // Convert : int Score --> string scorestr
    int score = myPlayer->getScore();
    stringstream scorestr;
    scorestr << score;

    int lives = myPlayer->getLife();
    int lifePoints = lives*500;
    stringstream livesStr;
    stringstream lifeScoreStr;
    livesStr << lives;
    lifeScoreStr << lifePoints;

    int playTime=myPlayer->getTime()/1000;
    int countdown=myPlayer->getCountdown();
    int timePoints=0;

    if(playTime>=countdown)
        timePoints=0;
    else
        timePoints=(1.0-(double)playTime/(double)countdown)*TIME_BONUS;

    stringstream playTimeStr;
    stringstream timeScoreStr;
    playTimeStr << std::setw(2) << std::setfill('0') << (playTime/60) << ":" <<std::setw(2) << std::setfill('0')<< (playTime%60);
    timeScoreStr << timePoints;

    int totalScore=score+timePoints+lifePoints;
    stringstream totalScoreStr;
    totalScoreStr << totalScore;

    // this part is responsible for saving the users highscore into the map file.
    stringstream savedHighScoreStr;
    int savedHighScore=myPlayer->getHighScore();
    int playerScore=totalScore;
    if(playerScore>savedHighScore){
        newHighScore=true;
        myPlayer->setHighScore(playerScore);
        tmap->saveHighScore(tmap,myPlayer);
    }
    savedHighScoreStr << savedHighScore;

//    SDL_Surface * tmpMessage1 = TTF_RenderText_Blended(font, "CONGRATULATIONS!!!" , textColor );
    SDL_Surface * tmpMessage1 = TTF_RenderText_Blended(font, "HIGH SCORE " , textColor );
    SDL_Surface * tmpMessage2 = TTF_RenderText_Blended(font, savedHighScoreStr.str().c_str() , textColor );
    SDL_Surface * tmpMessage3 = TTF_RenderText_Blended(font, "LEVEL SUMMARY" , textColor );
    SDL_Surface * tmpMessage4 = TTF_RenderText_Blended(font, "Score: ", textColor );
    SDL_Surface * tmpMessage5 = TTF_RenderText_Blended(font,  scorestr.str().c_str(), textColor );
    SDL_Surface * tmpMessage6 = TTF_RenderText_Blended(font, "Lives  : ", textColor );
    SDL_Surface * tmpMessage7 = TTF_RenderText_Blended(font,  livesStr.str().c_str(), textColor );
    SDL_Surface * tmpMessage8 = TTF_RenderText_Blended(font, "Time   : ", textColor );
    SDL_Surface * tmpMessage9 = TTF_RenderText_Blended(font,  playTimeStr.str().c_str(), textColor );
    SDL_Surface * tmpMessage10 = TTF_RenderText_Blended(font,  lifeScoreStr.str().c_str(), textColor );
    SDL_Surface * tmpMessage11 = TTF_RenderText_Blended(font, timeScoreStr.str().c_str() , textColor );
    SDL_Surface * tmpMessage12 = TTF_RenderText_Blended(font,  totalScoreStr.str().c_str(), textColor2 );
    SDL_Surface * tmpMessage13 = TTF_RenderText_Blended(font, "TOTAL" , textColor2 );
    SDL_Surface * tmpMessage14 = TTF_RenderText_Blended(font, "NEW" , textColor3 );
    SDL_Surface * tmpMessage15 = TTF_RenderText_Blended(font, "HIGH SCORE" , textColor3 );

    SDL_Rect t1 = {SCREEN_WIDTH/2 - tmpMessage1->w/2, 80,0,0 };
    SDL_Rect t2=  {SCREEN_WIDTH/2 - tmpMessage2->w/2, 130,0,0 };
    SDL_Rect t3=  {SCREEN_WIDTH/2 - tmpMessage3->w/2, 250,0,0 };
    SDL_Rect t4=  {SCREEN_WIDTH/3 - tmpMessage4->w/2-50, 350,0,0 };
    SDL_Rect t5=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+300, 350,0,0 };
    SDL_Rect t6=  {SCREEN_WIDTH/3 - tmpMessage4->w/2-50, 400,0,0 };
    SDL_Rect t7=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+100, 400,0,0 };
    SDL_Rect t8=  {SCREEN_WIDTH/3 - tmpMessage4->w/2-50, 450,0,0 };
    SDL_Rect t9=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+100, 450,0,0 };
    SDL_Rect t10=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+300, 400,0,0 };
    SDL_Rect t11=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+300, 450,0,0 };
    SDL_Rect t12=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+300, 520,0,0 };
    SDL_Rect t13=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+100, 520,0,0 };
    SDL_Rect t14=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+500, 380,0,0 };
    SDL_Rect t15=  {SCREEN_WIDTH/3 - tmpMessage5->w/2+450, 430,0,0 };

    SDL_BlitSurface(tmpMessage1,NULL,screen,&t1);
    SDL_BlitSurface(tmpMessage2,NULL,screen,&t2);
    SDL_Flip( screen );
    SDL_Delay(500);
    SDL_BlitSurface(tmpMessage3,NULL,screen,&t3);
    SDL_Flip( screen );
    SDL_Delay(500);
    SDL_BlitSurface(tmpMessage4,NULL,screen,&t4);
    SDL_BlitSurface(tmpMessage5,NULL,screen,&t5);
    SDL_Flip( screen );
    SDL_Delay(1200);
    SDL_BlitSurface(tmpMessage6,NULL,screen,&t6);
    SDL_BlitSurface(tmpMessage7,NULL,screen,&t7);
    SDL_BlitSurface(tmpMessage10,NULL,screen,&t10);
    SDL_Flip( screen );
    SDL_Delay(1200);
    SDL_BlitSurface(tmpMessage8,NULL,screen,&t8);
    SDL_BlitSurface(tmpMessage9,NULL,screen,&t9);
    SDL_BlitSurface(tmpMessage11,NULL,screen,&t11);
    SDL_Flip( screen );
    SDL_Delay(1200);
    SDL_BlitSurface(tmpMessage13,NULL,screen,&t13);
    SDL_BlitSurface(tmpMessage12,NULL,screen,&t12);

    if(newHighScore){
    Mix_PlayChannel( -1, mapPointer->soundvector[7], 0 );
    SDL_BlitSurface(tmpMessage14,NULL,screen,&t14);
    SDL_BlitSurface(tmpMessage15,NULL,screen,&t15);
    }

    SDL_Flip( screen );
    SDL_Delay(1500);

    SDL_FreeSurface(tmpMessage1);
    SDL_FreeSurface(tmpMessage2);
    SDL_FreeSurface(tmpMessage3);
    SDL_FreeSurface(tmpMessage4);
    SDL_FreeSurface(tmpMessage5);
    SDL_FreeSurface(tmpMessage6);
    SDL_FreeSurface(tmpMessage7);
    SDL_FreeSurface(tmpMessage8);
    SDL_FreeSurface(tmpMessage9);
    SDL_FreeSurface(tmpMessage10);
    SDL_FreeSurface(tmpMessage11);
    SDL_FreeSurface(tmpMessage12);
    SDL_FreeSurface(tmpMessage13);
    SDL_FreeSurface(tmpMessage14);
    SDL_FreeSurface(tmpMessage15);
}

void Environment::show_load()
{

    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xB2, 0xDF, 0xFF ) );
    SDL_Surface* loadm = TTF_RenderText_Blended(font, "### Loading ###" , textColor );
    SDL_Rect ttt = {SCREEN_WIDTH/2-loadm->clip_rect.w/2, SCREEN_HEIGHT/2-loadm->clip_rect.h/2};
    SDL_BlitSurface(loadm,NULL,screen,&ttt);
    SDL_FreeSurface(loadm);
    SDL_Flip(screen);
}

void Environment::apply_attak_surface(vector<Item *> &items)
{
    //FIGHT_CLOUD  4
    //PRINCESS_BULLET 5

    Item* fc = items[4];
    Item* pb = items[5];


    SDL_Rect ttt = fc->getRect();
    ttt.x -= camera.x;
    ttt.y -= camera.y;
    SDL_BlitSurface( fc->pictureVec[0], NULL, screen, &ttt);

    ttt = pb->getRect();
    ttt.x -= camera.x;
    ttt.y -= camera.y;
    SDL_BlitSurface( pb->pictureVec[0], NULL, screen, &ttt);


}




//clears the surfaces in cleanup(). Except screen! SDL_Quit() cleans that up automatically
void Environment::clear_surfaces(vector<Item*> &items){
    unsigned int ind; //index variable

    for(ind=0; ind<items.size(); ind++)
    {
        SDL_FreeSurface( items[ind]->pictureVec[0] );
    }

    cout << "Surfaces cleaned" << endl;
}

void Environment::show_pause(map *tmap){

    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

    SDL_Rect t1 = (tmap->resumebox->getRect());
    SDL_Rect t2 = (tmap->quitbox->getRect());
    SDL_Rect t3 = (tmap->savebox->getRect());
     SDL_Rect t4 = (tmap->sel->getRect());

    SDL_BlitSurface(tmap->resumebox->pictureVec[0], NULL, screen, &t1);
    SDL_BlitSurface(tmap->quitbox->pictureVec[0], NULL, screen, &t2 );
    SDL_BlitSurface(tmap->savebox->pictureVec[0], NULL, screen, &t3 );
    SDL_BlitSurface(tmap->sel->pictureVec[0], NULL, screen, &t4 );




    SDL_Flip( screen );
}

void Environment::apply_show_score(Player *player)
{
    //show score
    SDL_Rect xyPos = {SCREEN_WIDTH-990,15,0,0};
    stringstream tmpstr;
    tmpstr << "Score:" << player->getScore();
    scoreText = TTF_RenderText_Blended(font, tmpstr.str().c_str() , textColor );
    SDL_BlitSurface(scoreText,NULL,screen,&xyPos);
    SDL_FreeSurface(scoreText);

    //show lives
    SDL_Rect xyPos2 = {SCREEN_WIDTH-180,15,0,0};
    stringstream tmpstr2;
    tmpstr2 <<player->getLife();
    lifeText = TTF_RenderText_Blended(font, tmpstr2.str().c_str() , textColor );
    SDL_BlitSurface(lifeText,NULL,screen,&xyPos2);
    SDL_FreeSurface(lifeText);

    //show time
    int remainingTime=0;
    SDL_Rect xyPos3 = {SCREEN_WIDTH-915,75,0,0};
    stringstream tmpstr3;
    remainingTime=(player->getCountdown())-(player->getTime()/1000);
    if(remainingTime<0){remainingTime=0;}
    tmpstr3 << std::setw(2) << std::setfill('0') << (remainingTime/60) << ":" <<std::setw(2) << std::setfill('0')<< (remainingTime%60);
    timeText = TTF_RenderText_Blended(font, tmpstr3.str().c_str() , textColor );
    SDL_BlitSurface(timeText,NULL,screen,&xyPos3);
    SDL_FreeSurface(timeText);

    //show highscore
    SDL_Rect xyPos4 = {SCREEN_WIDTH-580,15,0,0};
    stringstream tmpstr4;
    tmpstr4 << "HI: " << player->getHighScore();
    highScoreText = TTF_RenderText_Blended(font, tmpstr4.str().c_str() , textColor );
    SDL_BlitSurface(highScoreText,NULL,screen,&xyPos4);
    SDL_FreeSurface(highScoreText);


    for(unsigned int i = 0; i< displayvector.size();i++){
        Item* temp = displayvector[i];
        SDL_Rect ttt = temp->getRect();
        SDL_BlitSurface(temp->pictureVec[0],NULL,screen,&ttt);

    }

}


void Environment::loadDisplay()
{
    string OiramPic = "media/world/OiramHead.png";
    string ClockPic = "media/world/Clock.png";
    Item* Oiram = new Item(SCREEN_WIDTH-250,23,DISPLAY,OiramPic);
    Item* Clock = new Item(SCREEN_WIDTH-990,80,DISPLAY,ClockPic);
    this->displayvector.push_back(Oiram);
    this->displayvector.push_back(Clock);

}

