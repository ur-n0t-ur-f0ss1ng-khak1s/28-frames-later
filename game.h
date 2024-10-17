#ifndef GAME_H
#define GAME_H
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <SDL/SDL_mixer.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include "camera.h"
#include "objloader.h"
#include "vector3d.h"
#include "collisionplane.h"
#include "collisionsphere.h"
#include "functions.h"
#include "level.h"
#include "player.h"
#include "weapon.h"

class game{
  objloader obj;
  player* player1;
  std::vector<level> levels;
  std::vector<weapon> weapons;
  std::ofstream out; 
  void update();
  void show();
  unsigned int loadTexture(const char* filename);
  void loadAnimation(std::vector<unsigned int>& anim,std::string filename,int frames);
//  void loadAnimation(std::vector<unsigned int>& frames,std::string filename,unsigned int num);

  public:
    game();
    ~game();
    void start();
};

#endif
