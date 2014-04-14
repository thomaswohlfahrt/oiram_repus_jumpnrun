#include "player.h"

Player::Player()
{

}


Player::Player(int x, int y, ITEM_TYP p_type, const char *image_string){
    this->addPic(image_string);
    this->setX(x);
    this->setY(y);
    this->velX=0;
    this->velY=GRAVITY;
    this->type = p_type;
    this->score=0;
    this->life=LIVES;
    this->blockTimer.start();
    this->blockingTime = 0;
    this->displayClock.start();
    this->manipulatedTime=0;
    this->countdown=0;
    this->friction=100;
    this->tInvul.start();
    ownTimer.start();
    shootTimer.start();
    waitForBullet = false;
    superMode = false;
    highScore=0;

}

int Player::getScore(void){
    return score;
}

void Player::setScore(bool resetScore, int gain){
    if(resetScore==true){
        score=0;
    }
    else if(resetScore==false){
        score+=gain;
    }
}

int Player::getLife(){
    return life;
}

void Player::setLife(int lives){
    this->blockingTime = this->blockTimer.get_ticks();
    std::cout << this->blockingTime << std::endl;
    life=lives;
}

int Player::getBlockingTime(){
    return blockingTime;
}

void Player::setBlockingTime(int time){
    blockingTime = time;
}

//this function loads time from a load file
void Player::addTime(int time){

    manipulatedTime+=time;

}

//calculates game clock (not remaining time)
int Player::getTime(){
    return (manipulatedTime+displayClock.get_ticks());
}

//variable 'countdown' shouldn't be changed during the game. This function should only be called at loading a map !
void Player::setCountdown(int time){
    countdown=time;
}

void Player::looseLife()
{
    this->superMode = false;
    tInvul.start();
    std::cout << "lifes:"<<life<< std::endl;
    this->life = this-> life- 1;
    if(life<=0){
        this->respawn();
    }
}

void Player::respawn()
{

    SDL_Surface* screen;
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    SDL_Color textColor = { 0, 255, 255,0 };
    TTF_Font *font = TTF_OpenFont( "media/ttf/animeace.ttf", 50 );
    SDL_Surface *t = TTF_RenderText_Blended(font, "You DIED!", textColor ); //is freed
    SDL_Rect rec={(SCREEN_WIDTH/2)-(t->w/2),(SCREEN_HEIGHT/2)-(t->h/2),0,0};
    SDL_BlitSurface(t,NULL,screen,&rec);
    SDL_Flip(screen);
    SDL_Delay(2000);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(t);
    TTF_CloseFont(font);

    this->setX(PLAYER_BEGIN_POS_X);
    this->setY(PLAYER_BEGIN_POS_Y);
    //this->displayClock.start();
    this->life=LIVES;
    this->superMode = false;
}

bool Player::invulnerable()
{
    if(tInvul.get_ticks() < INVULNERABLE_TIME)
        return true;
    else
        return false;
}

bool Player::shoot()
{   //Bool if player is able to shoot again
    if(shootTimer.get_ticks()>PLAYER_BULLET_INTERVALL && this->isInSuperMode()){
    std::cout <<"waiting for bullet"<<std::endl;
        shootTimer.start();
        waitForBullet = true;
        return true;
    }
    else
        return false;
}

int Player::getCountdown(){
    return countdown;
}

int Player::getHighScore(){
    return highScore;
}

void Player::setHighScore(int score){
    highScore=score;
    std::cout << highScore << std::endl;
}



