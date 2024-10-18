#include "player.h"

player::player(const char* n,collisionsphere ccs,float sprints,float normals,float looks,std::shared_ptr<weapon> wep)
{
  name=n;
  cs=ccs;
  sprintspeed=sprints;
  normalspeed=normals;
  lookspeed=looks;
  force.change(0.0,-0.3,0.0);
  setPosition(ccs.center);
  cam.setSpeed(normalspeed,looks);
  points=0;
  energy=10;
  isground=iscollision=issprint=false;
  health=100;
  weapons.push_back(wep);
  curweapon=0;
  isWeapon=true;
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
  for(int i=0;i<collplane.size();i++)
    collision::sphereplane(newpos,collplane[i].normal,collplane[i].p[0],collplane[i].p[1],collplane[i].p[2],collplane[i].p[3],cs.r);
  if(cam.getLocation().y!=newpos.y)
    isground=true;
  else
    isground=false;
  setPosition(newpos);

  weapons[curweapon]->setCurpos(newpos);
  weapons[curweapon]->setCurrot(cam.getVector());
  weapons[curweapon]->setPitchAndYaw(cam.getCamPitch(),cam.getCamYaw());
  weapons[curweapon]->update();
}
void player::show()
{
  if(isWeapon)
    weapons[curweapon]->show();
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
