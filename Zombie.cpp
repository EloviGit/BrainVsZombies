#include "Zombie.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "judgement.h"
#include "Plant.h"
#include "Game.h"

extern ZombieProperty ZombieProperties[ZOMBIE_TYPE_NUM];
extern Game game;

void Zombie::update() {
	if (state == STATE_DEAD) { return; }

	if (game.getScene() == SceneType::RE || game.getScene() == SceneType::ME) {
		if (int(abscissa) <= 400) {
			roofHeight = (400 - int(abscissa)) / 4.0;
		}
		else {
			roofHeight = 0;
		}
	}

	// 首先更新运动
	if (state == STATE_WALKING) {
		if (movementType() == MOVE_EVEN_RELA_SPEED_RANDOM) {
			switch (iceType())
			{
			case ICE_NORMAL:
				// 篮球
				if (!freezeCountdown) {
					if (icedCoutdown) {
						xspeed = -0.4 * relativeSpeed;
					}
					else {
						xspeed = -relativeSpeed;
					}
					abscissa += xspeed;
				}
				break;
			case ICE_NOT_FREEZE:
				// 气球
				if (icedCoutdown) {
					xspeed = -0.4 * relativeSpeed;
				}
				else {
					xspeed = -relativeSpeed;
				}
				abscissa += xspeed;
				break;
			case ICE_NONE:
				// 矿工
				xspeed = -relativeSpeed;
				abscissa += xspeed;
				break;
			default:
				break;
			}
		}
		else if (movementType() == MOVE_UNEVEN_RELA_SPEED_RANDOM) {
			// 撑杆橄榄小丑梯子巨人
			if (!freezeCountdown && !isEating) {
				if (icedCoutdown) {
					xspeed = - 0.5 * prop->shift[int(anim_progress * prop->shiftSize)] * replacement;
					anim_progress += 0.5 * progress_delta;
					if (anim_progress >= 1) {
						anim_progress -= 1;
					}
				}
				else {
					xspeed = - prop->shift[int(anim_progress * prop->shiftSize)] * replacement;
					anim_progress += progress_delta;
					if (anim_progress >= 1) {
						anim_progress -= 1;
					}

				}
				abscissa += xspeed;
			}
		}
		else if (movementType() == MOVE_ZOMBONI) {
			if (abscissa < 400) {
				xspeed = -0.1;
			}
			else if (abscissa > 700) {
				xspeed = -0.25;
			}
			else {
				xspeed = -(int(abscissa - 700) / 2000.0 + 0.25);
			}
			abscissa += xspeed;
		}
		// 其他情况暂时不在考虑范围内
	}
	else if (state == STATE_IMP_FLYING) {
		hspeed += haccelaration;
		abscissa += xspeed;
		height += hspeed;
		if (game.getScene() == SceneType::RE || game.getScene() == SceneType::ME) {
			if (int(height + roofHeight) < 0)
			{
				state = STATE_IMP_LANDING;
				height = 0;
				stateCountDown = 50;
			}
		}
		else {
			if (int(height) < 0) {
				state = STATE_IMP_LANDING;
				height = 0;
				stateCountDown = 50;
			}
		}
	}
	else if (state == STATE_IMP_LANDING)
	{
		// 小鬼的落地过程实际由动画进度完成，但我并不清楚细节
		// 暂时仅用减速50cs，原速25cs代替，可能造成1cs的误差
		if (icedCoutdown) {
			stateCountDown -= 1;
		}
		else {
			stateCountDown -= 2;
		}
		if (stateCountDown <= 0) {
			state = STATE_WALKING;
		}
	}
	else if (state == STATE_HAMMERING) {
		// 巨人的敲击过程实际由动画进度完成，但我并不清楚细节
		// 暂时仅用减速416cs，原速208cs代替，可能造成1cs的误差
		if (!freezeCountdown) {
			int lastStateCountDown = stateCountDown;
			if (icedCoutdown) {
				stateCountDown -= 1;
			}
			else {
				stateCountDown -= 2;
			}

			// 巨人敲击在减速第268帧，原速第134帧结算
			if (lastStateCountDown > 148 && stateCountDown <= 148) {
				for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
					if (judgeHitPlant(*it)) {
						(*it)->hammerCrush();
						break;
					}
				}
			}

			if (stateCountDown <= 0) {
				state = STATE_WALKING;
				if (icedCoutdown) {
					anim_progress = 0.5 * progress_delta;
				}
				else {
					anim_progress = progress_delta;
				}
			}
		}
	}
	else if (state == STATE_THROWING) {
		// 巨人扔小鬼的过程实际由动画进度完成，但我并不清楚细节
		// 暂时仅用减速282cs，原速142cs代替，可能造成1cs的误差
		// 注意，此处代码仅模拟巨人的行动因扔小鬼而发生打断造成的结果，
		// 并不真正地生成小鬼，想要知道小鬼需要使用其他函数
		if (!freezeCountdown) {
			int lastStateCountDown = stateCountDown;
			if (icedCoutdown) {
				stateCountDown -= 1;
			}
			else {
				stateCountDown -= 2;
			}

			if (stateCountDown <= 0) {
				state = STATE_WALKING;
				if (icedCoutdown) {
					anim_progress = 0.5 * progress_delta;
				}
				else {
					anim_progress = progress_delta;
				}
			}
		}
	}


	// 其次更新啃食，碾压，敲击
	if (state == STATE_WALKING) {
		bool oldIsEating = isEating, newIsEating = false;
		switch (attackType())
		{
		case ATK_EAT:
			if (!freezeCountdown && ((icedCoutdown && existTime % 8 == 0) || (!icedCoutdown && existTime % 4 == 0))) {
				for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
					if (judgeHitPlant(*it) && ((*it)->getState()==PLANT_STATE_NORMAL || (*it)->getState()==PLANT_STATE_INVINCIBLE)) {
						if (!freezeCountdown) {
							(*it)->hit();
						}
						newIsEating = true;
						isEating = true;
					}
					break;
				}
			}
			if (oldIsEating && !newIsEating) {
				// 啃食结束
				isEating = false;
				if (icedCoutdown) {
					anim_progress = 0.5 * progress_delta;
				}
				else {
					anim_progress = progress_delta;
				}
			}
			break;
		case ATK_CRUSH:
			for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
				if (judgeHitPlant(*it)) {
					(*it)->crush();
				}
			}
			break;
		case ATK_HAMMER:
			for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
				if (judgeHitPlant(*it) && ((*it)->getState()==PLANT_STATE_NORMAL || (*it)->getState()==PLANT_STATE_INVINCIBLE)) {
					state = STATE_HAMMERING;
					stateCountDown = 416;
					break;
				}
			}
			break;
		default:
			// 暂不支持撑杆跳跃和舞王召唤
			break;
		}
	}

	// 其次更新状态倒计时
	existTime++;
	if (freezeCountdown) { freezeCountdown--; }
	if (icedCoutdown) { icedCoutdown--; }
	if (stateCountDown) { stateCountDown--; }
}

void Zombie::update(int tick) {
	for (int i = 0; i < tick; i++)
		update();
}

void Zombie::kill() {
	state = STATE_DEAD;
}

void ReadZombieProperty() {
	std::ifstream infile;
	infile.open(DATA_PATH);
	std::string line;
	std::getline(infile, line);	// 扔掉第一行

	for (int type = 0; type < ZOMBIE_TYPE_NUM; type++) {
		std::getline(infile, line);
		std::istringstream f(line);
		std::string data = "";
		std::getline(f, data, ',');
		ZombieProperties[type].name = data;

		std::getline(f, data, ',');
		ZombieProperties[type].defXShift = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].defYShift = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].defWidth = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].defHeight = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].atkType = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].atkXShift = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].atkWidth = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].genType = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].spawnType = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].spawnAbscissaLower = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].spawnAbscissaHigher = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].movementType = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].relativeSpeedLower = std::stof(data);

		std::getline(f, data, ',');
		ZombieProperties[type].relativeSpeedHigher = std::stof(data);

		std::getline(f, data, ',');
		ZombieProperties[type].iceType = std::stoi(data);

		std::getline(f, data, ',');
		ZombieProperties[type].groundSize = std::stoi(data);
		int _groundSize = ZombieProperties[type].groundSize;

		for (int i = 0; i < _groundSize; i++) {
			std::getline(f, data, ',');
			ZombieProperties[type].ground[i] = std::stof(data);
		}

		ZombieProperties[type].shiftSize = _groundSize - 1;
		for (int i = 0; i < _groundSize - 1; i++) {
			ZombieProperties[type].shift[i] = ZombieProperties[type].ground[i + 1] - ZombieProperties[type].ground[i];
		}

		ZombieProperties[type].totalShift = ZombieProperties[type].ground[_groundSize - 1] - ZombieProperties[type].ground[0];
	}
}

Zombie::Zombie(ZombieType _type, int _row, float _abscissa, float _relativeSpeed) {
	prop = ZombieProperties + (int)_type;
	type = _type;
	state = STATE_WALKING;
	
	row = _row;

	if (_abscissa < ABSC_SPAWN_RANDOM + 1) {
		if (spawnType()) {
			abscissa = (float)game.getRandomInt(prop->spawnAbscissaLower, prop->spawnAbscissaHigher);
		}
		else {
			abscissa = 40;
		}
	}
	else {
		abscissa = _abscissa;
	}

	switch (game.getScene())
	{
	case SceneType::DE:
	case SceneType::NE:
		ordinate = 50 + row * 100;
		roofHeight = 0;
		break;
	case SceneType::PE:
	case SceneType::FE:
		ordinate = 50 + row * 85;
		roofHeight = 0;
		break;
	case SceneType::RE:
	case SceneType::ME:
		ordinate = 40 + row * 85;
		if (int(abscissa) <= 400) {
			roofHeight = (400 - int(abscissa)) / 4.0;
		}
		break;
	default:
		break;
	}

	if (type == ZombieType::BALLON) {
		ordinate -= 30;
		height = 25;
	}
	else {
		height = 0;
	}

	xspeed = 0;
	hspeed = 0;
	haccelaration = 0;

	existTime = 0;
	freezeCountdown = 0;
	icedCoutdown = 0;
	stateCountDown = 0;
	if (type == ZombieType::JACK_IN_THE_BOX) {
		int earlyExplode = game.getRandomInt(0, 20);
		//取0早爆，否则晚爆
		if (earlyExplode == 0) {
			stateCountDown = int(int((game.getRandomInt(0, 300) + 450) / 3) / relativeSpeed) * 2;
		}
		else {
			stateCountDown = int((game.getRandomInt(0, 300) + 450) / relativeSpeed) * 2;
		}
	}

	isEating = false;
	damage = 0;

	switch (movementType())
	{
	case MOVE_UNKNOWN:
	case MOVE_EVEN_RELA_SPEED_FIXED:
	case MOVE_UNEVEN_RELA_SPEED_FIXED:
		relativeSpeed = 0;
		// 目前不支持
		break;
	case MOVE_EVEN_RELA_SPEED_RANDOM:
		// 气球 矿工 篮球
		if (_relativeSpeed < RELATIVE_SPEED_RANDOM + 1) {
			relativeSpeed = game.getRandomFloat(prop->relativeSpeedLower, prop->relativeSpeedHigher);
		}
		else {
			relativeSpeed = _relativeSpeed;
		}
		xspeed = - relativeSpeed;
		break;
	case MOVE_UNEVEN_RELA_SPEED_RANDOM:
		// 撑杆 橄榄 小丑 梯子 巨人
		if (_relativeSpeed < RELATIVE_SPEED_RANDOM + 1) {
			relativeSpeed = game.getRandomFloat(prop->relativeSpeedLower, prop->relativeSpeedHigher);
		}
		else {
			relativeSpeed = _relativeSpeed;
		}
		progress_delta = relativeSpeed * float(47) * float(0.01) / prop->totalShift;
		anim_progress = progress_delta;
		replacement = progress_delta * prop->groundSize;
		xspeed = - prop->shift[int(anim_progress * prop->shiftSize)] * replacement;
		break;
	case MOVE_IMP:
		// 小鬼
		break;
	case MOVE_ZOMBONI:
		// 冰车
		break;
	default:
		break;
	}
}

bool Zombie::judgeHitPlant(Plant* plant) {
	return Judgement::IntervalIntersectInterval(atkAbscissa(), atkWidth(), plant->defAbscissa(), plant->defWidth());
}

bool Zombie::judgeClownExplodePlant(Plant* plant) {
	return Judgement::RectangleIntersectCircle(plant->clownDefAbscissa(),
		plant->defOrdinate(),
		plant->clownDefWidth(),
		plant->defHeight(),
		int(abscissa) + 60,
		int(ordinate+roofHeight) + 60,
		90
	);
}

Zombie* Zombie::throwImp(int _impRnd, bool stackHigher) {
	int impRnd;
	if (int(abscissa) <= 400 || !isDamagable()) {
		return nullptr;
	}

	if (int(abscissa) <= 500) {
		impRnd = 0;
	}
	else {
		if (_impRnd == -1) {
			impRnd = game.getRandomInt(0, 101);
		}
		else {
			impRnd = _impRnd;
		}
	}

	Zombie* imp = new Zombie(ZombieType::IMP, row);
	imp->state = STATE_IMP_FLYING;
	imp->xspeed = -3;
	imp->haccelaration = -0.05;
	imp->freezeCountdown = freezeCountdown;
	imp->abscissa = int(abscissa) - 133;
	imp->ordinate = ordinate;	// 能扔出小鬼的巨人不会有天台y偏移
	imp->height = 88;
	switch (game.getScene())
	{
	case SceneType::DE:
	case SceneType::NE:
	case SceneType::PE:
	case SceneType::FE:
		imp->hspeed = (int(abscissa) - 360 - impRnd) / 120.0;
		break;
	case SceneType::RE:
	case SceneType::ME:
		imp->hspeed = (int(abscissa) - 360 - 180 - impRnd) / 120.0;
	default:
		break;
	}
	if (stackHigher) {
		imp->update();
	}

	return imp;
}

std::vector<Zombie*> Zombie::throwAllImps() {
	if (int(abscissa) <= 400) {
		return std::vector<Zombie*>(0);
	}
	else if (int(abscissa) <= 500) {
		std::vector<Zombie*> imps;
		imps.push_back(throwImp(0, true));
		imps.push_back(throwImp(0, false));
		return imps;
	}
	else {
		std::vector<Zombie*> imps;
		for (int rnd = 0; rnd <= 100; rnd++) {
			imps.push_back(throwImp(rnd, true));
			imps.push_back(throwImp(rnd, false));
		}
		return imps;
	}
}

bool Zombie::isDamagable() {
	return Judgement::IntervalIntersectInterval(defAbscissa(), defWidth(), -21, 821);
}