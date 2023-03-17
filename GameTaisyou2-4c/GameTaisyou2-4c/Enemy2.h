#pragma once
#include "Enemy.h"

class Player;

class Enemy2 : public Enemy
{
private:
	int image;			//画像
	int E2Images[2];		//画像
	int DropItem_Image;	//ドロップアイテム
	int ene2x, ene2y;		//X,Y座標
	int Width, Height;	//敵の高さ
	int direction;		//敵の向き
	int E2_AttackFlg;
	int Player_Damage;
	int Enemy2_Damage;
	int Enemy2_Hp;
	int Player_Hp;
	bool MakeEnemy2;

	//Player player;

	float speed; //移動速度
	float fall;	 //落下速度
	int jump;	 //ジャンプ回数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

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

	//プレイヤーからの攻撃
	bool HitPlayer(float damage) { Enemy2_Hp -= damage; }
};