#pragma once
#include "DxLib.h"
#include "common.h"
#include "Enemy.h"

class Grim_Reaper : public Enemy
{
private:
	int SickleImg;		//武器画像
	int image;			//画像
	int EImages[2];		//画像
	int Anim;			//歩行アニメーション

	int DropItem_Image;	//ドロップアイテム
	int enex, eney;		//X,Y座標
	int Width, Height;	//敵の高さ
	int direction;		//敵の向き

	int Power;			//攻撃力
	
	int E_Count;		//プレイヤーが近づいた時増加する値

	bool E_AttackFlg;
	int Attack;			//攻撃アニメーション

	bool Turnflg;		//画像反転

	int Player_Damage;
	int Enemy_Damage;

	int Enemy_Hp;
	int Player_Hp;

	bool MakeEnemy;

	//Player player;

	int HitCool;	//被弾クールタイム

	float speed; //移動速度
	float fall;	 //落下速度

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

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

	//被弾クール取得
	virtual int GetCool()const { return HitCool; }

	virtual void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
	virtual void makeEnemy();

	bool CheckHp();

	void HitPlayer(float damage);
	bool EnemyAttack(int x, int y);
};

