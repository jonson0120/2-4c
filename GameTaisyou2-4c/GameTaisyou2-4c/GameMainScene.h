#pragma once
#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"Player.h"
#include"Enemy.h"
#include "Enemy2.h"
#include"UI.h"
#include"TreasureBox.h"


class GameMainScene : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ
	int CheckData[MAP_HEIGHT][MAP_WIDTH];	//空間チェック用マップデータ
	int MapImg[4];			//マップ画像
	int MapExitX; //マップの出口のX座標
	int MapExitY; //マップの出口のY座標

	int time;
	Player player;
	Enemy enemy;
	Enemy2 enemy2;
	UI ui;
	TreasureBox treasurebox;

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

	int x, y;

public:
	GameMainScene();
	//デストラクト
	virtual ~GameMainScene()
	{
		//delete player;//newを作ったらdeleteを作る
	};
	//描画以外の更新を実装する
	virtual AbstractScene* Update()override;

	//描画に関するころを実装する
	virtual void Draw()const override;

	void MakeMap();
	int CheckSpace(int y, int x, int* cnt);
	void NextMap();
	void ExitCheck();
};