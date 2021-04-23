#include <iostream>
#include "Game.h"
#include "Zombie.h"
#include "judgement.h"

Game game;
ZombieProperty ZombieProperties[ZOMBIE_TYPE_NUM];

int initialize() {
    game.setScene(SceneType::PE);
    ReadZombieProperty();
}

int main()
{
    std::cout << "Hello World!\n";
}

