#pragma once
#include"DxLib.h"
#include"Player.h"
#include"common.h"

class Item
{
private:
	int Type;		//�A�C�e����� 1:����
	weapons Weapon;	//������	

private:
	Item(int Type, weapons Weapon);

	void Update();
	void Draw()const;
};

