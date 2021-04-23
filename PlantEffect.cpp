#include "PlantEffect.h"
#include "Game.h"
#include "judgement.h"

extern Game game;

bool PlantEffect::operator<(const PlantEffect& other) {
	return (time < other.time) || (time == other.time && effectType < other.effectType);
}

Explosion::Explosion(int _time, ExplosionType _type, int row, float col, int roofPaoCol, int roofPaoRow) {
	time = _time;
	explosionType = _type;
	if (_type == ExplosionType::COB) {
		effectType = EffectType::BULLET_EFFECT;
		radius = 115;
		int a = int(col * 80);
		if (a >= 7) {
			abscissa = a - 7;
		}
		else {
			abscissa = a - 6;
		}
		if (game.getScene() == SceneType::RE || game.getScene() == SceneType::ME) {
			ordinate = 209 + row * 85;
			int g, h, k, L, R, S;
			if (a <= 206) {
				g = 0;
			}
			else if (a >= 527) {
				g = -100;
			}
			else {
				g = -20 * int((a - 127) / 80);
			}

			if (roofPaoCol == -1) {
				game.reportError("屋顶场地需指定炮尾所在列。");
				return;
			}

			if (roofPaoCol == 0) {
				L = 87;
				R = 524;
				S = 0;
			}
			else if (1 <= roofPaoCol && roofPaoCol <= 5) {
				L = 80 * roofPaoCol + 67;
				R = 524;
				S = 5;
			}
			else if (6 <= roofPaoCol && roofPaoCol <= 7) {
				L = 510;
				R = 523;
				S = 5;
			}
			else {
				game.reportError("不存在九列炮或其他列炮。注意行数是0~8而不是1~9");
				return;
			}

			if (0 <= a && a <= L) {
				h = 0;
			}
			else if (L + 1 <= a && a <= R - 1) {
				h = -int((a - L + 3) / 4) + S;
			}
			else if (a >= R) {
				h = -int((R - L + 3) / 4) + S;
			}

			k = 0;

			if (a == L) {
				if (roofPaoRow == -1) {
					game.reportError("屋顶场地特殊落点，需指定炮所在行。");
					return;
				}

				if (1 <= roofPaoCol && roofPaoCol <= 6 && 2 <= roofPaoRow && roofPaoRow <= 4) {
					k = 5;
				}

				if (roofPaoCol == 5 && roofPaoRow == 2) {
					k = 0;
				}
			}

			ordinate += g + h + k;
			if (ordinate < 0) { ordinate = 0; }
		}
		else if(game.getScene() == SceneType::PE || game.getScene()==SceneType::FE) {
			ordinate = 120 + row * 85;
		}
		else {
			ordinate = 120 + row * 100;
		}
	}
	else {
		effectType = EffectType::PLANT_EFFECT;
		Plant ash(PlantType::NORMAL, row, col);
		abscissa = ash.abscissa();
		ordinate = ash.ordinate();
		if (_type == ExplosionType::DOOM) {
			radius = 250;
		}
		else {
			radius = 115;
		}
	}
}

Zombie* Explosion::TestIfAllHit(std::vector<Zombie*> zombies) {
	for (std::vector<Zombie*>::iterator it = zombies.begin(); it != zombies.end(); it++) {
		if (!RectangleIntersectCircle(
			(*it)->defAbscissa(),
			(*it)->defOrdinate(),
			(*it)->defWidth(),
			(*it)->defHeight(),
			abscissa,
			ordinate,
			radius
		)) {
			return *it;
		}
	}
	return nullptr;
}

Zombie* Explosion::TestIfHit(std::vector<Zombie*> zombies) {
	for (std::vector<Zombie*>::iterator it = zombies.begin(); it != zombies.end(); it++) {
		if (RectangleIntersectCircle(
			(*it)->defAbscissa(),
			(*it)->defOrdinate(),
			(*it)->defWidth(),
			(*it)->defHeight(),
			abscissa,
			ordinate,
			radius
		)) {
			return *it;
		}
	}
	return nullptr;

}

void Explosion::effect() {
	for (std::vector<Zombie*>::iterator it = game.zombieList.begin(); it != game.zombieList.end(); it++) {
		if ((*it)->isDamagable() && RectangleIntersectCircle(
			(*it)->defAbscissa(),
			(*it)->defOrdinate(),
			(*it)->defWidth(),
			(*it)->defHeight(),
			abscissa,
			ordinate,
			radius)
		) {
			if ((*it)->type != ZombieType::GARGANTUAR && (*it)->type != ZombieType::GIGA) {
				(*it)->kill();
			}
			else {
				(*it)->damage += 1800;
			}
		}
	}
}

Ice::Ice(int _time, IceEffectType _type) {
	iceEffectType = _type;
	time = _time;
	effectType = EffectType::PLANT_EFFECT;
}

void Ice::effect() {
	for (std::vector<Zombie*>::iterator it = game.zombieList.begin(); it != game.zombieList.end(); it++) {
		if ((*it)->iceType() == ICE_UNKNOWN || (*it)->iceType() == ICE_NONE) {
			continue;
		}

		(*it)->icedCoutdown = 2000;

		if ((*it)->iceType() == ICE_NOT_FREEZE) {
			continue;
		}

		switch (iceEffectType)
		{
		case IceEffectType::FAST:
			(*it)->freezeCountdown = 400;
		case IceEffectType::SLOW:
			(*it)->freezeCountdown = 600;
		case IceEffectType::RANDOM:
			(*it)->freezeCountdown = game.getRandomInt(400, 601);
		default:
			break;
		}
	}
}

AddBlocker::AddBlocker(int _time, int _row, int _col, bool _setInvincible, PlantType _plantType) {
	time = _time;
	effectType = EffectType::PLANT_EFFECT;
	row = _row;
	col = _col;
	setInvincible = _setInvincible;
	plantType = _plantType;
}

void AddBlocker::effect() {
	int plantState = setInvincible ? PLANT_STATE_INVINCIBLE : PLANT_STATE_NORMAL;
	Plant* blocker = new Plant(plantType, row, col, plantState);
	game.plantList.push_back(blocker);
}

ShovelBlocker::ShovelBlocker(int _time, int _row, int _col) {
	time = _time;
	effectType = EffectType::PLANT_EFFECT;
	row = _row;
	col = _col;
}

void ShovelBlocker::effect() {
	for (std::vector<Plant*>::iterator it = game.plantList.begin(); it != game.plantList.end(); it++) {
		if ((*it)->row == row && (*it)->col == col) {
			(*it)->shovel();
		}
	}
}