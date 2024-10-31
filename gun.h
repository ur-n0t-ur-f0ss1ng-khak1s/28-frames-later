#ifndef GUN_H
#define GUN_H
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "vector3d.h"

class gun{
  unsigned int outerview;
  unsigned int normalanimation;
  unsigned int fireanimation;
  unsigned int reloadanimation;

  float pitch; //direction relative to camera and player orientation
  float yaw;

  vector3d position_expected;
  vector3d rotation_expected;

  vector3d offset; //each weapon needs to be offset down and to the left or right from the player's view

  vector3d position; //when you are not aiming
  vector3d rotation;

  vector3d aimposition; //when you are aiming
  vector3d aimrotation;

  vector3d curpos;
  vector3d currot;

  float precision,aimprecision;
  bool isaim,isreloading,isautomatic,isfired,istest;

  unsigned int maxBulletsInMag;
  unsigned int bulletsInMag;
  unsigned int allbullets;

  unsigned int lastshot; //how many frames ago
  unsigned int speed; 

  unsigned int curmode; //1 - normal, 2 - fire, 3 - reload

  public:
    weapon(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d ofset,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int str,int maxb,int allbul,int speedd,const char* namee,bool isa);
    ~weapon();
    void update();
    void show();

    bool fire(vector3d& direction,vector3d camdirection);
    void stopfire();

    void reload();

    void test();

    void addBullets(unsigned int num);
    void setBullets(unsigned int num);

    int getAmmo();
    int getAllAmmo();
    void setCurpos(vector3d newpos);
    void setCurrot(vector3d newrot);
    void setPitchAndYaw(float newpitch, float newyaw);

    bool isAimed();
    unsigned int getOuterView();

    void aim();
};

#endif
