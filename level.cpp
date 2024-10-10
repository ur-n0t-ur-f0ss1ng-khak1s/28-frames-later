#include "level.h"

level::level(const char* c,unsigned int map,std::vector<collisionplane>& cplane,std::vector<vector3d>& sp)
{
  mesh=map;
  cp=cplane;
  spawnPoints=sp;
  name=c;
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
