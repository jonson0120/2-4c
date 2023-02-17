#pragma once
#include"DxLib.h"
#include"Enemy.h"
#include"common.h"

enum struct weapons 
{
	dagger, //0
	mace	//1
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
	int Hp;
	float Power;
};

class Player
{
private:
	int image;	//画像

	Stat stat;	//ステータス

	int x, y;		//X,Y座標
	int Width, Height;	//プレイヤー高さ

	int a = 0, b = 0;
	int c = 0, d = 0;

	float speed;	//移動速度
	float fall;	//落下速度
	int jump;	//ジャンプ回数

	float Attack;	

	int PImages[2];
	bool TurnFlg;
	bool FalseFlg;
	

	int Weapon[2];		//武器画像
	enum weapons Equip;		//装備している武器
	Range range[2];	//攻撃範囲
	Inp_UD Yinput;	//上下入力
	int Combo;		//コンボ数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

	int JoypadX, JoypadY;	//パッド入力値

public:

	void InitPad();
	Player();
	void Update();
	void Draw() const;


	int GetX()const { return x; }
	int GetY()const { return y; }

	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	weapons GetEquip()const { return Equip; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

	void DrawDagger()const;
	void DrawMace()const;

	void DaggerAtk();
	void MaceAtk();

	bool HitDagger(int EneX, int EneY, int EneW, int EneH);
	bool HitMace(int EneX, int EneY, int EneW, int EneH);
};

