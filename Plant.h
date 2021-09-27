#pragma once
#include <string>

#define PLANT_STATE_NORMAL		0	// 正常
#define PLANT_STATE_DEAD		1	// 被啃食导致死亡
#define PLANT_STATE_CRUSHED		2	// 车系碾压
#define PLANT_STATE_HAMMERED	3	// 巨人敲击
#define PLANT_STATE_EXPLODED	4	// 小丑爆炸
#define PLANT_STATE_SHOVELED	5	// 铲除
#define PLANT_STATE_INVINCIBLE	6	// 开启无敌状态，无视僵尸，仅能被铲除


enum class PlantType {
	NORMAL = 0,			// 普通植物
	COB_CANNON,			// 玉米炮
	TALL_NUT,			// 高坚果
	PUMPKIN = 3,		// 南瓜
//	PUFF_SHROOM = 4,	// 小喷菇 小喷机制有点麻烦，暂时不考虑
};

class Plant {
	PlantType type;
	int state;
	int defXShift;
	int defXWidth;
	int defYShift = 0;
	int defYWidth = 80;
	int clownDefXShift;
	int clownDefXWidth;
	int damage = 0;
	int id = -1;
	int explodeID = -1;		// 炸掉植物的小丑id

public:
	int row;
	int col;


	Plant(PlantType _type, int _row, int _col, int _state = PLANT_STATE_INVINCIBLE);

	int abscissa() { return 40 + col * 80; }
	int ordinate();
	int defAbscissa() { return abscissa() + defXShift; }
	int defWidth() { return defXWidth; }
	int defOrdinate() { return ordinate() + defYShift; }
	int defHeight() { return defYWidth; }
	int clownDefAbscissa() { return abscissa() + clownDefXShift; }
	int clownDefWidth() { return clownDefXWidth; }
	bool isDisappeared();

	int getState() { return state; }
	PlantType getType() { return type; }
	std::string getTypeStr();
	int getExplodeID() { return explodeID; }

	void hit();
	int getDamage() { return damage; }

	void crush();
	void hammerCrush();
	void explode(int zombie_id = -1);
	void shovel() { if (!isDisappeared()) state = PLANT_STATE_SHOVELED; }

	void update();
	void update(int tick);
};
