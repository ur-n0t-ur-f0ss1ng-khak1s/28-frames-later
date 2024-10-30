#include "mobgen.h"
#include <iostream>
#include <ctime>

mobGen::mobGen(std::vector<std::shared_ptr<zombie>>& zombiesRef,const vector3d& corner1, const vector3d& corner2)
  : zombies(zombiesRef),        // Initialize the zombies reference here
    spawnArea{corner1, corner2}, // Initialize spawnArea with the corners
    rng(static_cast<unsigned>(std::time(nullptr))) // Seed rng in the initializer list
{
    // Constructor body, if needed
}

vector3d mobGen::getRandomSpawnPosition() {
    // Calculate random x and y within the area defined by the four corners
    std::uniform_real_distribution<float> distX(spawnArea[0].x, spawnArea[1].x);
    std::uniform_real_distribution<float> distY(spawnArea[0].y, spawnArea[1].y);
    float zSpawnHeight = spawnArea[0].z + 5.0f; // Adjust z for the height of the spawn

    return vector3d(distX(rng), distY(rng), zSpawnHeight);
}

void mobGen::spawnMob(std::vector<unsigned int>& anim) {
    std::uniform_int_distribution<int> distNumZombies(1, 5);
    int numZombies = distNumZombies(rng);

    for (int i = 0; i < numZombies; ++i) {
        vector3d spawnPosition = getRandomSpawnPosition();
        std::cout << "Spawning zombie at: (" << spawnPosition.x << ", " 
                  << spawnPosition.y << ", " << spawnPosition.z << ")\n";
        // Here, you’d create a new zombie entity at spawnPosition
        
        zombies.push_back(std::make_shared<zombie>(anim,30,20,10,200,5,0.2,collisionsphere(spawnPosition,2.0)));
    }
}
