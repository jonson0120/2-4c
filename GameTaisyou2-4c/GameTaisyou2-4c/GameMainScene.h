#pragma once
#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"Player.h"
#include"Enemy.h"
#include"UI.h"

class GameMainScene : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ
	int CheckData[MAP_HEIGHT][MAP_WIDTH];	//空間チェック用マップデータ
	int MapImg[2];			//マップ画像
	int Space;				//空間の数

	int time;
	Player player;
	Enemy enemy;
	UI ui;

	int CameraX;
	int CameraY;

	int hit = 0;

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
};