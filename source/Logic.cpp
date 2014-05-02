#include "logic.h"

#include "map.h"
#include <math.h>

#include <string>

Logic::Logic()
{
}

void Logic::run(vector<Item *> &items, map *tmap)
{
    mapPointer = tmap;

    for(unsigned int i = 0; i < items.size();i++){

        switch(items[i]->type){

        case PLAYER: handlePlayer(items[i],items);break;

        case CRAB: handleCrab(i,items);break;

        case GHOST: handleGhost(i,items);break;

        case BULLET: handleBullet(i,items);break;

        case FIGHT_CLOUD: handleFightCloud(i,items);break;

        case PRINCESS_BULLET: handlePrincessBullet(i,items);break;

        case GRRR: handleGrrr(i,items);break;

        case TRAPSTONE: handleTrapStone(i,items);break;

        default: break;
        }
    }

}


int Logic::checkCollision(Item *myitem1, Item *myitem2 ){

    SDL_Rect i1 = myitem1->getRect();
    SDL_Rect i2 = myitem2->getRect();

    if(i1.x > i2.x + i2.w-1) //i1 rechts von i2
        return 0;
    if(i1.x + i1.w < i2.x+1) //i1 links von i2
        return 0;
    if(i1.y > i2.y + i2.h-1 ) //i1 unter  i2
        return 0;
    if(i1.y + i1.h < i2.y+1) //i1 über  i2
        return 0;

    return 1;
}

void Logic::handleCrab(int pos, vector<Item *> &items)
{
    MovingItem* crab = (MovingItem*)items[pos];
    moveX(crab);

    for(unsigned int j = 0; j<items.size();j++){
        if(items[j]->type == MAP ||
                items[j]->type == DO_NOT_PLOT){
            if( checkCollision(items[pos],items[j])){
                if(crab->vx()>0)
                    crab->setX(items[j]->x()-crab->w());

                if(crab->vx()<0)
                    crab->setX(items[j]->x()+items[j]->w());

                crab->setVx(-crab->vx());
            }
        }
    }


    //GRAYITY: Only till Crab is standing
    // if(crab->isStanding() == false){
    crab->setVy(3);
    moveY(crab);
    for(unsigned int j = 0; j<items.size();j++){
        if(items[j]->type == MAP){
            if( checkCollision(items[pos],items[j])){
                Item* m = items[j];
                crab->setStanding(true);
                crab->setY(m->getRect().y-crab->getRect().h);

            }
        }
    }
    //}

}

void Logic::handleGrrr(int pos, vector<Item *> &items)
{
    MovingItem* grrr = (MovingItem*)items[pos];

    if(grrr->isStanding() == true){
        grrr->setVy(-2);
        if(grrr->ownTimer.get_ticks()>1000)
            moveY(grrr);

        for(unsigned int j = 0; j<items.size();j++){
            if(items[j]->type == MAP ||
                    items[j]->type == DO_NOT_PLOT){
                if( checkCollision(items[pos],items[j])){
                    Item* it = items[j];
                    grrr->setStanding(false);
                    grrr->ownTimer.start();
                    grrr->setY(it->y()+it->h()+1);
                }
            }
        }
    }
    else{
        grrr->setVy(9);
        if(grrr->ownTimer.get_ticks() > 3000)
            moveY(grrr);

        for(unsigned int j = 0; j<items.size();j++){
            if(items[j]->type == MAP ||
                    items[j]->type == DO_NOT_PLOT){
                if( checkCollision(items[pos],items[j])){
                    Item* it = items[j];
                    grrr->setStanding(true);
                    grrr->ownTimer.start();
                    grrr->setY(it->y() - grrr->h() );
                }
            }
        }
    }


}

void Logic::handleFightCloud(int pos, vector<Item *> &items)
{
    Item* fc = items[pos];//pos = 4!
    if(fc->ownTimer.get_ticks()>1000){
        fc->setX(COMPLETE_WORLD_LENGTH+100);
    }
}

void Logic::handleBullet(int pos, vector<Item *> &items)
{
    Player* pl = (Player*)items[0];
    MovingItem* mv = (MovingItem*)items[pos];

    if(pl->waitForBullet){
        Mix_PlayChannel( -1, mapPointer->soundvector[6], 0 );
        pl->waitForBullet = false;
        mv->setX(pl->x()+pl->w()/2);
        mv->setY(pl->y() + pl->h()/3);
        if(pl->looksLeft()){
            mv->setVx(-BULLET_SPEED);
        }
        else
            mv->setVx(BULLET_SPEED);
        mv->ownTimer.start();
    }
    if(mv->ownTimer.get_ticks()>BULLET_LIVETIME)
        mv->setY(COMPLETE_WORLD_HEIGTH+100);

    moveX(mv);

    for(unsigned int i = 0;i<items.size();i++){
        if( checkCollision(mv,items[i])==1){
            if(items[i]->type == MAP)
                mv->setY(COMPLETE_WORLD_HEIGTH+100);

            if(items[i]->type == CRAB){
                pl->setScore(false,30);
                Item* crab = items[i];
                Item* fightcloud = items[4];
                fightcloud->ownTimer.start();
                fightcloud->setX(crab->x()+crab->w()/2-fightcloud->w()/2);
                fightcloud->setY(crab->y()+crab->h()/2-fightcloud->h()/2);
                delete items[i];
                items.erase(items.begin()+i);
                mv->setY(COMPLETE_WORLD_HEIGTH+100);
            }

            if(items[i]->type == PRINCESS_BULLET){
                MovingItem* pbull = (MovingItem*)items[i];
                Item* fightcloud = items[4];
                fightcloud->ownTimer.start();
                fightcloud->setX(pbull->x()+pbull->w()/2-fightcloud->w()/2);
                fightcloud->setY(pbull->y()+pbull->h()/2-fightcloud->h()/2);
                pbull->setX(COMPLETE_WORLD_HEIGTH+10);
                pbull->setVx(0);
                pbull->setVy(0);
            }

        }


    }

}

void Logic::handlePrincessBullet(int pos, vector<Item *> &items)
{
    MovingItem* bullet = (MovingItem*)items[pos];
    Item* peach = (Item*)items[1]; //immer peach
    Item* player = (Item*)items[0]; //immer oiram

    if(bullet->ownTimer.get_ticks()>PRINCESS_BULLET_INTERVALL){

        bullet->ownTimer.start();
        bullet->setX(peach->x());
        bullet->setY(peach->y());

        //cout << "peachx: "<<peach->x() <<"peachy: "<< peach->y()<< endl;
        //cout << "playerx: "<<player->x() <<"playery: "<< player->y()<< endl;

        //Pythagoras + Strahlensatz :-)
        double dx = (double)(peach->x()-player->x());
        double dy = (double)(player->y()-peach->y());
        double vx = PRINCESS_BULLET_SPEED/sqrt(1.0+(dy*dy)/(dx*dx));
        double vy = vx*dy/dx;

        vx = round(vx);
        vy = round(vy);

        //cout << "vx: "<<vx <<"vy: "<< vy<< endl;

        bullet->setVx(-vx);
        bullet->setVy(vy);
    }

    moveX(bullet);
    moveY(bullet);

}



void Logic::handleGhost(int pos, vector<Item *> &items)
{
    MovingItem* ghost = (MovingItem*)items[pos];
    moveY(ghost);
    for(unsigned int j = 0; j<items.size();j++){
        if(items[j]->type == MAP ||
                items[j]->type == DO_NOT_PLOT){

            if( checkCollision(items[pos],items[j])>0){
                moveYBack(ghost);
                ghost->setVy(-(ghost->vy()));

            }
        }
    }
}

void Logic::handleTrapStone(int pos, vector<Item *> &items)
{


    MovingItem* pl = (MovingItem*)items[0];
    Item* tstone = items[pos];


    if(pl->vy() < 0 && checkCollision((Item*)pl,tstone) == 1){
        moveYBack(pl);
        if(checkCollision((Item*)pl,tstone) == 0){
            tstone->type= MAP;
            cout<<pl->vx() << " "<< pl->vy()<< endl;
            pl->setY(tstone->y()+tstone->h());
            //cout << "ada" << endl;
        }
        else{
            moveY(pl);
        }
    }


}

void Logic::handlePlayer(Item *tmp, vector<Item *> &items)
{

    Player* pl = (Player*)tmp;
    standing = false;
    int collFlag = 0;
    if(!pl->isStanding() && pl->vy() < 100)
        pl->setVy(pl->vy()+GRAVITY);

    //////////////////////////////
    ///////IN X DIRECTION/////////

    moveX(pl);
    for(unsigned int j = 0; j<items.size();j++){

        collFlag = checkCollision((Item*)pl,items[j]);
        if(collFlag != 0){
            switch(items[j]->type){

            case MAP:
                collisionPlayerMapX(pl,j,items);
                break;

            case GHOST:
                collisionPlayerGhostXY(pl,j,items);
                break;

            case CRAB:
                collisionPlayerCrabX(pl,j,items);
                break;

            case COIN:
                collisionPlayerCoinXY(pl,j,items);
                break;

            case HEART:
                collisionHeartXY(pl,j,items);
                break;

            case PRINCESS_BULLET:
                collisionPrincesBulletXY(pl,j,items);
                break;

            case GOAL:
                collisionGoalXY();
                break;

            case STAR:
                collisionStarXY(pl,j,items);
                break;

            case WATER:
                collisionWaterYX(pl);
                break;

            case GRRR:
                collisionGrrrXY(pl);
                break;

            default:
                break;
            }
        }
    }

    //////////////////////////////
    ///////IN Y DIRECTION/////////
    moveY(pl);
    for(unsigned int j = 0; j<items.size();j++){

        collFlag = checkCollision((Item*)pl,items[j]);
        if(collFlag != 0){
            switch(items[j]->type){

            case MAP:
                collisionPlayerMapY(pl,j,items);
                break;

            case GHOST:
                collisionPlayerGhostXY(pl,j,items);
                break;

            case CRAB:
                collisionPlayerCrabY(pl,j,items);
                break;

            case COIN:
                collisionPlayerCoinXY(pl,j,items);
                break;

            case HEART:
                collisionHeartXY(pl,j,items);
                break;

            case PRINCESS_BULLET:
                collisionPrincesBulletXY(pl,j,items);
                break;

            case GOAL:
                collisionGoalXY();
                break;

            case STAR:
                collisionStarXY(pl,j,items);
                break;

            case WATER:
                collisionWaterYX(pl);
                break;

            case GRRR:
                collisionGrrrXY(pl);
                break;

            default:
                break;
            }
        }
    }

    if(pl->y() > COMPLETE_WORLD_HEIGTH)
        pl->respawn();

    pl->setStanding(standing);

}

void Logic::collisionPlayerMapX(Player *tmp, int pos,vector<Item*> &items)
{
    if(tmp->vx()<0){
        //teleport nach links
        tmp->setX(items[pos]->x()+items[pos]->getRect().w);
    }
    if(tmp->vx()>0){
        //teleport nach rechts
        tmp->setX(items[pos]->x()-tmp->w());
    }
}

void Logic::collisionPlayerMapY(Player *tmp, int pos,vector<Item*> &items)
{

    if(tmp->vy() > 0){
        //teleport to floor
        tmp->setY(items[pos]->y() - tmp->getRect().h);
        tmp->friction=100;
        standing = true;
    }

    if(tmp->vy() < 0){
        //teleport to ceiling
        tmp->setY(items[pos]->y()+items[pos]->getRect().h);
        tmp->setVy(0);
    }

}

void Logic::moveX(MovingItem *movitem)
{
    movitem->setX(movitem->x() + movitem->vx());
}

void Logic::moveY(MovingItem *movitem)
{
    movitem->setY(movitem->y() + movitem->vy());
}

void Logic::moveXBack(MovingItem *movitem)
{
    movitem->setX(movitem->x() - movitem->vx());
}

void Logic::moveYBack(MovingItem *movitem)
{
    movitem->setY(movitem->y() - movitem->vy());
}

void Logic::collisionPlayerGhostXY(Player *tmp, int pos, vector<Item *> &items)
{
    if(tmp->invulnerable()==false)
        tmp->looseLife();
}

void Logic::collisionPlayerCrabX(Player *tmp, int pos, vector<Item *> &items)
{
    if(tmp->invulnerable()==false)
        tmp->looseLife();

}

void Logic::collisionPlayerCrabY(Player *tmp, int pos, vector<Item *> &items)
{

    if(tmp->vy()>0 && tmp->isStanding() == false){
        cout<< "Crab is DEAD!!!"<<endl;
        tmp->setScore(false,60);
        tmp->setVy(-JUMPING_SPEED/1.2);
        tmp->setY(items[pos]->y()-tmp->h());
        delete items[pos];
        items.erase(items.begin()+pos);

    }
    else{
        if(tmp->invulnerable()==false)
            tmp->looseLife();
    }

}

void Logic::collisionPlayerCoinXY(Player *tmp, int pos, vector<Item *> &items)
{
    Mix_PlayChannel( -1, mapPointer->soundvector[4], 0 );
    tmp->setScore(false,50);
    delete items[pos];
    items.erase(items.begin()+pos);
}

void Logic::collisionHeartXY(Player *tmp, int pos, vector<Item *> &items)
{
    delete items[pos];
    items.erase(items.begin()+pos);
    //cout << "Heart collected" << endl;
    Mix_PlayChannel( -1, mapPointer->soundvector[5], 0 );
    tmp->setLife(tmp->getLife()+1);

}

void Logic::collisionStarXY(Player *tmp, int pos, vector<Item *> &items)
{
    Mix_PlayChannel( -1, mapPointer->soundvector[7], 0 );
    Player* pl =(Player*) items[0];
    pl->setScore(false,30);
    delete items[pos];
    items.erase(items.begin()+pos);
    tmp->setToSuperMode();
}

void Logic::collisionPrincesBulletXY(Player *tmp, int pos, vector<Item *> &items)
{
    if(tmp->invulnerable() == false){
        MovingItem* bullet = (MovingItem*)items[pos];
        bullet->ownTimer.start();
        tmp->looseLife();
    }
}

void Logic::collisionWaterYX(Player* tmp) {
    tmp->respawn();
}

void Logic::collisionGoalXY() {
    this->mapPointer->level_done=true;
}

void Logic::collisionGrrrXY(Player *tmp)
{
    if(tmp->invulnerable()==false)
        tmp->looseLife();
}


