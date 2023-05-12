#pragma once
#include "Player.h"
#include "common.h"

enum ItemType
{
	Sh,		//ƒVƒƒ[ƒh
	Equip	//‘•”õ
};

class Item
{

public:
	Item() {};

	virtual void Update(Player* player) = 0;
	virtual void Draw(Range pos)const = 0;

	virtual	void SetItem() = 0;

	virtual bool GetGet() = 0;
	virtual ItemType GetType() = 0;

	virtual int GetDis(Range Player) = 0;

	virtual	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]) = 0;
};