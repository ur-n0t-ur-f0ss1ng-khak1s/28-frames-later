#ifndef GUN_H
#define GUN_H
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "vector3d.h"

class gun : public weapon {
  unsigned int outerview;
  unsigned int normalanimation;
  unsigned int fireanimation;
  unsigned int reloadanimation;

  vector3d position_expected;
  vector3d rotation_expected;

  vector3d direction = vector3d(0,0,0);
  vector3d aimposition; //when you are aiming
  vector3d aimrotation;

  float precision,aimprecision;
  bool isaim,isreloading,isautomatic,isfired,istest;

  unsigned int maxBulletsInMag;
  unsigned int bulletsInMag;
  unsigned int allbullets;

  unsigned int lastshot; //how many frames ago
  unsigned int speed; 

  unsigned int curmode; //1 - normal, 2 - fire, 3 - reload

  public:
    gun(std::vector<unsigned int>& anim,unsigned int ol,unsigned int na,unsigned int fa,unsigned int ra,vector3d ofset,vector3d pos,vector3d rot,vector3d apos,vector3d arot,float prec,float aprec,int str,int maxb,int allbul,int speedd,const char* namee,bool isa);
    ~gun();
    void update();
    void show();

    bool fire(vector3d camdirection);
    void stopfire();

    void reload();

    void test();

    void addBullets(unsigned int num);
    void setBullets(unsigned int num);

    int getAmmo();
    int getAllAmmo();
    vector3d getDirection();

    bool isAimed();
    unsigned int getOuterView();

    void aim();
};

#endif
