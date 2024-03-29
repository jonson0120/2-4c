#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
#include "Enemy2.h"
#include"UI.h"
#include"TreasureBox.h"


class TestMap : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ
	int CheckData[MAP_HEIGHT][MAP_WIDTH];	//空間チェック用マップデータ
	int MapImg[4];			//マップ画像
	int MapExitX; //マップの出口のX座標
	int MapExitY; //マップの出口のY座標

	int Art[8];		//敵アイコンなど
	int info[3];	//案内板
	int MapType;	//マップ地形タイプ 0:平坦 1:段差 2:階段

	int time;
	Player player;
	Enemy** enemy;
	Item** item;
	//Enemy2 enemy2;
	UI ui;

	int CameraX;
	int CameraY;

	int hit = 0;

	int Bright; //輝度
	int Bright_minus; //輝度を引くためのもの

	bool Exit_flg; //出口についてるかを判定する
	int AnimTimer; //アニメーション用のタイマー
	bool Anim_flg; //アニメーション用のフラグ
	bool MakeMap_flg; //マップを作るか判断する
	bool MoveStop_flg; //プレイヤーの行動制御フラグ
	bool Pressed_flg; //出口に着いた時にボタン長押し入力を受け入れるか判定する

	int x, y;

	int count;
public:
	TestMap();
	//デストラクト
	virtual ~TestMap()
	{
		//delete player;//newを作ったらdeleteを作る
	};
	//描画以外の更新を実装する
	virtual AbstractScene* Update()override;

	void SortEnemy();
	void SortItem();

	//描画に関するころを実装する
	virtual void Draw()const override;

	void MakeMap();
	int CheckSpace(int y, int x, int* cnt);
	void NextMap();
	void ExitCheck();

	void SearchEnemy();
};