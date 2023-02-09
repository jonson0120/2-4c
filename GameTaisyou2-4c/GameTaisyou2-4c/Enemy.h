#pragma once
#include"DxLib.h"
#include"common.h"
#include"Player.h"
class Enemy
{
private:
	int image;	//画像

	int EImages[5];
	int EnemyImage = LoadGraph("images/slime1.png");
	int enex, eney;		//X,Y座標
	int Width, Height;	//敵の高さ

	float speed;	//移動速度
	float fall;	//落下速度
	int jump;	//ジャンプ回数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

public:

	Enemy();
	void Update();
	void Draw(int x,int y) const;

	int GetX()const { return enex; }
	int GetY()const { return eney; }

	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

};