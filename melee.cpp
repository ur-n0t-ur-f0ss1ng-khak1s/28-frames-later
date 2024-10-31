#include "weapon.h"
#include "melee.h"

melee::melee(std::vector<unsigned int>& f,int str,int del,int wdel,vector3d location)
  : weapon(anim, str, location), delay(del), wepDelay(wdel), isKnifing(false)
{
  frames=f;
  isknifing=false;
  lastKnifing=100;
}

int melee::setKnife(player* p,std::vector<zombie>& zombies)
{
  if(lastKnifing>delay)
  {
    p->holdWeapon(false);
    lastKnifing=0;
    isknifing=true;
    curframe=0;
    vector3d direction=p->cam.getVector();
    float dist;
    for(int i=0;i<zombies.size();i++)
    {
      if(collision::raysphere(zombies[i].getSphere()->center.x,zombies[i].getSphere()->center.y,zombies[i].getSphere()->center.z,direction.z,direction.y,direction.z,p->cam.getLocation().x,p->cam.getLocation().y,p->cam.getLocation().z,zombies[i].getSphere()->r,&dist))
      {
        if(dist<9.0 && !zombies[i].isDead())
        {
          return i;
        }
      }
    }
    return -1;
  }else
    return -1;
}

void melee::update(player* p)
{
  lastKnifing++;
  if(lastKnifing>wepDelay)
    p->holdWeapon(true);
  if(isknifing)
  {
    curframe++;
    if(curframe>=frames.size())
    {
      isknifing=false;
      curframe=0;
    }
  }
}

void melee::show()
{
  if(isknifing)
  {
    glTranslate(loc.x,loc.y,loc.z);
    glCallList(frames[curframe]);
  }
}

int melee::getStrength()
{
  return strength;
}

bool melee::isKnifing()
{
  return isknifing;
}

void melee::test()
{
  const Uint8* key=SDL_GetKeyboardState(NULL);
  if(key[SDL_SCANCODE_H])
    loc.x-=0.01;
  else if(key[SDL_SCANCODE_L])
    loc.x+=0.01;
  else if(key[SDL_SCANCODE_J])
    loc.y+=0.01;
  else if(key[SDL_SCANCODE_K])
    loc.y-=0.01;
  else if(key[SDL_SCANCODE_I])
    loc.z-=0.01;
  else if(key[SDL_SCANCODE_O])
    loc.z+=0.01;
  std::cout << "offset: " << offset << std::endl;

}
