#include <iostream>
#include "Game.h"
#include "Zombie.h"
#include "Plant.h"
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <iomanip>
//#include <vector>

Game game;
ZombieProperty ZombieProperties[ZOMBIE_TYPE_NUM];

void initialize(bool debug = false) {
	std::cout << "Initializing..." << std::endl;
	game.setScene(SceneType::PE);
	ReadZombieProperty();
}

void getFastChart(int iceTime = 0, int endTime = 3000) {
	int startRunningTime = GetTickCount64();
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
	fast_filename << "output/iced_fast_" << iceTime << ".csv";
	std::ofstream fastfile;
	fastfile.open(fast_filename.str(), std::ios::out | std::ios::trunc);
	fastfile << "time" << ",";

	if (iceTime > 0) {
		game.addPlantEffect(new Ice(iceTime, IceEffectType::FAST));
	}

	for (int i = 0; i < 9; i++) {
		ZombieType type = types[i];
		ZombieProperty* prop = ZombieProperties + int(type);
		fastfile << prop->name << ",";

		switch (prop->movementType) {
			case MOVE_EVEN_RELA_SPEED_RANDOM:
				game.addZombie(new Zombie(type, 0, prop->spawnAbscissaLower, prop->relativeSpeedHigher));
				break;
			case MOVE_UNEVEN_RELA_SPEED_RANDOM:
				for (int i = 0; 0.0001 * i + prop->relativeSpeedLower <= prop->relativeSpeedHigher; i++) {
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

	while (game.getGameClock() <= endTime) {
		fastfile << game.getGameClock() << ",";
		if (game.getGameClock() % int(endTime / 10) == 0) {
			int ratio = game.getGameClock() / int(endTime / 10);
			std::cout << ratio * 10 << "% completed." << std::endl;
		}
		for (int i = 0; i < 9; i++) {
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

	int endRunningTime = GetTickCount64();
	std::cout << "Time elapsed: " << (endRunningTime - startRunningTime) << "ms." << std::endl;
}

void getSlowChart(int iceTime = 0, int endTime = 3000) {
	int startRunningTime = GetTickCount64();
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

		switch (prop->movementType) {
			case MOVE_EVEN_RELA_SPEED_RANDOM:
				game.addZombie(new Zombie(type, 0, prop->spawnAbscissaHigher, prop->relativeSpeedLower));
				break;
			case MOVE_UNEVEN_RELA_SPEED_RANDOM:
				for (int i = 0; 0.0001 * i + prop->relativeSpeedLower <= prop->relativeSpeedHigher; i++) {
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

	while (game.getGameClock() <= endTime) {
		slowfile << game.getGameClock() << ",";

		if (game.getGameClock() % int(endTime / 10) == 0) {
			int ratio = game.getGameClock() / int(endTime / 10);
			std::cout << ratio * 10 << "% completed." << std::endl;
		}

		for (int i = 0; i < 9; i++) {
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

	int endRunningTime = GetTickCount64();
	std::cout << "Time elapsed: " << (endRunningTime - startRunningTime) << "ms." << std::endl;
}

void test() {
	game.resetGame();
	Zombie* poleVault = new Zombie(ZombieType::POLE_VAULTING, 0, 870, 0.66);
	game.addZombie(poleVault);
	game.addPlantEffect(new Ice(1, IceEffectType::FAST));
	game.addPlantEffect(new Ice(500, IceEffectType::FAST));
	game.addPlantEffect(new Ice(1000, IceEffectType::FAST));
	while (game.getGameClock() <= 2000) {
		std::cout << game.getGameClock() << ", ";
		std::cout << poleVault->abscissa << ", " << poleVault->freezeCountdown
			<< ", " << poleVault->icedCountdown << std::endl;
		game.update();
	}
}

// 测试小丑在指定时间段内的炸炮数
void testJackTime(int sample_size, size_t start_tick, size_t end_tick, int ice_time = 1, int dc_time = -1, bool debug = false) {
	int startRunningTime = GetTickCount64();
	const int ZOM_NUM = 5;
	std::vector<ZombieType> types{ ZombieType::JACK_IN_THE_BOX, ZombieType::LADDER, };
	std::vector<int> cob_rows{ 0, 1, 4, 5 };
	std::vector<int> zom_rows{ 0, 1, 4, 5 };
	std::vector<int> loss_counter(end_tick - start_tick), hp_counter(end_tick - start_tick);
	std::vector<std::vector<int>> explodeInfo(cob_rows.size(), std::vector<int>(zom_rows.size()));

	std::stringstream filename;
	std::ofstream file;
	filename << "output/" << ice_time << "冰 " << dc_time << "垫 n=" << sample_size << ".csv";
	file.open(filename.str(), std::ios::out | std::ios::trunc);

	if (debug) game.setClownEarlyExplode(1);
	game.setDebug(debug);
	int divide = sample_size / 10 == 0 ? 1 : sample_size / 10;
	for (int l = 0; l < sample_size; l++) {
		game.resetGame();

		// 创建植物
		for (int i = 0; i < cob_rows.size(); i++) {
			game.addPlant(new Plant(PlantType::COB_CANNON, cob_rows[i], 6, PLANT_STATE_INVINCIBLE));
		}

		// 创建僵尸
		for (int i = 0; i < types.size(); i++) {
			ZombieType type = types[i];
			ZombieProperty* prop = ZombieProperties + int(type);
			for (int j = 0; j < ZOM_NUM; j++) {
				game.addZombie(new Zombie(type, zom_rows[game.getRandomInt(0, zom_rows.size())]));
			}
		}

		game.addPlantEffect(new Ice(ice_time, IceEffectType::RANDOM));

		// 测试
		if (dc_time == -1) {
			game.update(start_tick);
		}
		else {
			game.update(dc_time);
			for (int i = 0; i < cob_rows.size(); i++) {
				game.addPlant(new Plant(PlantType::NORMAL, cob_rows[i], 8, PLANT_STATE_INVINCIBLE));
			}
			game.update(266);
			for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
				if ((*it)->getType() == PlantType::NORMAL) {
					(*it)->shovel();
				}
			}
			game.update(start_tick - dc_time - 266);
		}

		for (int t = 0; t < end_tick - start_tick; t++) {
			int sum = 0;
			int hp_loss = 0;
			for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
				if ((*it)->getType() == PlantType::COB_CANNON) {
					if ((*it)->isDisappeared()) sum++;
					else hp_loss += (*it)->getDamage();
				}
			}
			loss_counter[t] += sum;
			hp_counter[t] += hp_loss;
			game.update();
		}

		// 记录爆炸信息
		for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
			if ((*it)->getType() == PlantType::COB_CANNON && (*it)->getState() == PLANT_STATE_EXPLODED) {
				auto cob_index = std::find(cob_rows.begin(), cob_rows.end(), (*it)->row);
				if (cob_index == cob_rows.end()) continue;
				Zombie* zom = game.findZombieById((*it)->getExplodeID());
				if (zom == nullptr) continue;
				auto zom_index = std::find(zom_rows.begin(), zom_rows.end(), zom->row);
				if (zom_index == zom_rows.end()) continue;
				explodeInfo[cob_index - cob_rows.begin()][zom_index - zom_rows.begin()]++;
			}
		}

		if (l % divide == 0) {
			std::cout << l + 1 << " completed\n";
		}
	}

	// 输出
	file << ",";
	for (auto r : zom_rows) file << r << ",";
	file << "\n";
	for (int i = 0; i < cob_rows.size(); i++) {
		file << cob_rows[i] << ",";
		for (auto r : explodeInfo[i]) file << r << ",";
		file << "\n";
	}
	file << "time,cob_loss,hp_loss,num,avg_loss\n";
	for (int i = 0; i < end_tick - start_tick; i++) {
		file << start_tick + i << "," << loss_counter[i] << "," << hp_counter[i]
			<< "," << sample_size * cob_rows.size() - loss_counter[i] << "," << double(hp_counter[i]) / (sample_size * cob_rows.size() - loss_counter[i]) << ",\n";
	}
	file.close();
	int endRunningTime = GetTickCount64();
	std::cout << "Time elapsed: " << (endRunningTime - startRunningTime) / 1000.0 << "s." << std::endl;
}

int main() {
	initialize();
	int simulationTime = 3000;
	//getFastChart(0, simulationTime);
	//getFastChart(1, simulationTime);
	//getSlowChart(0, simulationTime);
	//getSlowChart(1, simulationTime);
	//getFastChart(11, simulationTime);
//    test();

	testJackTime(10000, 1200, 1500, 1, 770, false);
	testJackTime(10000, 1200, 1500, 1, 824, false);

	system("pause");
}

