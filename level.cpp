#include "level.h"

level::level(const char* c,unsigned int map,std::vector<collisionplane>& cplane,std::vector<vector3d>& sp,unsigned int l,unsigned int ba,unsigned int r,unsigned int f,unsigned int t,unsigned int bo)
{
  mesh=map;
  cp=cplane;
  spawnPoints=sp;
  name=c;
  skyb=std::make_unique<skybox>(l,ba,r,f,t,bo,200.0);
}

level::~level()
{

}

void level::update()
{

}

void level::show()
{
  glCallList(mesh);
}

std::vector<collisionplane>& level::getCollisionPlanes()
{
  return cp;
}

std::string& level::getName()
{
  return name;
}

void level::change(const char* c,unsigned int map,std::vector<collisionplane>& cplane,std::vector<vector3d>& sp)
{
  mesh=map;
  cp=cplane;
  spawnPoints=sp;
  name=c;
}

std::vector<vector3d>& level::getSpawnPoints()
{
  return spawnPoints;
}

skybox* level::getSkybox()
{
  //(from chatgpt): The get() function retrieves the raw pointer managed by std::unique_ptr without altering its ownership.
  return skyb.get();
}
