#pragma once
#include "DxLib.h"
#include "common.h"
#include "Enemy.h"

class Player;

class Slime : public Enemy
{
private:
	int image;			//画像
	int EImages[5];		//画像
	int Anim;			//歩行アニメーション

	int enex, eney;		//X,Y座標
	int Width, Height;	//敵の高さ
	bool Turnflg;		//敵の向き

	int Power;			//攻撃力

	bool E_AttackFlg;
	bool HighJump;
	int Attack;			//攻撃アニメーション

	int Enemy_Hp;

	bool MakeEnemy;

	//Player player;

	int AttackCool;	//攻撃クールタイム
	int HitCool;	//被弾クールタイム
	
	float speed; //移動速度
	float fall;	 //落下速度
	int jump;	 //ジャンプ回数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

public:
	Slime(int level);
	virtual void Update(Player* player) ;
	virtual void Draw(int x, int y) const ;

	//座標取得
	virtual int E_GetX()const  { return enex; }
	virtual int E_GetY()const  { return eney; }

	//幅、高さ取得
	virtual int GetWidth()const  { return Width; }
	virtual int GetHeight()const   { return Height; }

	//攻撃力取得
	virtual int GetPower()const { return Power; }

	//被弾クール取得
	virtual int GetCool()const { return HitCool; }

	//敵配置
	virtual void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]) ;
	virtual void makeEnemy() ;

	bool CheckHp() ;

	//ダメージを受ける
	void HitPlayer(float damage);

	//敵の攻撃当たり
	bool EnemyAttack(int x, int y);
};