#pragma once
#include"DxLib.h"
#include"Player.h"
#include"Item.h"
#include"common.h"

class Weapon : public Item
{
private:
	weapons Kind;	//������	
	Passive passive[4];	//�t�^����
	bool CanGet;		//�E����͈͂ɂ��邩
	bool Getted;		//���t���[���ɂ����ďE��ꂽ��

	int image;		//�A�C�e���摜
	int passiveimg[8];	//�p�b�V�u�摜
	int numimg[44];		//�����摜
	int icon;		//�A�C�R���摜
	float size;		//�摜�T�C�Y��

	float fall;		//�������x

	Range pos;		//�A�C�e�����W
	int Height, Width;	//�A�C�e���T�C�Y
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

public :
	Weapon(weapons Weapon, Range position, int Level);
	void SetPassive(int Level);

	void Update(Player* player);
	void Draw(Range pos)const;

	void SetItem();

	bool GetGet() { return Getted; }
	ItemType GetType() { return Equip; }

	weapons GetweaponType() { return Kind; }
	int GetDis(Range Player);

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
};

