#include "player.h"

player::player(const char* n,collisionsphere ccs,float sprints,float normals,float looks)
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
}
void player::show()
{

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

