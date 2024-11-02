#include "player.h"

player::player(const char* n,collisionsphere ccs,float sprints,float normals,float looks,std::shared_ptr<weapon> wep)
{
  name=n;
  cs=ccs;
  sprintspeed=sprints;
  normalspeed=normals;
  lookspeed=looks;
  force.change(0.0,-0.3,0.0);
  direction.change(0.0,0.0,0.0);
  setPosition(ccs.center);
  cam.setSpeed(normalspeed,looks);
  points=0;
  energy=50;
  isground=iscollision=issprint=false;
  health=10000;
  weapons.push_back(wep);
  curweapon=0;
  isWeapon=true;
  headPosition=0;
  lifeTime=0;
}

player::~player()
{

}

void player::update(std::vector<collisionplane>& collplane)
{
  if(direction.y>=force.y)
    direction+=force;
  vector3d newpos(cam.getLocation());
  newpos+=direction;
  vector3d oldpos(newpos);
  for(int i=0;i<collplane.size();i++)
    collision::sphereplane(newpos,collplane[i].normal,collplane[i].p[0],collplane[i].p[1],collplane[i].p[2],collplane[i].p[3],cs.r);
  if(oldpos.y<newpos.y)
    isground=true;
//  if(newpos.y<-2)
//  {
//    newpos.y = -1;
//    std::cout << newpos.y << std::endl;
//  }
  setPosition(newpos);

  if(isWeapon)
  {
    weapons[curweapon]->setCurpos(newpos);
    weapons[curweapon]->setCurrot(cam.getVector());
    weapons[curweapon]->setPitchAndYaw(cam.getCamPitch(),cam.getCamYaw());
    auto curWeapon = weapons[curweapon];
    if(auto gunPtr = std::dynamic_pointer_cast<gun>(curWeapon)){
      gunPtr->update();
    }else if(auto meleePtr = std::dynamic_pointer_cast<melee>(curWeapon)) {
      meleePtr->update();
    }
  }

  if(issprint)
    energy-=0.5;
  else if(energy<50)
    energy+=0.05;
  if(issprint && energy<=0.0)
    stopSprint();

  if(cam.isMoved())
  {
    if(headUp)
    {
      headPosition+=10;
      if(headPosition>=50)
        headUp=false;
    }else{
      headPosition-=10;
      if(headPosition<=-50)
        headUp=true;
    }
    cam.lookAt(cam.getCamPitch()+(float)headPosition/(150.0-issprint*100),cam.getCamYaw());
  }
}

void player::show()
{
  if(isWeapon)
  {
    auto curWeapon = weapons[curweapon];
    if(auto gunPtr = std::dynamic_pointer_cast<gun>(curWeapon)){
      gunPtr->show();
    }else if(auto meleePtr = std::dynamic_pointer_cast<melee>(curWeapon)) {
      meleePtr->show();
    }
  }
}

void player::setPosition(vector3d pos)
{
  cs.center=pos;
  cam.setLocation(pos);
}

//getters
std::string player::getName()
{
  return name;
}

int player::getHealth()
{
  return health;
}

collisionsphere player::getCollisionSphere()
{
  return cs;
}

int player::getPoints()
{
  return points;
}

//setters
void player::setHealth(int h)
{
  health=h;
}

void player::addPoints(int n)
{
  points+=n;
}

void player::addWeapon(std::shared_ptr<weapon> wep)
{
  weapons.push_back(wep);
}

void player::changeWeapon(int num)
{
  if(weapons.size()>num)
    curweapon=num;
}

void player::changeWeaponDown()
{
  curweapon--;
  if(curweapon<0)
    curweapon=weapons.size()-1;
}

void player::changeWeaponUp()
{
  curweapon++;
  if(curweapon>=weapons.size())
    curweapon=0;
}

std::shared_ptr<weapon> player::getCurrentWeapon()
{
  return weapons[curweapon];
}

void player::haveWeapon(bool b)
{
  isWeapon=b;
}

camera* player::getCamera()
{
	return &cam;
}

void player::setJump()
{
  if(isground)
  {
    direction.change(0.0,2,0.0);
    isground=false;
  }
}

void player::setSprint()
{
  if(energy>2.0)
  {
    issprint=true;
    cam.setSpeed(sprintspeed,cam.getMousevel());
  }else{
    issprint=false;
  }
}

void player::stopSprint()
{
  issprint=false;
  cam.setSpeed(normalspeed,cam.getMousevel());
}

Uint32 player::getLifeTime()
{
  return lifeTime;
}

void player::setLifeTime(Uint32 newLifeTime)
{
  lifeTime = newLifeTime;
}

