#pragma once
#include "DxLib.h"
#include "common.h"
#include "Enemy.h"

class Player;

class Boss :public Enemy
{
private:
	int image;			//�摜
	int EImages[5];		//�摜
	int Anim;			//�A�j���[�V����

	int DropItem_Image;	//�h���b�v�A�C�e��
	int enex, eney;		//X,Y���W
	int Width, Height;	//�G�̍���
	int direction;		//�G�̌���

	int Power;			//�U����

	bool E_AttackFlg;
	bool HighJump;
	bool LowJump;
	int Attack;			//�U���A�j���[�V����
	int AttackSpeed;
	float AttackVec;		//�U���p�x
	int Dive;

	bool Turnflg;		//�摜���]

	int Player_Damage;
	int Enemy_Damage;

	int Enemy_Hp;
	int Player_Hp;

	bool MakeEnemy;

	//Player player;

	int AttackCool;	//�U���N�[���^�C��
	int HitCool;	//��e�N�[���^�C��

	float speed; //�ړ����x
	float fall;	 //�������x
	int jump;	 //�W�����v��

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

public:
	Boss(int level);
	virtual void Update(Player* player);
	virtual void Draw(int x, int y) const;

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

