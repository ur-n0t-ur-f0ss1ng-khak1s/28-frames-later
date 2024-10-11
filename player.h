#ifndef PLAYER_H
#define PLAYER_H
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include "vector3d.h"
#include "collisionplane.h"
#include "collisionsphere.h"
#include "collision.h"
#include "camera.h"

class player{
  std::string name;
  collisionsphere cs;
  int health;
  vector3d force; //gravity
  vector3d direction;
  bool isground,iscollision,issprint;
  float sprintspeed,normalspeed;
  float lookspeed;
  float energy;
  int points;
  public:
    camera cam;
    player(const char* n,collisionsphere ccs,float sprints,float normals,float looks);
    ~player();
    void update(std::vector<collisionplane>& collplane);
    void show();
    void setPosition(vector3d pos);

    //getters
    std::string getName();
    int getHealth();
    collisionsphere getCollisionSphere();
    int getPoints();

    //setters
    void setHealth(int h);
    void addPoints(int n);
};

#endif
