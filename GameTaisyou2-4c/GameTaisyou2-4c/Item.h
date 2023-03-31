#pragma once
#include"DxLib.h"
#include"Player.h"
#include"common.h"

class Item
{
private:
	int Type;		//ƒAƒCƒeƒ€í—Ş 1:•Ší
	weapons Weapon;	//•Šíí—Ş	

private:
	Item(int Type, weapons Weapon);

	void Update();
	void Draw()const;
};

