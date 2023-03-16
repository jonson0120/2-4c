#pragma once
#include "Enemy.h"

class Player;

class Enemy2 : public Enemy
{
private:
	int image;			//�摜
	int E2Images[2];		//�摜
	int DropItem_Image;	//�h���b�v�A�C�e��
	int ene2x, ene2y;		//X,Y���W
	int Width, Height;	//�G�̍���
	int direction;		//�G�̌���
	int E2_AttackFlg;
	int Player_Damage;
	int Enemy2_Damage;
	int Enemy2_Hp;
	int Player_Hp;
	bool MakeEnemy2;

	//Player player;

	float speed; //�ړ����x
	float fall;	 //�������x
	int jump;	 //�W�����v��

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

public:
	Enemy2();
	void Update(Player* player);
	void Draw(int x, int y) const;

	int E2_GetX()const { return ene2x; }
	int E2_GetY()const { return ene2y; }

	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
	void makeEnemy2();

	//�v���C���[����̍U��
	bool HitPlayer(float damage) { Enemy2_Hp -= damage; }
};