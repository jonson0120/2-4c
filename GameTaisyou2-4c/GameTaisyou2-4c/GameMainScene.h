#pragma once
#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"Player.h"
class GameMainScene : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ
	int MapImg[2];			//マップ画像

	int time;
	Player player;

	int CameraX;
	int CameraY;

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
};