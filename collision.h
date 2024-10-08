#ifndef COLLISION_H
#define COLLISION_H
#include "vector3d.h"

class collision{
  public:
    static bool raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r, float* dist=NULL,vector3d* point=NULL); //done
    static float rayplanedist(float nx,float ny,float nz,float x0,float y0,float z0,float xs,float ys,float zs,float xd,float yd,float zd);
    static bool rayplane(float nx, float ny, float nz, float xs, float ys, float zs, float xd, float yd, float zd, vector3d p1, vector3d p2, vector3d p3, vector3d p4, float* dist=NULL, vector3d* point=NULL); //done
    static float trianglearea(vector3d p1, vector3d p2, vector3d p3); //done
    static float pointdistacesquare(vector3d p1,vector3d p2);
    static bool spheresphere(vector3d& c1, float r1, vector3d c2, float r2); // done
    static float pointdistance(vector3d c1, vector3d c2);
    static bool sphereplane(vector3d& sp, vector3d vn, vector3d p1, vector3d p2, vector3d p3, vector3d p4, float r); //done
};

#endif
