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
    std::string name;
    float pitch; //direction relative to camera and player orientation
    float yaw;

    vector3d offset; //each weapon needs to be offset down and to the left or right from the player's view

    vector3d position; //when you are not aiming
    vector3d rotation;

    vector3d curpos;
    vector3d currot;

  public:
    weapon(std::vector<unsigned int>& animFrames, int str, vector3d pos=vector3d(0,0,0), vector3d rot=vector3d(0,0,0),vector3d offset=vector3d(-1,-1.5,4.5))
        : frames(animFrames), strength(str), curframe(0), position(pos),rotation(rot),pitch(0.0),yaw(0.0),offset(offset) {
        curpos=position;
        currot=rotation;
    }

    virtual ~weapon() = default;

    void setCurpos(vector3d newpos);
    void setCurrot(vector3d newrot);
    void setPitchAndYaw(float newpitch, float newyaw);

    vector3d getPosition() const { return position; }
    int getStrength();
    std::string getName();
    std::vector<unsigned int>& getAnimation();
};

#endif
