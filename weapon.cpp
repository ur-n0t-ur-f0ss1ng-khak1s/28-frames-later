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
