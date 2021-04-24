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

	// ���ȸ����˶�
	if (state == STATE_WALKING) {
		if (movementType() == MOVE_EVEN_RELA_SPEED_RANDOM) {
			switch (iceType())
			{
			case ICE_NORMAL:
				// ����
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
				// ����
				if (icedCoutdown) {
					xspeed = -0.4 * relativeSpeed;
				}
				else {
					xspeed = -relativeSpeed;
				}
				abscissa += xspeed;
				break;
			case ICE_NONE:
				// ��
				xspeed = -relativeSpeed;
				abscissa += xspeed;
				break;
			default:
				break;
			}
		}
		else if (movementType() == MOVE_UNEVEN_RELA_SPEED_RANDOM) {
			// �Ÿ����С�����Ӿ���
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
		// ���������ʱ���ڿ��Ƿ�Χ��
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
		// С�����ع���ʵ���ɶ���������ɣ����Ҳ������ϸ��
		// ��ʱ���ü���50cs��ԭ��25cs���棬�������1cs�����
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
		// ���˵��û�����ʵ���ɶ���������ɣ����Ҳ������ϸ��
		// ��ʱ���ü���416cs��ԭ��208cs���棬�������1cs�����
		if (!freezeCountdown) {
			int lastStateCountDown = stateCountDown;
			if (icedCoutdown) {
				stateCountDown -= 1;
			}
			else {
				stateCountDown -= 2;
			}

			// �����û��ڼ��ٵ�268֡��ԭ�ٵ�134֡����
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
		// ������С��Ĺ���ʵ���ɶ���������ɣ����Ҳ������ϸ��
		// ��ʱ���ü���282cs��ԭ��142cs���棬�������1cs�����
		// ע�⣬�˴������ģ����˵��ж�����С������������ɵĽ����
		// ��������������С����Ҫ֪��С����Ҫʹ����������
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


	// ��θ��¿�ʳ����ѹ���û�
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
				// ��ʳ����
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
			// �ݲ�֧�ֳŸ���Ծ�������ٻ�
			break;
		}
	}

	// ��θ���״̬����ʱ
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
	std::getline(infile, line);	// �ӵ���һ��

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
		//ȡ0�籬��������
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
		// Ŀǰ��֧��
		break;
	case MOVE_EVEN_RELA_SPEED_RANDOM:
		// ���� �� ����
		if (_relativeSpeed < RELATIVE_SPEED_RANDOM + 1) {
			relativeSpeed = game.getRandomFloat(prop->relativeSpeedLower, prop->relativeSpeedHigher);
		}
		else {
			relativeSpeed = _relativeSpeed;
		}
		xspeed = - relativeSpeed;
		break;
	case MOVE_UNEVEN_RELA_SPEED_RANDOM:
		// �Ÿ� ��� С�� ���� ����
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
		// С��
		break;
	case MOVE_ZOMBONI:
		// ����
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
	imp->ordinate = ordinate;	// ���ӳ�С��ľ��˲�������̨yƫ��
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