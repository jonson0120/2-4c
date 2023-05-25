#pragma once
#include"DxLib.h"
#include"Enemy.h"
#include"common.h"

class Player
{
private:
	int PImages[5];	//画像：上半身
	int image_U[7];	//画像：下半身
	int ArmImg;
	int BarrierImg;	//画像：バリア

	int HealSE;
	int JumpSE;
	int Attack1SE;
	int KatanaSE;
	int WeaponSE;
	int ShardSE;
	int DamageSE;
	int DodgeSE;
	int DefenseSE;

	int Walk;	//歩行アニメーション管理

	Stat stat;	//ステータス
	int PotionCount;	//回復回数

	int x, y;		//X,Y座標
	int Width, Height;	//プレイヤー高さ

	Range Arm_L, Arm_R;
	int ArmAngle_L, ArmAngle_R;

	bool Move = true;	//入力受付
	int EndMove = 0;	//クリア演出

	float speedinit;	//移動速度最大
	float speed;	//移動速度
	float Dodgespd;	//回避速度
	float KnockBack;//ノックバック速度
	
	float fall;	//落下速度
	float Dodgefall;	//回避ジャンプ速度
	int jump;	//ジャンプ回数
	int wall;	//壁・天井移動 0:地面 1:左壁 2:右壁 3:天井

	int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

	float Attack;

	int UsePotion = 0;	//ポーション使用カウント
	int HitCool;		//被弾後無敵

	bool TurnFlg;
	bool FalseFlg;
	

	int Weapon[4];		//武器画像
	int WeaponEffect[4];	//武器効果

	enum weapons Equip[2];		//持っている武器
	int EquipNum;				//装備している武器
	Passive passive[2][4];		//装備パッシブ効果

	//パッシブ関連ステータス
	int TotalAtk;	//総合攻撃力
	int Defense;	//軽減ダメージ
	int drop;		//シャード増加数
	int barrier;	//バリア回数
	int vamp;		//攻撃回復
	int repair;		//自動回復
	int dodge;		//回避率
	//----------------------
	int nowbarrier;	//実効バリア回数

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
	void Draw(bool boss) const;

	void Spawn();

	//座標取得
	int GetX()const { return x; }
	int GetY()const { return y; }

	//サイズ取得
	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	//座標セット
	void SetX(int X) { x = X; }
	void SetY(int Y) { y = Y; }

	//装備取得・更新
	weapons GetEquip()const { return Equip[EquipNum]; }
	weapons GetSecond()const {
		if (EquipNum)return Equip[0];
		else return Equip[1];
	}

	weapons Secondary()const { return Equip[1]; }

	//武器変更
	void ChangeEquip(weapons get, Passive passive[4]);

	//ステータス取得---------------------------
	Stat GetStat() { return stat; }

	int GetLife() { return stat.Hp; }
	int GetMaxLife() { return stat.MaxHp; }

	int GetAtk() { return stat.Atk; }
	int GetDmg() { return TotalAtk; }

	int GetPotion() { return stat.Potion; }
	int GetPotionMax() { return stat.PotionMax; }

	int GetShard() { return stat.Shard; }


	//パッシブステータス
	Passive GetPassive(int index) { return passive[EquipNum][index]; }

	int GetDef() { return Defense; }
	int GetDrop() { return drop; }
	int GetVamp() { return vamp; }
	//-----------------------------------------
	int UseP() { return UsePotion; }
	bool GetCool() { return HitCool; }

	//ステータス変化
	//パッシブ適用 (引数：変更する武器配列)
	void SetPassive(int num);

	//パッシブ：バリア
	void SetBarrier() { nowbarrier = barrier; }

	//シャード取得
	void AddShard() {PlaySoundMem(ShardSE, DX_PLAYTYPE_BACK); stat.Shard++; }

	//状態リセット
	void ResetPosition() 
	{
		Attack = 0;
		TurnFlg = false;

		Arm_L = { SCREEN_WIDTH / 2 + 13, SCREEN_HEIGHT / 2 };
		ArmAngle_L = 0;

		Arm_R = { SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 };
		ArmAngle_R = 0;
	}

	//体力強化
	void StrHP(int add) 
	{
		stat.MaxHp += add;
		stat.Hp += add;
	}

	//攻撃強化
	void StrAtk() {
		stat.Atk++;
		SetPassive(EquipNum);
	}

	//回復力強化
	void StrHeal() 
	{
		stat.PotionMax++;
		stat.Potion++;
		stat.PotionPower += 0.05;
	}

	//状態リセットシャード使用
	bool UseShard(int amount) { 

		if (amount <= stat.Shard) {
			stat.Shard -= amount;
			return true;
		}
		else return false;
	}

	//マップデータ取得
	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

	//武器描画
	void DrawDagger(int fix)const;
	void DrawMace(int fix)const;
	void DrawSpear(int fix)const;
	void DrawKatana(int fix)const;

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

	//攻撃入力・攻撃力取得
	float GetAttack() { return Attack; }
	float GetPower() { return stat.Power; }

	//索敵
	bool WaitSearch() { return Search; }
	void SetNear(int X, int Y, int Dis);

	//敵との当たり判定(返り値　0：ダメージを受けた　1：回避した　2：バリアで防いだ)
	int HitEnemy(float damage, int EneX);

	//ノックバック
	void SetKnockBack(float power,int EneX)
	{
		if (EneX < x)KnockBack = power;
		else KnockBack = -power;
	}

	//全回復
	void Reset() 
	{
		stat.Hp = stat.MaxHp;
		stat.Potion = stat.PotionMax;
	}

	//攻撃回復
	void Vamp() { 
		stat.Hp += vamp; 
		if (stat.MaxHp < stat.Hp)stat.Hp = stat.MaxHp;
	}

	//入力可否の切り替え
	void SwitchMove() { Move = !Move; }
};

