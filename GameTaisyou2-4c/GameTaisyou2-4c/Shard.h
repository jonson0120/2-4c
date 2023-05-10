#pragma once
#include "Item.h"
class Shard :
    public Item
{
private:
	bool CanGet;		//�E����͈͂ɂ��邩
	bool Getted;		//�E��ꂽ��

	int image;		//�A�C�e���摜
	float size;		//�摜�T�C�Y��

	float fall;		//�������x
	float Slide;	//���ړ����x

	int ShardSE;

	Range pos;		//�A�C�e�����W
	int Height, Width;	//�A�C�e���T�C�Y
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

public:
	Shard(Range position);

	void Update(Player* player);
	void Draw(Range pos)const;

	void SetItem();

	ItemType GetType() { return Sh; }
	bool GetGet() { return Getted; }

	int GetDis(Range Player);

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
};

