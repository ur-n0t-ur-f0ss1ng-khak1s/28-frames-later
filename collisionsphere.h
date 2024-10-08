#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H
#include "vector3d.h"

class collisionsphere{
  public:
    float r;
    vector3d center;
    collisionsphere();
    collisionsphere(const vector3d& vec, float rad);
};

#endif
