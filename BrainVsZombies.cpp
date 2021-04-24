#include <iostream>
#include "Game.h"
#include "Zombie.h"
#include "Plant.h"
#include <fstream>
#include <sstream>

Game game;
ZombieProperty ZombieProperties[ZOMBIE_TYPE_NUM];

void initialize() {
    game.setScene(SceneType::PE);
    ReadZombieProperty();
}

void getFastChart(int iceTime=0, int endTime=3000) {
    game.resetGame();
    ZombieType types[9] = {
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

    std::stringstream fast_filename;
    fast_filename << "output/iced_fast_"<< iceTime <<".csv";
    std::ofstream fastfile;
    fastfile.open(fast_filename.str(), std::ios::out|std::ios::trunc);
    fastfile << "time" << ",";

    if (iceTime > 0) {
        game.addPlantEffect(new Ice(iceTime, IceEffectType::FAST));
    }

    for (int i = 0; i < 9; i++) {
        ZombieType type = types[i];
        ZombieProperty* prop = ZombieProperties + int(type);
        fastfile << prop->name << ",";

        switch (prop->movementType)
        {
        case MOVE_EVEN_RELA_SPEED_RANDOM:
            game.addZombie(new Zombie(type, 0, prop->spawnAbscissaLower, prop->relativeSpeedHigher));
            break;
        case MOVE_UNEVEN_RELA_SPEED_RANDOM:
            for (int i = 0; 0.0001 * i + prop->relativeSpeedLower <= prop->relativeSpeedHigher; i++)
            {
                game.addZombie(new Zombie(type, 0, prop->spawnAbscissaLower, 0.0001 * i + prop->relativeSpeedLower));
            }
            break;
        case MOVE_ZOMBONI:
            game.addZombie(new Zombie(type, 0, prop->spawnAbscissaLower));
            break;
        default:
            break;
        }
    }
    fastfile << std::endl;

    for (int time = 0; time <= endTime; time++) {
        fastfile << time << ",";
        for (int i = 0; i < 9; i++)
        {
            ZombieType type = types[i];
            Zombie* fastZombie = game.findFastestZombie(type);
            if (fastZombie == nullptr) {
                fastfile << -1 << ",";
            }
            else {
                fastfile << fastZombie->abscissa << ",";
            }
        }
        fastfile << std::endl;
        game.update();
    }

    fastfile.close();
}

void getSlowChart(int iceTime=0, int endTime=3000) {
    game.resetGame();
    ZombieType types[9] = {
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

    std::stringstream slow_filename;
    std::ofstream slowfile;
    slow_filename << "output/iced_slow_" << iceTime << ".csv";
    slowfile.open(slow_filename.str(), std::ios::out | std::ios::trunc);
    slowfile << "time" << ",";

    if (iceTime > 0) {
        game.addPlantEffect(new Ice(iceTime, IceEffectType::FAST));
    }

    for (int i = 0; i < 9; i++) {
        ZombieType type = types[i];
        ZombieProperty* prop = ZombieProperties + int(type);
        slowfile << prop->name << ",";

        switch (prop->movementType)
        {
        case MOVE_EVEN_RELA_SPEED_RANDOM:
            game.addZombie(new Zombie(type, 0, prop->spawnAbscissaHigher, prop->relativeSpeedLower));
            break;
        case MOVE_UNEVEN_RELA_SPEED_RANDOM:
            for (int i = 0; 0.0001 * i + prop->relativeSpeedLower <= prop->relativeSpeedHigher; i++)
            {
                game.addZombie(new Zombie(type, 0, prop->spawnAbscissaHigher, 0.0001 * i + prop->relativeSpeedLower));
            }
            break;
        case MOVE_ZOMBONI:
            game.addZombie(new Zombie(type, 0, prop->spawnAbscissaHigher));
            break;
        default:
            break;
        }
    }
    slowfile << std::endl;

    for (int time = 0; time <= endTime; time++) {
        slowfile << time << ",";
        for (int i = 0; i < 9; i++)
        {
            ZombieType type = types[i];
            Zombie* slowZombie = game.findSlowestZombie(type);
            if (slowZombie == nullptr) {
                slowfile << -1 << ",";
            }
            else {
                slowfile << slowZombie->abscissa << ",";
            }
        }
        slowfile << std::endl;
        game.update();
    }

    slowfile.close();
}

int main()
{
    initialize();
    getFastChart(0);
    getFastChart(1);
    getSlowChart(0);
    getSlowChart(1);
}

