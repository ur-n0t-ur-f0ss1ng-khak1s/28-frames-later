#ifndef MELEE_H
#define MELEE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <vector>
#include "player.h"
#include "zombie.h"
#include "vector3d.h"
#include "collision.h"

class melee : public weapon {
  bool isknifing;
  int delay;
  int wepDelay;
  int lastKnifing;
  
  public:
    melee(std::vector<unsigned int>& f,int str,int del,int wdel, vector3d offset=vector3d(-1.55,-1.11,2));
    int setKnife(vector3d playerLoc,vector3d playerDir,std::vector<std::shared_ptr<zombie>>& zombies);
    void update();
    void show();
    bool isKnifing();
    int getStrength();
    void test();
};

#endif
