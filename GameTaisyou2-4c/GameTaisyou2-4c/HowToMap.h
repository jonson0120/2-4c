#pragma once
#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"HowToPlayer.h"
#include"Enemy.h"
#include"Item.h"
#include "Enemy2.h"
#include"UI.h"
#include"TreasureBox.h"


class HowToMap : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH_T];	//マップデータ
	int ImgData[MAP_HEIGHT][MAP_WIDTH_T];	//マップ背景データ
	int MapImg[8];			//マップ画像
	int Block;				//ブロック
	int HowToImg[13];		//操作説明画像

	int Phase = 0;				//進行度
	int Anim = 0;				//説明アニメーション
	int PhaseCount = 0;			//フェーズ進行時アニメーション

	int MapExitX; //マップの出口のX座標
	int MapExitY; //マップの出口のY座標

	int Dagger[2];		//武器画像
	int Icon;		//アイコン
	bool Getted;	//武器を拾ったか

	int time;
	HowToPlayer player;

	int CameraX;
	int CameraY;

	int hit = 0;

	int Bright; //輝度
	int Bright_minus; //輝度を引くためのもの

	bool Exit_flg; //出口についてるかを判定する
	int AnimTimer; //アニメーション用のタイマー
	bool Anim_flg; //アニメーション用のフラグ

	bool GoMain_flg; //メインに移動するか判断する
	bool GoTitle_flg; //メインに移動するか判断する

	bool MoveStop_flg; //プレイヤーの行動制御フラグ
	bool Pressed_flg; //出口に着いた時にボタン長押し入力を受け入れるか判定する

	int DoorIcon[3];	//出口などに表示するアイコン

	int x, y;

	int count;

	int TutorialBGM;
	int WeaponSE;
	
public:
	HowToMap();
	//デストラクト
	virtual ~HowToMap()
	{
		//delete player;//newを作ったらdeleteを作る
	};
	//描画以外の更新を実装する
	virtual AbstractScene* Update()override;

	//描画に関するころを実装する
	virtual void Draw()const override;

	void NextMap();

	void MakeMap();
};