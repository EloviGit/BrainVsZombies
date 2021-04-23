#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

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

	for (std::vector<PlantEffect*>::iterator it = plantEffectList.begin(); it != plantEffectList.end(); it++) {
		delete (*it);
	}

	zombieList.clear();
	plantList.clear();
	plantEffectList.clear();
}
