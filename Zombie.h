#pragma once
#include <string>
#include "Plant.h"

#define DATA_PATH		"data.csv"			// 数据文件路径

// 运动
#define MOVE_UNKNOWN						0	// 运动模式我也不知道
#define MOVE_EVEN_RELA_SPEED_FIXED			1	// 匀速运动，相对速度确定
#define MOVE_EVEN_RELA_SPEED_RANDOM			2	// 匀速运动，相对速度随机，0.23~0.37, 0.66~0.68, 0.79~0.81, 或 0.89~0.91
#define MOVE_UNEVEN_RELA_SPEED_FIXED		3	// 非匀速运动，相对速度确定
#define MOVE_UNEVEN_RELA_SPEED_RANDOM		4	// 非匀速运动，相对速度随机，0.23~0.37, 0.66~0.68, 0.79~0.81, 或 0.89~0.91
#define MOVE_ZOMBONI						5	// 冰车运动模式，随位置衰减
#define MOVE_IMP							6	// 小鬼运动模式，空中飞行

// 出怪
#define	GEN_NOT_SELECT						0	// 不会被随机到
#define GEN_NOTAPPEAR						1	// 可以被随机到，但不会出现（旗帜，僵王）
#define GEN_NORMAL							2	// 可以被随机到，然后就出现
#define GEN_ZOMBONI							3   // 冰车，NE不会出现
#define GEN_PLPH_SNRK						4   // 海豚潜水，泳池才会出现
#define GEN_DANC_DIGG						5	// 舞王矿工，天台才会出现
#define GEN_GIGA							6	// 红眼，非大波至多50只

// 出生
#define SPAWN_NOT							0	// 不会出生
#define	SPAWN_NORMAL						1	// 普通出生
#define	SPAWN_ZOMBONI						2	// 冰车出生
#define	SPAWN_CATAPULT						3	// 篮球出生
#define SPAWN_POLE							4	// 撑杆出生
#define	SPAWN_GARGANTUAR					5	// 巨人出生
#define SPAWN_FLAG							6	// 旗杆出生

// 攻击方式
#define	ATK_NONE							0	// 蹦极/僵王/雪橇车，不攻击
#define ATK_EAT								1	// 啃食
#define ATK_CRUSH							2	// 碾压
#define ATK_HAMMER							3	// 敲击
#define ATK_OTHER							4	// 跳跃，召唤

// 状态
#define STATE_WALKING						0	// 普通行走状态，可以啃食
#define STATE_DEAD							1	// 死亡，1帧后回收
#define STATE_THROWING						69	// 巨人投掷
#define STATE_HAMMERING						70	// 巨人敲击
#define STATE_IMP_FLYING					71	// 小鬼飞行
#define STATE_IMP_LANDING					72	// 小鬼落地
#define STATE_CLOWN_EXPLODE					16	// 小丑开盒

// 受冰情况
#define ICE_UNKNOWN							0	// 未知
#define	ICE_NORMAL							1	// 受冰冻，且会减速
#define ICE_NOT_FREEZE						2	// 不受冰冻，但会减速
#define ICE_NONE							3	// 无视冰菇

// 僵尸种类数
#define ZOMBIE_TYPE_NUM						27	// 共有27种僵尸

#define COL_SPAWN_RANDOM					-1.0f	// 随机生成出生点
#define RELATIVE_SPEED_RANDOM				-1.0f	// 随机生成相对速度

enum class ZombieType {
	ZOMBIE=0,		// 普僵
	FLAG,			// 旗帜
	CONEHEAD,		// 路障
	POLE_VAULTING,	// 撑杆
	BUCKETHEAD,		// 铁桶
	NEWSPAPER,		// 报纸
	SCREEN_DOOR,	// 铁门
	FOOTBALL,		// 橄榄
	DANCING,		// 舞王
	BACKUP_DANCER,	// 伴舞
	DUCKY_TUBE,		// 鸭子
	SNORKEL,		// 潜水
	ZOMBONI,		// 冰车
	BOBSLED_TEAM,	// 雪橇
	DOLPHIN_RIDER,	// 海豚
	JACK_IN_THE_BOX,// 小丑 
	BALLON,			// 气球
	DIGGER,			// 矿工
	POGO,			// 跳跳
	YETI,			// 雪人
	BUNGEE,			// 蹦极
	LADDER,			// 梯子
	CATAPULT,		// 篮球
	GARGANTUAR,		// 巨人
	IMP,			// 小鬼
	DR_ZOMBOSS,		// 僵王
	GIGA=26,		// 红眼
};

typedef struct zombie_property
{
public:
	std::string name;
	int defXShift;
	int defYShift;
	int defWidth;
	int defHeight;
	int atkType;
	int atkXShift;
	int atkWidth;
	int genType;
	int spawnType;
	int spawnAbscissaLower;
	int spawnAbscissaHigher;
	int movementType;
	float relativeSpeedLower;
	float relativeSpeedHigher;
	int iceType;
	int groundSize;
	float ground[50];
	int shiftSize;
	float shift[50];
	float totalShift;
}ZombieProperty;

void ReadZombieProperty();

class Zombie
{
public:
	ZombieProperty* prop;
	ZombieType type;
	int state;

	int row;
	float abscissa;
	float ordinate;
	float height;
	float roofHeight;

	float xspeed;
	float hspeed;
	float haccelaration;

	int existTime;

	int freezeCountdown;	// 冰冻倒计时
	int icedCoutdown;		// 减速倒计时
	int stateCountDown;

	bool isEating;
	int damage;

	float relativeSpeed;
	float anim_progress;
	float progress_delta;
	float replacement;

	int defAbscissa() { return (int)abscissa + prop->defXShift; }
	int defWidth() { return prop->defWidth; }
	int defOrdinate() {
		if (type == ZombieType::IMP && state == STATE_IMP_FLYING) {
			// 注：当场景非天台时两种情况相同。当场景为天台时，
			// 小鬼实际y为 ordinate+roofHeight，而实际高度为 height+roofHeight，两者相减抵消roofHeight
			return (int)(ordinate - height) + prop->defYShift;
		}
		else {
			return (int)(ordinate + roofHeight - height) + prop->defYShift;
		}
	}
	int defHeight() { return prop->defHeight; }
	int atkAbscissa() { return (int)(abscissa) + prop->atkXShift; }
	int atkWidth() { return prop->atkWidth; }
	
	int movementType() { return prop->movementType; }
	int generationType() { return prop->genType; }
	int spawnType() { return prop->spawnType; }
	int attackType() { return prop->atkType; }
	int iceType() { return prop->iceType; }

	Zombie(ZombieType _type, int _row=0, float _col=COL_SPAWN_RANDOM, float _relativeSpeed=RELATIVE_SPEED_RANDOM);
	~Zombie(){}

	void update();
	void update(int tick);

	// 范围： 0~100，可以取到0和100，但401~500只能为0
	Zombie* throwImp(int _impRnd=-1, bool stackHigher=true);

	std::vector<Zombie*> throwAllImps();

	bool judgeHitPlant(Plant* plant);

	bool judgeClownExplodePlant(Plant* plant);

	bool isDamagable();

	bool isDamaged() { return damage; }

	void kill();
};
