#pragma once
#include"DxLib.h"
#include"Enemy.h"
#include"common.h"

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
	int Hp;
	float Power;
};

class Player
{
private:
	int PImages[5];	//画像：上半身
	int image_U[7];	//画像：下半身
	int ArmImg;

	int Walk;	//歩行アニメーション管理

	Stat stat;	//ステータス

	int x, y;		//X,Y座標
	int Width, Height;	//プレイヤー高さ

	Range Arm_L, Arm_R;
	int ArmAngle_L, ArmAngle_R;

	float speedinit;	//移動速度最大
	float speed;	//移動速度
	float Dodgespd;	//回避速度
	
	float fall;	//落下速度
	float Dodgefall;	//回避ジャンプ速度
	int jump;	//ジャンプ回数
	int wall;	//壁・天井移動 0:地面 1:左壁 2:右壁 3:天井

	int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

	float Attack;	

	bool TurnFlg;
	bool FalseFlg;
	

	int Weapon[4];		//武器画像
	int WeaponEffect[4];	//武器効果

	enum weapons Equip[2];		//持っている武器
	int EquipNum;				//装備している武器

	Range range[4];	//攻撃範囲
	Inp_UD Yinput;	//上下入力
	int Combo;		//コンボ数

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

	int JoypadX, JoypadY;	//パッド入力値
	float PadangL;			//パッド入力角度
	int TriggerL, TriggerR;	//トリガー入力値

	int Enemy_Damage;//敵の攻撃力

	//一部の武器種が使用する変数-----------
	int Atkpt;				//汎用・攻撃パターン
	bool Search;			//索敵が必要な時にTRUE
	Range Near_Enemy;		//最も近くの敵座標
	int NearEneDis;			//最も近くの敵までの距離

	float spear_angle;		//槍・攻撃方向

	int Katana_num = 8;		//刀・斬撃の数
	Range katana_slash[8];		//刀・斬撃位置
	int katana_angle[8];		//刀・斬撃角度

	//-------------------------------------

public:

	void InitPad();
	Player();
	void Update();
	void Draw() const;

	void Spawn();

	//座標取得
	int GetX()const { return x; }
	int GetY()const { return y; }

	//サイズ取得
	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	//装備取得
	weapons GetEquip()const { return Equip[EquipNum]; }

	//マップデータ取得
	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

	//武器描画
	void DrawDagger()const;
	void DrawMace()const;
	void DrawSpear()const;
	void DrawKatana()const;

	//武器アニメーション管理
	void DaggerAtk();
	void MaceAtk();
	void SpearAtk();
	void KatanaAtk();

	//当たり判定
	bool HitDagger(int EneX, int EneY, int EneW, int EneH);
	bool HitMace(int EneX, int EneY, int EneW, int EneH);
	bool HitSpear(int EneX, int EneY, int EneW, int EneH);
	bool HitKatana(int EneX, int EneY, int EneW, int EneH);

	//攻撃力取得
	float GetPower() { return stat.Power; }

	//索敵
	bool WaitSearch() { return Search; }
	void SetNear(int X, int Y, int Dis);

	//敵との当たり判定
	void HitEnemy(float damage) { stat.Hp -= damage; }
};

