#include "weapon.h"
#include "melee.h"

melee::melee(std::vector<unsigned int>& f,int str,int del,int wdel, vector3d offset)
  : weapon(f, str, vector3d(0,0,0), vector3d(0,0,0), offset), delay(del), wepDelay(wdel)
{
  isknifing=false;
  lastKnifing=100;
}

int melee::setKnife(vector3d playerLoc,vector3d playerDir,std::vector<std::shared_ptr<zombie>>& zombies)
{
  if(lastKnifing>delay)
  {
    lastKnifing=0;
    isknifing=true;
    curframe=0;
    float dist;
    for(int i=0;i<zombies.size();i++)
    {
      if(collision::raysphere(zombies[i]->getCollisionSphere()->center.x,zombies[i]->getCollisionSphere()->center.y,zombies[i]->getCollisionSphere()->center.z,playerDir.x,playerDir.y,playerDir.z,playerLoc.x,playerLoc.y,playerLoc.z,zombies[i]->getCollisionSphere()->r,&dist))
      {
        if(dist<15.0 && !zombies[i]->isDead())
        {
          return i;
        }
      }
    }
    return -1;
  }else
    return -1;
}

void melee::update()
{
  lastKnifing++;
  if(isknifing)
  {
    curframe++;
    if(curframe>=frames.size())
    {
      isknifing=false;
      curframe=0;
    }
  }

  vector3d forwardVector = currot; 
  vector3d rightVector(forwardVector.z, 0, -forwardVector.x);
  rightVector.normalize();
  curpos = curpos + forwardVector * offset.z //offset forward
                  + rightVector * offset.x //offset to the right
                  + vector3d(0,1,0) * offset.y; //offset slight down


}

void melee::show()
{
  glPushMatrix();
    glTranslatef(curpos.x,curpos.y,curpos.z);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);
    glCallList(frames[curframe]);
  glPopMatrix();
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
    offset.x-=0.01;
  else if(key[SDL_SCANCODE_L])
    offset.x+=0.01;
  else if(key[SDL_SCANCODE_J])
    offset.y+=0.01;
  else if(key[SDL_SCANCODE_K])
    offset.y-=0.01;
  else if(key[SDL_SCANCODE_I])
    offset.z-=0.01;
  else if(key[SDL_SCANCODE_O])
    offset.z+=0.01;
  std::cout << "offset: " << offset << std::endl;
}
