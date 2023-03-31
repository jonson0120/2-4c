#pragma once
#include "Player.h"
#include "common.h"

class Player;

class Enemy
{
public:
	Enemy() {};

	virtual void Update(Player* player) = 0;
	virtual void Draw(int x, int y)const = 0;

	virtual void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]) = 0;
	virtual void makeEnemy() = 0;

	virtual int E_GetX()const = 0;
	virtual int E_GetY()const = 0;

	virtual int GetWidth()const = 0;
	virtual int GetHeight()const = 0;

	virtual bool CheckHp() = 0;

	virtual void HitPlayer(float damage) = 0;
};

