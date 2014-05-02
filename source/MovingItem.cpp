#include "movingItem.h"

MovingItem::MovingItem()
{

}

MovingItem::MovingItem(int x, int y, ITEM_TYP p_type,std::string image_string){
    this->addPic(image_string);
    this->setX(x);
    this->setY(y);
    this->velX=0;
    this->velY=0;
    this->type = p_type;
    this->isAlive = true;
    this->ownTimer.start();
    this->standingOnFloor = false;
}

void MovingItem::setVx(double x)
{   this->velX = x;
    if(x<0)
        this->lookingleft = true;
    if(x>0)
        this->lookingleft = false;
}
