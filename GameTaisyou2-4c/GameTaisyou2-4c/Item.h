#pragma once
#include"DxLib.h"
#include"Player.h"
#include"common.h"

class Item
{
private:
	int Type;		//�A�C�e����� 1:����
	weapons Weapon;	//������	
	bool CanGet;		//�E����͈͂ɂ��邩

	int image;		//�A�C�e���摜
	int icon;		//�A�C�R���摜
	float size;		//�摜�T�C�Y��

	float fall;		//�������x

	Range pos;		//�A�C�e�����W
	int Height, Width;	//�A�C�e���T�C�Y
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

public :
	Item(int Type, weapons Weapon, Range position);

	void Update(Player* player);
	void Draw(Range pos)const;

	void SetItem();

	int GetType() { return Type; }
	weapons GetweaponType() { return Weapon; }
	int GetDis(Range Player);

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
};

