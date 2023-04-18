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

struct Range
{
	int X;
	int Y;
};

struct Stat
{
	int Hp;			//体力
	int MaxHp;		//最大体力
	int Atk;		//基礎攻撃力
	float Power;	//攻撃倍率
	int Potion;		//ポーション所持数
	int PotionMax;	//ポーション最大所持数
	int Shard;		//シャード所持数
};