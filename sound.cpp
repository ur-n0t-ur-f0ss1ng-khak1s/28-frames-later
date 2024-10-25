#include "sound.h"

void sound::loadSound(const std::string& soundd)
{
  // Load sound and add to map
  std::string file = "data/wav/" + soundd + ".wav";
  Mix_Chunk* newSound = Mix_LoadWAV(file.c_str());
  if (!newSound) {
    std::cerr << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
  } else {
    sounds[soundd] = newSound;
  }
}

// Function to load and play a sound
void sound::playSound(const std::string& soundd)
{
  auto it = sounds.find(soundd);
  if (it != sounds.end())
  {
      Mix_PlayChannel(-1, it->second, 0);  // Play sound
  } else {
      std::cerr << "Sound" << soundd << "not found!" << std::endl;
  }
}

// Free the sound resources
void sound::clean()
{
  for (auto& pair : sounds)
  {
      Mix_FreeChunk(pair.second);
  }
}
