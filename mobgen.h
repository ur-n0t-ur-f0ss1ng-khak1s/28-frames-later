#ifndef MOBGEN_H
#define MOBGEN_H

#include "zombie.h"
#include "vector3d.h"
#include <vector>
#include <random>
#include <memory>

class mobGen {
public:
    mobGen(std::vector<std::shared_ptr<zombie>>& zombiesRef,const vector3d& corner1, const vector3d& corner2);
    
void spawnMob(std::vector<unsigned int>& anim);  // Spawns between 1 and 5 zombies at random positions within the area

private:
    std::vector<vector3d> spawnArea; // Holds the four corners defining the spawn area
    std::default_random_engine rng; // Random engine for spawning
    vector3d getRandomSpawnPosition(); // Returns a random position within the spawn  
    std::vector<std::shared_ptr<zombie>>& zombies;
};

#endif
