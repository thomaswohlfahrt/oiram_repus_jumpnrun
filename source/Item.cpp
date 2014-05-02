#include "item.h"

using namespace std;

Item::Item(){
}
Item::Item(int x, int y, ITEM_TYP p_type, string image_string){
  cout << image_string << endl;
  this->addPic(image_string);
  this->setX(x);
  this->setY(y);
  this->type = p_type;
  ownTimer.start();

}
Item::~Item(){
  //Clear allocated graphics
  while (!this->pictureVec.empty() ){
    SDL_FreeSurface(this->pictureVec[this->pictureVec.size()-1]);
    this->pictureVec.pop_back();
  }

}
void Item::addPic(std::string picName){

  //cout << picName << endl;

  SDL_Surface* loadedImage;
  loadedImage = loadPNG(picName);


  if( loadedImage != NULL ) { 

    hitbox.w = loadedImage->w;
    hitbox.h = loadedImage->h;

    //cout << "h:"<<hitbox.h << " w: "<< hitbox.w << endl;

    pictureVec.push_back(loadedImage);
    numOfSprites=0;

  }
  else{
    cout << "Imageadd Error: "<<picName << endl;
  }

}

bool Item::contains(int x , int y){
  if( ( x > this->getRect().x ) && ( x < this->getRect().x + this->getRect().w ) && ( y > this->getRect().y ) && ( y < this->getRect().y + this->getRect().h ) )
    return true;
  return false;
}


SDL_Surface* Item::loadPNG(std::string img)
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;

  loadedImage = IMG_Load(img.c_str() );

  if( loadedImage != NULL ) {

    //cout<< "could open image: "<< img << endl;

    optimizedImage = SDL_DisplayFormatAlpha( loadedImage );

    //cout<< "alpha: "<< optimizedImage << endl;
    SDL_FreeSurface( loadedImage );
    return optimizedImage;

  }
  else{
    cout << "There was an Error loading image: " << img << endl;
    return 0;
  }
}
