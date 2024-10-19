#ifndef LEVEL_H
#define LEVEL_H
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include "collisionplane.h"
#include "vector3d.h"
#include "skybox.h"

class level{
  unsigned int mesh;
  std::vector<collisionplane> cp;
  std::vector<vector3d> spawnPoints;
  std::string name;
  std::unique_ptr<skybox> skyb;

  public:
  level(const char* c,unsigned int map,std::vector<collisionplane>& cplane,std::vector<vector3d>& sp,unsigned int l,unsigned int ba,unsigned int r,unsigned int f,unsigned int t,unsigned int bo);
  ~level();
  void update();
  void show();
  std::vector<collisionplane>& getCollisionPlanes();
  std::string& getName();
  void change(const char* c,unsigned int map,std::vector<collisionplane>& cplane,std::vector<vector3d>& sp);
  std::vector<vector3d>& getSpawnPoints();
  skybox* getSkybox();
};

#endif
