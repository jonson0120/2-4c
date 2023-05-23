#pragma once
#include "DxLib.h"
#include "common.h"
#include "Enemy.h"

class Grim_Reaper : public Enemy
{
private:
	int SickleImg;		//����摜
	int image;			//�摜
	int EImages[2];		//�摜
	int Anim;			//���s�A�j���[�V����

	int DropItem_Image;	//�h���b�v�A�C�e��
	int enex, eney;		//X,Y���W
	int Width, Height;	//�G�̍���
	int direction;		//�G�̌���

	int Power;			//�U����
	
	int E_Count;		//�v���C���[���߂Â�������������l

	bool E_AttackFlg;
	int Attack;			//�U���A�j���[�V����

	bool Turnflg;		//�摜���]

	int Player_Damage;
	int Enemy_Damage;

	int Enemy_Hp;
	int Player_Hp;

	bool MakeEnemy;

	//Player player;

	int HitCool;	//��e�N�[���^�C��

	float speed; //�ړ����x
	float fall;	 //�������x

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

public:
	Grim_Reaper(int level);
	virtual void Update(Player* player);
	virtual void Draw(int x, int y) const;
	void DrawSickle(int x, int y)const;

	virtual int E_GetX()const { return enex; }
	virtual int E_GetY()const { return eney; }

	virtual int GetWidth()const { return Width; }
	virtual int GetHeight()const { return Height; }

	virtual int GetPower()const { return Power; }

	//��e�N�[���擾
	virtual int GetCool()const { return HitCool; }

	virtual void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
	virtual void makeEnemy();

	bool CheckHp();

	void HitPlayer(float damage);
	bool EnemyAttack(int x, int y);
};

