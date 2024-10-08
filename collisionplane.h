#ifndef COLLISIONPLANE_H
#define COLLISIONPLANE_H
#include "vector3d.h"

class collisionplane{
  public:
    vector3d p[4];
    vector3d normal;
    collisionplane(float n1, float n2, float n3, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l);
};

#endif
