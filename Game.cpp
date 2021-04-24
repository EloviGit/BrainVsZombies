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
	std::cout << "´íÎó£º\t" << info << std::endl;
}

void Game::resetGame(SceneType _scene) {
	for (std::vector<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); it++) {
		delete (*it);
	}

	for (std::vector<Plant*>::iterator it = plantList.begin(); it != plantList.end(); it++) {
		delete (*it);
	}

	while (!plantEffectList.empty())
	{
		PlantEffect* top = plantEffectList.top();
		delete top;
		plantEffectList.pop();
	}

	zombieList.clear();
	plantList.clear();

	gameClock = 0;
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
	plantEffectList.push(plantEffect);
}

void Game::addPlantEffects(std::vector<PlantEffect*> plantEffects) {
	std::vector<PlantEffect*>::iterator it = plantEffects.begin();
	for (; it != plantEffects.end(); it ++)
	{
		plantEffectList.push(*it);
	}
}

PlantEffect* Game::popLatestPlantEffect() {
	if (plantEffectList.empty()) {
		return nullptr;
	}
	else {
		PlantEffect* top = plantEffectList.top();
		plantEffectList.pop();
		return top;
	}
}


void Game::update() {
	gameClock++;

	while(currentEffect == nullptr && !plantEffectList.empty()) {
		currentEffect = popLatestPlantEffect();
		if (currentEffect == nullptr) {
			continue;
		}
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
		delete currentEffect;
		currentEffect = popLatestPlantEffect();
	}
}

void Game::update(int tick) {
	for (int i = 0; i < tick; i++)
		update();
}

Zombie* Game::findFastestZombie() {
	if (zombieList.empty()) {
		return nullptr;
	}
	else if (zombieList.size() == 1) {
		return *zombieList.begin();
	}
	else {
		Zombie* fastest = *zombieList.begin();
		for (std::vector<Zombie*>::iterator it = zombieList.begin() + 1; it<zombieList.end(); it++) {
			if (fastest->abscissa > (*it)->abscissa) {
				fastest = (*it);
			}
		}
		return fastest;
	}
}

Zombie* Game::findFastestZombie(ZombieType _type) {
	if (zombieList.empty()) {
		return nullptr;
	}
	else {
		Zombie* fastest = nullptr;
		for (std::vector<Zombie*>::iterator it = zombieList.begin(); it < zombieList.end(); it++) {
			if ((*it)->type == _type) {
				if (fastest == nullptr) {
					fastest = (*it);
				}
				else if (fastest->abscissa > (*it)->abscissa) {
					fastest = (*it);
				}
			}
		}
		return fastest;
	}
}

Zombie* Game::findSlowestZombie() {
	if (zombieList.empty()) {
		return nullptr;
	}
	else if (zombieList.size() == 1) {
		return *zombieList.begin();
	}
	else {
		Zombie* slowest = *zombieList.begin();
		for (std::vector<Zombie*>::iterator it = zombieList.begin() + 1; it < zombieList.end(); it++) {
			if (slowest->abscissa < (*it)->abscissa) {
				slowest = (*it);
			}
		}
		return slowest;
	}
}

Zombie* Game::findSlowestZombie(ZombieType _type) {
	if (zombieList.empty()) {
		return nullptr;
	}
	else {
		Zombie* slowest = nullptr;
		for (std::vector<Zombie*>::iterator it = zombieList.begin(); it < zombieList.end(); it++) {
			if ((*it)->type == _type) {
				if (slowest == nullptr) {
					slowest = *it;
				}
				else if (slowest->abscissa < (*it)->abscissa) {
					slowest = (*it);
				}
			}
		}
		return slowest;
	}
}
