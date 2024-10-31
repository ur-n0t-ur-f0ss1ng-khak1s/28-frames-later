#ifndef WEAPON_H
#define WEAPON_H
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "vector3d.h"

class weapon{
  protected:
    std::vector<unsigned int> frames;
    unsigned int curframe;
    int strength;
    vector3d position;
    std::string name;

  public:
    weapon(std::vector<unsigned int>& animFrames, int str, vector3d pos)
        : frames(animFrames), strength(str), curframe(0), position(pos) {}

    virtual ~weapon() = default;

    virtual void attack() = 0; // Abstract method for specific attack behavior
    void setPosition(const vector3d& pos) { position = pos; }
    vector3d getPosition() const { return position; }

    int getStrength();
    std::string getName();
    std::vector<unsigned int>& getAnimation();
};

#endif
