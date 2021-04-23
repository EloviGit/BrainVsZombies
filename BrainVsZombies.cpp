#include <iostream>
#include "Game.h"
#include "Zombie.h"
#include "judgement.h"
#include <fstream>

Game game;
ZombieProperty ZombieProperties[ZOMBIE_TYPE_NUM];

int initialize() {
    game.setScene(SceneType::PE);
    ReadZombieProperty();
}

void getSpeedChart(int iceTime) {
    game.resetGame();
    ZombieType types[] = {
        ZombieType::POLE_VAULTING,
        ZombieType::FOOTBALL,
        ZombieType::ZOMBONI,
        ZombieType::JACK_IN_THE_BOX,
        ZombieType::BALLON,
        ZombieType::DIGGER,
        ZombieType::LADDER,
        ZombieType::CATAPULT,
        ZombieType::GARGANTUAR,
    };

    std::ofstream outfile;



}

int main()
{
    initialize();
}

