#pragma once
#include"DxLib.h"
#include"Enemy.h"
#include"common.h"

enum weapons 
{
	dagger //0
};

enum Input_UpDown
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

class Player
{
private:
	int image;	//画像

	int x, y;		//X,Y座標
	int Width, Height;	//プレイヤー高さ

	float speed;	//移動速度
	float fall;	//落下速度
	int jump;	//ジャンプ回数

	int Attack;		//攻撃フラグ

	int PImages[2];
	bool TurnFlg;
	

	int Weapon;		//武器画像
	enum weapons Equip;		//装備している武器
	Range range[1];	//攻撃範囲
	Input_UpDown Yinput;	//上下入力
	int Combo;		//コンボ数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

	int JoypadX, JoypadY;	//パッド入力値

public:

	void InitPad();
	Player();
	void Update();
	void Draw() const;

	void DrawDagger()const;

	int GetX()const { return x; }
	int GetY()const { return y; }

	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

	void DaggerAtk();

	bool HitAttack(int EneX, int EneY, int EneW, int EneH);
};

