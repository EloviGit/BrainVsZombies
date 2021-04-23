#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

Game::Game(SceneType _scene, int _randomSeed)
{
	scene = _scene;
	if (_randomSeed == 0) {
		randomSeed = (unsigned int)(time(NULL));
	}
	else {
		randomSeed = _randomSeed;
	}
	srand(randomSeed);
	gameClock = 0;
	currentEffect = nullptr;
}

void Game::resetSeed(int seed) {
	randomSeed = seed;
	srand(randomSeed);
}

int Game::getRandomInt(int low, int high) {
	if (low >= high) {
		return low;
	}
	else {
		return (rand() % (high - low)) + low;
	}
}

float Game::getRandomFloat(float low, float high) {
	if (low >= high) {
		return low;
	}
	else {
		return low + (high - low) * (rand() / float(RAND_MAX));
	}
}

void Game::setClownExplode(bool toExplode) {
	clownExplode = toExplode;
}

void Game::reportError(std::string info) {
	std::cout << "����\t" << info << std::endl;
}

void Game::resetGame(SceneType _scene) {
	for (std::vector<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); it++) {
		delete (*it);
	}

	for (std::vector<Plant*>::iterator it = plantList.begin(); it != plantList.end(); it++) {
		delete (*it);
	}

	for (std::vector<PlantEffect*>::iterator it = plantEffectList.begin(); it != plantEffectList.end(); it++) {
		delete (*it);
	}

	zombieList.clear();
	plantList.clear();
	plantEffectList.clear();
}

void Game::addZombie(Zombie* zombie) {
	zombieList.push_back(zombie);
}

void Game::addZombies(std::vector<Zombie*> zombies) {
	zombieList.insert(zombieList.end(), zombies.begin(), zombies.end());
}

void Game::addPlant(Plant* plant) {
	plantList.push_back(plant);
}

void Game::addPlants(std::vector<Plant*> plants) {
	plantList.insert(plantList.end(), plants.begin(), plants.end());
}

void Game::addPlantEffect(PlantEffect* plantEffect) {
	plantEffectList.push_back(plantEffect);
	sort(plantEffectList.begin(), plantEffectList.end());
}

void Game::addPlantEffects(std::vector<PlantEffect*> plantEffects) {
	plantEffectList.insert(plantEffectList.end(), plantEffects.begin(), plantEffects.end());
	sort(plantEffectList.begin(), plantEffectList.end());
}

PlantEffect* Game::popLatestPlantEffect() {
	if (plantEffectList.empty()) {
		return nullptr;
	}
	else {
		return *(plantEffectList.erase(plantEffectList.begin()));
	}
}


void Game::update() {
	while(currentEffect == nullptr && !plantEffectList.empty()) {
		currentEffect = popLatestPlantEffect();
		if (currentEffect->time <= 0) {
			delete currentEffect;
			currentEffect = nullptr;
			continue;
		}
		if (currentEffect->time <= gameClock) {
			if (currentEffect->time == gameClock && currentEffect->effectType == EffectType::PLANT_EFFECT) {
				currentEffect->effect();
			}
			else if (currentEffect->time == gameClock && currentEffect->effectType == EffectType::BULLET_EFFECT) {
				break;
			}
			delete currentEffect;
			currentEffect = nullptr;
		}
	}

	for (std::vector<Plant*>::iterator it = plantList.begin(); it != plantList.end(); it++) {
		(*it)->update();
	}

	for (std::vector<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); it++) {
		(*it)->update();
	}

	while (currentEffect != nullptr) {
		if (currentEffect->time > gameClock) {
			break;
		}
		currentEffect->effect();
		delete	currentEffect;
		currentEffect = popLatestPlantEffect();
	}

	gameClock++;
}

void Game::update(int tick) {
	for (int i = 0; i < tick; i++)
		update();
}
