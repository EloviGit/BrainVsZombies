#include "Plant.h"
#include "Game.h"

extern Game game;

Plant::Plant(PlantType _type, int _row, int _col, int _state) {
	type = _type;
	row = _row;
	col = _col;
	switch (type)
	{
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
	switch (game.getScene())
	{
	case SceneType::DE:
	case SceneType::NE:
		_ordinate = 80 + row * 100;
	case SceneType::PE:
	case SceneType::FE:
		_ordinate = 80 + row * 85;
	case SceneType::RE:
	case SceneType::ME:
		_ordinate = 70 + row * 85;
		if (col <= 4) {
			_ordinate += 20 * col;
		}
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

void Plant::update() {
	if (damage >= 300 && state == PLANT_STATE_NORMAL) {
		state = PLANT_STATE_DEAD;
	}
}

void Plant::update(int tick) {
	for (int i = 0; i < tick; i++)
		update();
}
