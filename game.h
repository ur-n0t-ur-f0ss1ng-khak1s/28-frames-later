#ifndef GAME_H
#define GAME_H
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <memory>
#include "objloader.h"
#include "vector3d.h"
#include "collisionplane.h"
#include "collisionsphere.h"
#include "functions.h"
#include "camera.h"
#include "level.h"

class game{
  objloader obj;
  camera cam;
  std::vector<std::unique_ptr<level>> levels;

  void update();
  void show();
  unsigned int loadTexture(const char* filename);
  void loadAnimation(std::vector<unsigned int>& anim,std::string filename,int frames);

  public:
    game();
    ~game();
    void start();
};

#endif
