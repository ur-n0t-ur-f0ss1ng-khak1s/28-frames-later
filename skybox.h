#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/gl.h>

class skybox{
  enum {SKY_LEFT=0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM};	//constants for the skybox faces, so we don't have to remember too many ints
  public:
    unsigned int side[6];
    float size;
    skybox();
    skybox(unsigned int left,unsigned int back,unsigned int right,unsigned int front,unsigned int top,unsigned int bottom,float s);
    ~skybox();
    void drawSkybox();
};

#endif
