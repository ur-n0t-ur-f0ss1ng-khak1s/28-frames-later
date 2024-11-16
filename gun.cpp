#include "weapon.h"
#include "gun.h"

gun::gun(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d ofset,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int str,int maxb,int allbul,int speedd,const char* namee,bool isa)
  : weapon(anim, str, pos, rot), outerview(ol), normalanimation(na), fireanimation(fa), reloadanimation(ra)
{
  outerview=ol;
  normalanimation=na;
  reloadanimation=ra;
  fireanimation=fa;
  precision=(prec!=0 ? prec : 0.00001);
  aimprecision=(aprec!=0 ? aprec : 0.00001);

  offset=ofset;

  aimposition=apos;
  aimrotation=arot;

  allbullets=allbul;
  maxBulletsInMag=maxb;
  speed=speedd;

  position_expected=position;
  rotation_expected=rotation;

  isaim=false;
  isreloading=false;
  isautomatic=isa;
  isfired=false;
  istest=true;
  bulletsInMag=maxBulletsInMag;
  
  lastshot=1000;

  curmode=1;
}

gun::~gun()
{

}

void gun::update()
{
  lastshot++;
  curframe++;
  if(curmode==1) //normal
  {
    if (curframe>=normalanimation)
      curframe=0;
  }else if(curmode==2)
  {
    if(curframe>normalanimation+fireanimation)
    {
      if(isautomatic && isfired) //may be a BUG
      {
        curframe=normalanimation;
      }else{
        curmode=1;
        curframe=0;
      }
    }
  }else if(curmode==3)
  {
    if(curframe>=normalanimation+fireanimation+reloadanimation-1)
    {
      curmode=1;
      curframe=0;
      isreloading=false;
    }
  }

  vector3d forwardVector = currot; 
  vector3d rightVector(forwardVector.z, 0, -forwardVector.x);
  rightVector.normalize();
  curpos = curpos + forwardVector * offset.z //offset forward
                  + rightVector * offset.x //offset to the right
                  + vector3d(0,1,0) * offset.y; //offset slight down

//  vector3d tmpVec(position_expected-curpos);
//  tmpVec.normalize();
//  tmpVec*=0.3;
//  curpos+=tmpVec;
//  if(std::abs(position_expected.x-curpos.x)<0.3 && std::abs(position_expected.y-curpos.y)<0.3 && std::abs(position_expected.z-curpos.z)<0.3)
//    curpos=position_expected;
//
//  tmpVec.change(rotation_expected-currot);
//  tmpVec.normalize();
//  tmpVec*=0.3;
//  currot+=tmpVec;
//  if(std::abs(rotation_expected.x-currot.x)<0.3 && std::abs(rotation_expected.y-currot.y)<0.3 && std::abs(rotation_expected.z-currot.z)<0.3)
//    currot=rotation_expected;

  //std::cout << "offset: " << offset << "pitch: " << pitch << "yaw: " << yaw << std::endl;
}

void gun::show()
{
  test();
  glPushMatrix();
    glTranslatef(curpos.x,curpos.y,curpos.z);
    if(isaim)
    {
      glRotatef(yaw-90.0f, 0.0f, 1.0f, 0.0f);
      glRotatef(-pitch-3.0f, 0.0f, 0.0f, 1.0f);
    }else{
      glRotatef(yaw-80.0f, 0.0f, 1.0f, 0.0f);
      glRotatef(-pitch, 0.0f, 0.0f, 1.0f);
    }
    if(curframe >= 0 && curframe < frames.size()) {
      glCallList(frames[curframe]);
    } else {
      std::cerr << "curframe is out of bounds!" << std::endl;
    }
  glPopMatrix();
}

bool gun::fire(vector3d camdirection)
{
  if(isreloading)
    return 0;
  if((!isautomatic && !isfired || isautomatic))
  {
    if(lastshot>=speed)
    {
      if(bulletsInMag>0)
      {
        if(isaim)
        {
          direction.x=camdirection.x+((float)(rand()%3-1)/aimprecision);
          direction.y=camdirection.y+((float)(rand()%3-1)/aimprecision);
          direction.z=camdirection.z+((float)(rand()%3-1)/aimprecision);
        }else{
          direction.x=camdirection.x+((float)(rand()%3-1)/precision);
          direction.y=camdirection.y+((float)(rand()%3-1)/precision);
          direction.z=camdirection.z+((float)(rand()%3-1)/precision);
        }
        isfired=true;
        lastshot=0;
        bulletsInMag--;
        curframe=normalanimation;
        curmode=2;
        return 1;
      }else{
        reload();
        return 0;
      }
    }
  }
  return 0;
}

void gun::stopfire()
{
  isfired=false;
}

void gun::reload()
{
  if(!isreloading && maxBulletsInMag!=bulletsInMag)
  {
    isreloading=true;
    if(allbullets>maxBulletsInMag-bulletsInMag)
    {
      allbullets-=maxBulletsInMag-bulletsInMag;
      bulletsInMag=maxBulletsInMag;
    }else{
      bulletsInMag=allbullets+bulletsInMag;
      allbullets=0;
    }
    curframe=normalanimation+fireanimation;
    curmode=3;
  }
}

void gun::aim()
{
  isaim=!isaim;
  if(isaim)
  {
    offset.y+=1;
    offset.x+=1;
    rotation_expected+=aimrotation;
    position_expected+=aimposition;
  }else
  {
    offset.y-=1;
    offset.x-=1;
    rotation_expected+=rotation;
    position_expected+=position;
  }
}

void gun::test()
{
  if(istest)
  {
    const Uint8* keys=SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_J])
      offset.x-=0.2;
    if(keys[SDL_SCANCODE_K])
      offset.x+=0.2;
    if(keys[SDL_SCANCODE_L])
      offset.z-=0.2;
    if(keys[SDL_SCANCODE_O])
      offset.z+=0.2;
    if(keys[SDL_SCANCODE_U])
      offset.y-=0.01;
    if(keys[SDL_SCANCODE_I])
      offset.y+=0.01;
  }
  //std::cout << offset << std::endl;
}

void gun::addBullets(unsigned int num)
{
  allbullets+=num;
}

void gun::setBullets(unsigned int num)
{
  allbullets=num;
}

int gun::getAmmo()
{
  return bulletsInMag;
}

int gun::getAllAmmo()
{
  return allbullets;
}

bool gun::isAimed()
{
  return isaim;
}

unsigned int gun::getOuterView()
{
  return outerview;
}

vector3d gun::getDirection()
{
  return direction;
}

