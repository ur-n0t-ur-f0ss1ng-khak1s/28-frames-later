#ifndef TEXT_H
#define TEXT_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include "vector3d.h"
#include "collisionplane.h"

class text{
  std::vector<unsigned int> characters;
  float width,height;
  int screenWidth,screenHeight;
  bool istest;
  bool islocked = false;
  vector3d changepos,changerot,changescale;
  vector3d offset; 

  public:
    text(std::vector<unsigned int>& ch,float w,float h,int screenW,int screenH);
    void assignFont(std::vector<unsigned int>& ch,float w, float h);
    void sdlDrawText(SDL_Renderer* renderr,TTF_Font* font,int x,int y,const char* tex);
    void drawOrtho(int posX,int posY, const char* text,float scale=10.0f);
    void fillScreenOrtho(const char* text,float scale=10.0f,float Rgb=0.0f,float rGb=0.0f,float rgB=0.0f);

    void drawText(vector3d pos,vector3d rot,vector3d scale,const char* tex,float yaw,float pitch);

    void test(vector3d& scale);
    void setTest(bool b);
    std::vector<unsigned int>& getCharacters();
};
#endif
