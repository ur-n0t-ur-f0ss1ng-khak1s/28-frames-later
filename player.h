#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include <memory>
#include "vector3d.h"
#include "collisionplane.h"
#include "collisionsphere.h"
#include "collision.h"
#include "weapon.h"
#include "camera.h"

class player:private collision {
  std::string name;
  collisionsphere cs;
  int health;
  vector3d force; //gravity
  vector3d direction;
  bool isground,iscollision,issprint;
  float sprintspeed,normalspeed;
  float lookspeed;
  float energy;
  int points;
  std::vector<std::shared_ptr<weapon>> weapons;
  int curweapon;
  bool isWeapon;
  int headPosition;
  bool headUp;
	Uint32 lifeTime;
  public:
    camera cam;
    player(const char* n,collisionsphere ccs,float sprints,float normals,float looks,std::shared_ptr<weapon> wep);
    ~player();
    void update(std::vector<collisionplane>& collplane);
    void show();
    void setPosition(vector3d pos);

    //getters
    std::string getName();
    int getHealth();
    collisionsphere getCollisionSphere();
    int getPoints();
    camera* getCamera();
    std::shared_ptr<weapon> getCurrentWeapon();
    Uint32 getLifeTime();

    //setters
    void setHealth(int h);
    void addPoints(int n);

    void addWeapon(std::shared_ptr<weapon> wep);
    void changeWeapon(int num);
    void changeWeaponDown();
    void changeWeaponUp();
    void haveWeapon(bool b);

    void setJump();

    void setSprint();
    void stopSprint();
    void setLifeTime(Uint32 newLifeTime);
};

#endif
