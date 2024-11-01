#include "weapon.h"

int weapon::getStrength()
{
  return strength;
}

std::string weapon::getName()
{
  return name;
}

std::vector<unsigned int>& weapon::getAnimation()
{
  return frames;
}

void weapon::setCurpos(vector3d newpos)
{
  curpos = newpos;
}

void weapon::setCurrot(vector3d newrot)
{
  currot = newrot;
}

void weapon::setPitchAndYaw(float newpitch, float newyaw)
{
  pitch=newpitch;
  yaw=newyaw;
}

