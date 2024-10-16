#include "weapon.h"

weapon::weapon(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int str,int maxb,int allbul,int speed,const char* name,bool isa)
{
  frames=anim;
  outerview=ol;
  normalanimation=na;
  reloadanimation=ra;
  fireanimation=fa;
  precision=(prec!=0 ? prec : 0.00001);
  aimprecision=(aprec!=0 ? aprec : 0.00001);

  position=pos;
  rotation=rot;

  aimposition=apos;
  aimrotation=arot;

  strength=str;
  allbullets=allbul;
  maxBulletsInMag=maxb;
  speed=speed;
  name=name;

  position_expected=position;
  position_expected=rotation;

  curpos=position;
  currot=rotation;

  isaim=false;
  isreloading=false;
  isautomatic=isa;
  isfired=false;
  istest=true;
  bulletsInMag=maxBulletsInMag;
  
  lastshot=1000;

  curframe=0;
  curmode=1;
}

weapon::~weapon()
{

}

void weapon::update()
{
  test();
  lastshot++;
  curframe++;
  if(curmode==1) //normal
  {
    if (curframe>=normalanimation)
      curframe=0;
  }else if(curmode==2)
  {
    if(curframe>=normalanimation+fireanimation)
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
    if(curframe>=normalanimation+fireanimation+reloadanimation)
    {
      curmode=1;
      curframe=0;
      isreloading=false;
    }
  }

  vector3d tmpVec(curpos);
  tmpVec.normalize();
  tmpVec*=0.3;
  if(std::abs(position_expected.x-curpos.x)<0.3 &&std::abs(position_expected.y-curpos.y)<0.3 &&std::abs(position_expected.z-curpos.z)<0.3)
    curpos=position_expected;

  tmpVec.change(currot-rotation_expected);
  tmpVec.normalize();
  tmpVec*=0.3;
  if(std::abs(position_expected.x-currot.x)<0.3 &&std::abs(position_expected.y-currot.y)<0.3 &&std::abs(position_expected.z-currot.z)<0.3)
    currot=position_expected;
}

void weapon::show()
{
  glPushMatrix();
    std::cout << "curframe: " << curframe << " frames.size(): " << frames.size() << std::endl;
    glTranslatef(curpos.x,curpos.y,curpos.z);
    glRotatef(currot.x,1,0,0);
    glRotatef(currot.y,0,1,0);
    glRotatef(currot.z,0,0,1);
    //glCallList(frames[curframe]);
    if(curframe >= 0 && curframe < frames.size()) {
      glCallList(frames[curframe]);
    } else {
      std::cerr << "curframe is out of bounds!" << std::endl;
    }
  glPopMatrix();
}

bool weapon::fire(vector3d& direction,vector3d camdirection)
{
  if(isreloading)
    return false;
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
        return true;
      }else{
        reload();
        return false;
      }
    }
  }
  return 0;
}

void weapon::stopfire()
{
  isfired=false;
}

void weapon::reload()
{
  if(isreloading && maxBulletsInMag!=bulletsInMag)
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

void weapon::aim()
{
  isaim=!isaim;
  if(isaim)
  {
    rotation_expected=aimrotation;
    position_expected=aimposition;
  }else
  {
    rotation_expected=rotation;
    position_expected=position;
  }
}

void weapon::test()
{
  if(istest)
  {
    Uint8* keys=SDL_GetKeyState(NULL);
    if(keys[SDLK_j])
      position_expected.x-=0.02;
    if(keys[SDLK_l])
      position_expected.x+=0.02;
    if(keys[SDLK_k])
      position_expected.z-=0.02;
    if(keys[SDLK_k])
      position_expected.z+=0.02;
    if(keys[SDLK_k])
      position_expected.y-=0.02;
    if(keys[SDLK_k])
      position_expected.y+=0.02;
    std::cout << "line 200 position expected: " << position_expected << position_expected << std::endl;
  }
}

void weapon::addBullets(unsigned int num)
{
  allbullets+=num;
}

void weapon::setBullets(unsigned int num)
{
  allbullets=num;
}

int weapon::getStrength()
{
  return strength;
}

int weapon::getAmmo()
{
  return bulletsInMag;
}

int weapon::getAllAmmo()
{
  return allbullets;
}

std::string weapon::getName()
{
  return name;
}

std::vector<unsigned int>& weapon::getAnimation()
{
  return frames;
}

bool weapon::isAimed()
{
  return isaim;
}

unsigned int weapon::getOuterView()
{
  return outerview;
}

