#ifndef SOUND_H
#define SOUND_H 
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class sound{
  public:
    void loadSound(const std::string& file);
    void playSound(const std::string& soundd);
    void clean();
  private:
    std::unordered_map<std::string, Mix_Chunk*> sounds;
};

#endif
