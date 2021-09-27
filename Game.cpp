#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>

Game::Game(SceneType _scene, int _randomSeed) {
	scene = _scene;
	if (_randomSeed == 0) {
		randomSeed = (unsigned int)(time(NULL));
	}
	else {
		randomSeed = _randomSeed;
	}
	srand(randomSeed);
	gameClock = 0;
}

Game::~Game() {
	if (debugMode != DebugMode::OFF) {
		logFile.close();
	}
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
	std::cout << "Error:\t" << info << std::endl;
}

void Game::resetGame(SceneType _scene) {
	for (std::vector<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); it++) {
		delete (*it);
	}

	for (std::vector<Plant*>::iterator it = plantList.begin(); it != plantList.end(); it++) {
		delete (*it);
	}

	while (!plantEffectList.empty()) {
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
	for (; it != plantEffects.end(); it++) {
		plantEffectList.push(*it);
	}
}

void Game::recycleTopPlantEffect() {
	if (!plantEffectList.empty()) {
		PlantEffect* top = plantEffectList.top();
		plantEffectList.pop();
		delete top;
	}
}

void Game::update() {
	gameClock++;

	while (!plantEffectList.empty()) {
		if (plantEffectList.top()->time < gameClock) {
			recycleTopPlantEffect();
		}
		else if (plantEffectList.top()->time == gameClock) {
			if (plantEffectList.top()->effectType == EffectType::PLANT_EFFECT) {
				plantEffectList.top()->effect();
			}
			else {
				// BULLET_EFFECT
				break;
			}
			recycleTopPlantEffect();
		}
		else {
			// plantEffectList.top()->time > gameClock
			break;
		}
	}

	for (std::vector<Plant*>::iterator it = plantList.begin(); it != plantList.end(); it++) {
		(*it)->update();
	}

	for (std::vector<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); it++) {
		(*it)->update();
	}

	while (!plantEffectList.empty()) {
		if (plantEffectList.top()->time > gameClock) {
			break;
		}
		plantEffectList.top()->effect();
		recycleTopPlantEffect();
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
		for (std::vector<Zombie*>::iterator it = zombieList.begin() + 1; it < zombieList.end(); it++) {
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

Zombie* Game::findZombieById(int _id) {
	if (!zombieList.empty()) {
		for (std::vector<Zombie*>::iterator it = zombieList.begin(); it < zombieList.end(); it++) {
			if ((*it)->id == _id) {
				return (*it);
			}
		}
	}
	return nullptr;
}

std::string Game::getTimestamp() {
	auto t = std::time(nullptr);
	struct tm buf;
	localtime_s(&buf, &t);
	std::ostringstream oss;
	oss << std::put_time(&buf, "%Y-%m-%d_%H.%M.%S");
	return oss.str();
}

void Game::setDebug(bool _debug) {
	if (_debug) {
		if (debugMode == DebugMode::OFF) {
			logFile.open(logFilename, std::ios::out | std::ios::trunc);
			logFile << "clock,category,id,exist_time/type,row,col,absc,ord,state,eat/dmg,state_countdown,freeze_countdown,iced_countdown,\n";
			debugMode = DebugMode::ACTIVE;
		}
		else if (debugMode == DebugMode::INACTIVE) debugMode = DebugMode::ACTIVE;
	}
	else {
		if (debugMode == DebugMode::ACTIVE) debugMode = DebugMode::INACTIVE;
	}
}