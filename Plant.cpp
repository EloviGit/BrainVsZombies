#include "Plant.h"
#include "Game.h"
#include <sstream>

extern Game game;

Plant::Plant(PlantType _type, int _row, int _col, int _state) {
	type = _type;
	row = _row;
	col = _col;
	id = game.getPlantID();
	switch (type) {
		case PlantType::NORMAL:
			defXShift = 30;
			defXWidth = 20;
			break;
		case PlantType::COB_CANNON:
			defXShift = 20;
			defXWidth = 100;
			break;
		case PlantType::TALL_NUT:
			defXShift = 30;
			defXWidth = 40;
			break;
		case PlantType::PUMPKIN:
			defXShift = 20;
			defXWidth = 60;
			break;
		default:
			break;
	}
	clownDefXShift = defXShift - 20;
	clownDefXWidth = defXWidth + 40;
	state = _state;
}

int Plant::ordinate() {
	int _ordinate = -1;
	switch (game.getScene()) {
		case SceneType::DE:
		case SceneType::NE:
			_ordinate = 80 + row * 100;
			break;
		case SceneType::PE:
		case SceneType::FE:
			_ordinate = 80 + row * 85;
			break;
		case SceneType::RE:
		case SceneType::ME:
			_ordinate = 70 + row * 85;
			if (col <= 4) {
				_ordinate += 20 * col;
			}
			break;
		default:
			break;
	}
	return _ordinate;
}

void Plant::hit() {
	if (state == PLANT_STATE_NORMAL || state == PLANT_STATE_INVINCIBLE) {
		damage += 4;
	}
}

void Plant::crush() {
	if (state == PLANT_STATE_NORMAL) {
		state = PLANT_STATE_CRUSHED;
	}
}

void Plant::hammerCrush() {
	if (state == PLANT_STATE_NORMAL) {
		state = PLANT_STATE_HAMMERED;
	}
}

// 此处有一小bug，起跳的窝瓜仍能被小丑炸到，待细化
void Plant::explode(int zombie_id) {
	if (state == PLANT_STATE_NORMAL || state == PLANT_STATE_INVINCIBLE) {
		state = PLANT_STATE_EXPLODED;
		explodeID = zombie_id;
	}
}

void Plant::update() {
	if (damage >= 300 && state == PLANT_STATE_NORMAL) {
		state = PLANT_STATE_DEAD;
	}

	// 输出
	if (game.debug()) {
		std::stringstream ss;
		ss << game.getGameClock() << ",Plant," << id << "," << getTypeStr() << "," << row <<
			"," << col << "," << abscissa() << "," << ordinate() << "," << state << "," << damage << ",\n";
		game.log(ss.str());
	}
}

void Plant::update(int tick) {
	for (int i = 0; i < tick; i++)
		update();
}

bool Plant::isDisappeared() {
	return state == PLANT_STATE_DEAD || state == PLANT_STATE_CRUSHED || state == PLANT_STATE_HAMMERED
		|| state == PLANT_STATE_EXPLODED || state == PLANT_STATE_SHOVELED;
}

std::string Plant::getTypeStr() {
	std::string names[] = { "NORMAL", "COB","TALL_NUT", "PUMPKIN" };
	return names[int(type)];
}