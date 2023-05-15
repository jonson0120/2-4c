#pragma once
#include "DxLib.h"
#include "common.h"
#include "Enemy.h"

class Player;

class Boss :public Enemy
{
private:
	int image;			//画像
	int EImages[5];		//画像
	int Anim;			//アニメーション

	int DropItem_Image;	//ドロップアイテム
	int enex, eney;		//X,Y座標
	int Width, Height;	//敵の高さ
	int direction;		//敵の向き

	int Power;			//攻撃力

	bool E_AttackFlg;
	bool HighJump;
	bool LowJump;
	int Attack;			//攻撃アニメーション
	int AttackSpeed;
	float AttackVec;		//攻撃角度
	int Dive;

	bool Turnflg;		//画像反転

	int Player_Damage;
	int Enemy_Damage;

	int Enemy_Hp;
	int Player_Hp;

	bool MakeEnemy;

	//Player player;

	int AttackCool;	//攻撃クールタイム
	int HitCool;	//被弾クールタイム

	float speed; //移動速度
	float fall;	 //落下速度
	int jump;	 //ジャンプ回数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

public:
	Boss(int level);
	virtual void Update(Player* player);
	virtual void Draw(int x, int y) const;

	virtual int E_GetX()const { return enex; }
	virtual int E_GetY()const { return eney; }

	virtual int GetWidth()const { return Width; }
	virtual int GetHeight()const { return Height; }

	virtual int GetPower()const { return Power; }

	//被弾クール取得
	virtual int GetCool()const { return HitCool; }

	virtual void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
	virtual void makeEnemy();

	bool CheckHp();

	void HitPlayer(float damage);
	bool EnemyAttack(int x, int y);
};

