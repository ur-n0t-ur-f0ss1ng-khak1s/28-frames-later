#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include "vector3d.h"
#include "collisionsphere.h"
#include "collisionplane.h"
#include "collision.h"
#include <vector>
#include <cmath>
#include <iostream>

class zombie{
  std::vector<unsigned int> frames;
  unsigned int walk,attack,die;
  unsigned int curframe;

  int health,strength;
  float speed;
  vector3d direction,rotation;
  bool iswalk,isattack,isdead;

  collisionsphere cs;

  public:
    zombie(std::vector<unsigned int>& anim,unsigned int w, unsigned int a, unsigned int d,int h,int str,float sp,collisionsphere ccs);
    ~zombie () {}
    bool update(std::vector<collisionplane>& col,vector3d playerloc);
    void show();
    bool setAttack(collisionsphere player);
    collisionsphere* getCollisionSphere();
    void setLocation(vector3d newloc);
    vector3d getLocation();
    void decreaseHealth(int num);
    int getHealth();
    int getStrength();
    bool isDead();
};

#endif
