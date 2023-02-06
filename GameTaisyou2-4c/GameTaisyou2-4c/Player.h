#pragma once
#include"DxLib.h"
#include"common.h"

class Player
{
private:
	int image;	//画像

	int x, y;		//X,Y座標
	float speed;	//移動速度

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

	int JoypadX, JoypadY;	//パッド入力値

public:

	void InitPad();
	Player();
	void Update();
	void Draw() const;

	int GetX()const { return (int)x; }
	int GetY()const { return (int)y; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

};

