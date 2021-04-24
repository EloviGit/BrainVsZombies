#pragma once
#include <vector>
#include "Zombie.h"
#include "Plant.h"
#include "PlantEffect.h"
#include <queue>

enum class SceneType {
	DE = 0,
	NE,
	PE,
	FE,
	RE,
	ME = 5,
};

class Game
{
	SceneType scene;
	int randomSeed;
	bool clownExplode = false;
	int gameClock;

public:
	std::vector<Zombie*> zombieList;
	std::vector<Plant*> plantList;
	std::priority_queue<PlantEffect*, std::vector<PlantEffect*>, std::greater<PlantEffect*>> plantEffectList;

	Game(SceneType _scene = SceneType::PE, int _randomSeed=0);

	SceneType getScene() { return scene; }
	void setScene(SceneType _scene) { scene = _scene; }

	int getSeed() { return randomSeed; }
	void resetSeed(int seed);
	int getRandomInt(int low, int high);			// low ~ high-1
	float getRandomFloat(float low, float high);	// low ~ high

	bool getClownExplode() { return clownExplode; }
	void setClownExplode(bool toExplode);

	int getGameClock() { return gameClock; }

	void reportError(std::string info);

	void resetGame(SceneType _scene = SceneType::PE);

	void addZombie(Zombie* zombie);
	void addZombies(std::vector<Zombie*> zombies);
	void addPlant(Plant* plant);
	void addPlants(std::vector<Plant*> plants);
	void addPlantEffect(PlantEffect* plantEffect);
	void addPlantEffects(std::vector<PlantEffect*> plantEffects);
	void recycleTopPlantEffect();

	void update();
	void update(int tick);

	Zombie* findFastestZombie();
	Zombie* findFastestZombie(ZombieType _type);
	Zombie* findSlowestZombie();
	Zombie* findSlowestZombie(ZombieType _type);
};
