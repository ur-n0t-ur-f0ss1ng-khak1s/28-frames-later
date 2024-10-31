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

class melee{
  bool isknifing;
  int delay;
  int wepDelay;
  int lastKnifing;
  vector3d loc;
  
  public:
    knife(std::vector<unsigned int>& f,int str,int del,int wdel,vector3d location);
    int setKnife(player* p,std::vector<zombie>& zombies);
    void update(player* p);
    void show();
    bool isKnifing();
    int getStrength();
    void test();
};

#endif
