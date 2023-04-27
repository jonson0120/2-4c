#pragma once

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280
#define SCREEN_COLOR_BIT_16 16

#define MAP_HEIGHT 11
#define MAP_WIDTH 14
#define MAP_WIDTH_T 50

#define BLOCK_SIZE 160

#define ITEM_MAX 128
#define ENEMY_MAX 20
#define TREASURE_MAX 3

enum struct weapons
{
	dagger, //0
	mace,	//1
	spear,	//2
	katana,	//3
	NONE	//4
};

enum struct Inp_UD
{
	NONE,	//0
	UP,		//1
	DOWN	//2
};

enum struct Pause
{
	NONE,
	RETURN,
	TITLE
};

struct Range
{
	int X;
	int Y;
};

struct Stat
{
	int Hp;			//�̗�
	int MaxHp;		//�ő�̗�
	int Atk;		//��b�U����
	float Power;	//�U���{��
	int Potion;		//�|�[�V����������
	int PotionMax;	//�|�[�V�����ő及����
	float PotionPower;//�|�[�V�������ʗ�
	int Shard;		//�V���[�h������
};

struct LocNum
{
	int X;		//X���W
	int Y;		//Y���W
	int NumA;	//�C�ӂ̐��l
	int NumB;	//�C�ӂ̐��l
};