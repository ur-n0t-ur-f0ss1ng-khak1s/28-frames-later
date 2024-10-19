#ifndef ITEM_H
#define ITEM_H
#include "collision.h"
#include "collisionsphere.h"
#include "vector3d.h"
#include <GL/gl.h>
#include <iostream>
#include <vector>

class oneItem{
  public:
    vector3d rotation,scale;
    collisionsphere cs;
    int id;
    unsigned int mesh;
    oneItem(const vector3d& rot,const vector3d& sc,const collisionsphere& ccs,int idnum,unsigned int m);
};

class item{
  std::vector<oneItem> items;
  public:
    void add(const vector3d& rot,const vector3d& sc,const collisionsphere& ccs,int idnum,unsigned int m);
    void del(int id);
    int update(collisionsphere playerpos);
    void show();
};

#endif
