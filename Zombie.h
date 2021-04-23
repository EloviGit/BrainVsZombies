#pragma once
#include <string>
#include "Plant.h"

#define DATA_PATH		"data.csv"			// �����ļ�·��

// �˶�
#define MOVE_UNKNOWN						0	// �˶�ģʽ��Ҳ��֪��
#define MOVE_EVEN_RELA_SPEED_FIXED			1	// �����˶�������ٶ�ȷ��
#define MOVE_EVEN_RELA_SPEED_RANDOM			2	// �����˶�������ٶ������0.23~0.37, 0.66~0.68, 0.79~0.81, �� 0.89~0.91
#define MOVE_UNEVEN_RELA_SPEED_FIXED		3	// �������˶�������ٶ�ȷ��
#define MOVE_UNEVEN_RELA_SPEED_RANDOM		4	// �������˶�������ٶ������0.23~0.37, 0.66~0.68, 0.79~0.81, �� 0.89~0.91
#define MOVE_ZOMBONI						5	// �����˶�ģʽ����λ��˥��
#define MOVE_IMP							6	// С���˶�ģʽ�����з���

// ����
#define	GEN_NOT_SELECT						0	// ���ᱻ�����
#define GEN_NOTAPPEAR						1	// ���Ա����������������֣����ģ�������
#define GEN_NORMAL							2	// ���Ա��������Ȼ��ͳ���
#define GEN_ZOMBONI							3   // ������NE�������
#define GEN_PLPH_SNRK						4   // ����Ǳˮ��Ӿ�زŻ����
#define GEN_DANC_DIGG						5	// �����󹤣���̨�Ż����
#define GEN_GIGA							6	// ���ۣ��Ǵ�����50ֻ

// ����
#define SPAWN_NOT							0	// �������
#define	SPAWN_NORMAL						1	// ��ͨ����
#define	SPAWN_ZOMBONI						2	// ��������
#define	SPAWN_CATAPULT						3	// �������
#define SPAWN_POLE							4	// �Ÿ˳���
#define	SPAWN_GARGANTUAR					5	// ���˳���
#define SPAWN_FLAG							6	// ��˳���

// ������ʽ
#define	ATK_NONE							0	// �ļ�/����/ѩ������������
#define ATK_EAT								1	// ��ʳ
#define ATK_CRUSH							2	// ��ѹ
#define ATK_HAMMER							3	// �û�
#define ATK_OTHER							4	// ��Ծ���ٻ�

// ״̬
#define STATE_WALKING						0	// ��ͨ����״̬�����Կ�ʳ
#define STATE_DEAD							1	// ������1֡�����
#define STATE_THROWING						69	// ����Ͷ��
#define STATE_HAMMERING						70	// �����û�
#define STATE_IMP_FLYING					71	// С�����
#define STATE_IMP_LANDING					72	// С�����
#define STATE_CLOWN_EXPLODE					16	// С�󿪺�

// �ܱ����
#define ICE_UNKNOWN							0	// δ֪
#define	ICE_NORMAL							1	// �ܱ������һ����
#define ICE_NOT_FREEZE						2	// ���ܱ������������
#define ICE_NONE							3	// ���ӱ���

// ��ʬ������
#define ZOMBIE_TYPE_NUM						27	// ����27�ֽ�ʬ

#define COL_SPAWN_RANDOM					-1.0f	// ������ɳ�����
#define RELATIVE_SPEED_RANDOM				-1.0f	// �����������ٶ�

enum class ZombieType {
	ZOMBIE=0,		// �ս�
	FLAG,			// ����
	CONEHEAD,		// ·��
	POLE_VAULTING,	// �Ÿ�
	BUCKETHEAD,		// ��Ͱ
	NEWSPAPER,		// ��ֽ
	SCREEN_DOOR,	// ����
	FOOTBALL,		// ���
	DANCING,		// ����
	BACKUP_DANCER,	// ����
	DUCKY_TUBE,		// Ѽ��
	SNORKEL,		// Ǳˮ
	ZOMBONI,		// ����
	BOBSLED_TEAM,	// ѩ��
	DOLPHIN_RIDER,	// ����
	JACK_IN_THE_BOX,// С�� 
	BALLON,			// ����
	DIGGER,			// ��
	POGO,			// ����
	YETI,			// ѩ��
	BUNGEE,			// �ļ�
	LADDER,			// ����
	CATAPULT,		// ����
	GARGANTUAR,		// ����
	IMP,			// С��
	DR_ZOMBOSS,		// ����
	GIGA=26,		// ����
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

	int freezeCountdown;	// ��������ʱ
	int icedCoutdown;		// ���ٵ���ʱ
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
			// ע������������̨ʱ���������ͬ��������Ϊ��̨ʱ��
			// С��ʵ��yΪ ordinate+roofHeight����ʵ�ʸ߶�Ϊ height+roofHeight�������������roofHeight
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

	// ��Χ�� 0~100������ȡ��0��100����401~500ֻ��Ϊ0
	Zombie* throwImp(int _impRnd=-1, bool stackHigher=true);

	std::vector<Zombie*> throwAllImps();

	bool judgeHitPlant(Plant* plant);

	bool judgeClownExplodePlant(Plant* plant);

	bool isDamagable();

	bool isDamaged() { return damage; }

	void kill();
};
