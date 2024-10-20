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
#include "level.h"
#include "player.h"
#include "weapon.h"
#include "zombie.h"
#include "item.h"

class game{
  std::ofstream out; 
  objloader obj;
  std::unique_ptr<player> player1;
  std::vector<std::shared_ptr<level>> levels;
  std::vector<std::shared_ptr<weapon>> weapons;
  std::vector<std::shared_ptr<zombie>> zombies;
  std::vector<unsigned int> goldenApples;
  unsigned int testgreen;
  item items;
  //unsigned int goldenApple;

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
