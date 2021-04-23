#pragma once
#include <vector>
#include "Zombie.h"
#include "Plant.h"
#include "PlantEffect.h"

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

public:
	std::vector<Zombie*> zombieList;
	std::vector<Plant*> plantList;
	std::vector<PlantEffect*> plantEffectList;

	Game(SceneType _scene = SceneType::PE, int _randomSeed=0);

	SceneType getScene() { return scene; }
	void setScene(SceneType _scene) { scene = _scene; }

	int getSeed() { return randomSeed; }
	void resetSeed(int seed);
	int getRandomInt(int low, int high);			// low ~ high-1
	float getRandomFloat(float low, float high);	// low ~ high

	bool getClownExplode() { return clownExplode; }
	void setClownExplode(bool toExplode);

	void reportError(std::string info);

	void resetGame(SceneType _scene = SceneType::PE);
};
