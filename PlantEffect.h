#pragma once
#include <vector>
#include "Zombie.h"

enum class EffectType {
	PLANT_EFFECT=0,
	BULLET_EFFECT=1,
};

class PlantEffect
{
public:
	int time;
	EffectType effectType;
	virtual void effect()=0;
	bool operator<(const PlantEffect& other);
};

enum class ExplosionType {
	COB=0,
	DOOM,
	CHERRY=2,
};

class Explosion : public PlantEffect
{
public:
	ExplosionType explosionType;
	int abscissa;
	int ordinate;
	int radius;
	Explosion(int _time, ExplosionType _type, int row, float col, int roofPaoCol=-1, int roofPaoRow=-1);
	void effect();
	Zombie* TestIfAllHit(std::vector<Zombie*> zombies);
	Zombie* TestIfHit(std::vector<Zombie*> zombies);
};

enum class IceEffectType {
	RANDOM = 0,
	FAST,
	SLOW = 2,
};

class Ice : public PlantEffect
{
public:
	IceEffectType iceEffectType;
	Ice(int _time, IceEffectType _type = IceEffectType::RANDOM);
	void effect();
};

class AddBlocker : public PlantEffect
{
public:
	int row;
	int col;
	bool setInvincible;
	PlantType plantType;
	AddBlocker(int _time, int _row, int _col, bool _setInvincible = true, PlantType _plantType = PlantType::NORMAL);
	void effect();
};

class ShovelBlocker : public PlantEffect
{
public:
	int row;
	int col;
	ShovelBlocker(int _time, int _row, int _col);
	void effect();
};
