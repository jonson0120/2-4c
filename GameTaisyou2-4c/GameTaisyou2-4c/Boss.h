#pragma once
#include "DxLib.h"
#include "common.h"
#include "Enemy.h"

class Player;

class Boss :public Enemy
{
private:
	int image;			//画像
	int EImages[10];		//画像
	int Anim;			//アニメーション

	int DropItem_Image;	//ドロップアイテム
	int enex, eney;		//X,Y座標
	int Width, Height;	//敵の高さ
	int direction;		//敵の向き

	int Power;			//攻撃力

	//行動制御---------------
	int Movecnt = 0;			//移動切り替え時間
	int Moveswitch = 60;		//移動切り替えタイミング
	int MoveAng = 1;			//移動方向(1か-1)

	bool Pounce = false;			//飛び掛かり
	int JumpDis = 0;			//飛び掛かり距離

	bool Claw = false;			//爪攻撃
	bool ClawTurn = false;		//爪攻撃方向
	int ClawCool = 450;			//爪攻撃頻度
	int ClawTime = 0;			//爪攻撃時間
	float ClawX = 0;				//攻撃位置
	float ClawY = 0;				//攻撃位置
	float ClawSpd = 0;			//攻撃速度
	int ClawImg[6];				//爪画像

	bool Roar = false;			//咆哮
	int Oldx = 0;				//1フレーム前のx座標
	int RoarTime = 0;			//咆哮までの時間
	int RoarImg;				//咆哮画像

	//-----------------------

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

	void FixX();
	void FixY();
};

