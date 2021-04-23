#pragma once

#define PLANT_STATE_NORMAL		0	// ����
#define PLANT_STATE_DEAD		1	// ����ʳ��������
#define PLANT_STATE_CRUSHED		2	// ��ϵ��ѹ
#define PLANT_STATE_HAMMERED	3	// �����û�
#define PLANT_STATE_SHOVELED	4	// ����
#define PLANT_STATE_INVINCIBLE	5	// �����޵�״̬�����ӽ�ʬ�����ܱ�����

enum class PlantType {
	NORMAL = 0,			// ��ֲͨ��
	COB_CANNON,			// ������
	TALL_NUT,			// �߼��
	PUMPKIN=3,			// �Ϲ�
//	PUFF_SHROOM = 4,	// С�繽 С������е��鷳����ʱ������
};

class Plant
{
	PlantType type;
	int state;
	int defXShift;
	int defXWidth;
	int defYShift = 0;
	int defYWidth = 80;
	int clownDefXShift;
	int clownDefXWidth;
	int damage = 0;

public:
	int row;
	int col;


	Plant(PlantType _type, int _row, int _col, int _state=PLANT_STATE_INVINCIBLE);
	
	int abscissa() { return 40 + row * 80; }
	int ordinate();
	int defAbscissa() { return abscissa() + defXShift; }
	int defWidth() { return defXWidth; }
	int defOrdinate() { return ordinate() + defYShift; }
	int defHeight() { return defYWidth; }
	int clownDefAbscissa() { return abscissa() + clownDefXShift; }
	int clownDefWidth() { return clownDefXWidth; }

	int getState() { return state; }

	void hit(); 
	int getDamage() { return damage; }

	void crush();
	void hammerCrush();
	void shovel() { state = PLANT_STATE_SHOVELED; }

	void update();
};

