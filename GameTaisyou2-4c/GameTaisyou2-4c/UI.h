#pragma once
#include "Player.h"
#include"PadInput.h"

enum Price
{
	HP,
	ATK,
	HEAL,
};

class UI
{
private:
	Stat stat;

	float MaxHP;
	float PlayerHP;

	int Shard;

	bool potionflag;

	int Damage;


	int Weapon;	//現在装備している武器
	int Second;	//現在装備していない武器

	int DaggerImage;
	int MaceImage;
	int SpearImage;
	int KatanaImage;
	int NoneImage;
	 
	int Gauge[3];

	int ButtonImg[2];

	int ItemImg[2];

	int Number[44];		//数字
	int Chara[28];		//文字
	int Sign[4];		//符号

	Passive passive[4];	//表示するパッシブ
	int passiveimg[7];	//パッシブ画像

	int PotionCount;
	int PotionMax;

	int UseCount = 0;

	int MenuNum = 0;	//メニュー：選択項目

	int UpGradeImg[6];	//強化メニュー：画像
	int UpGradeTxt[4];	//強化メニュー：文字

	int WaitTime = 0;		//入力待ち時間

	int cost[3];		//強化：必要コスト
	int nowstat[3];		//強化：現在の値
	bool Title = false;		//ポーズ：最終確認

public:
	
	UI();
	void Update(Player* player);

	//強化メニュー
	bool UpGradeUI(Player* player);

	Pause PauseUI();

	void Draw()const;
	void UpGradeDraw()const;
	void PauseDraw()const;
};

