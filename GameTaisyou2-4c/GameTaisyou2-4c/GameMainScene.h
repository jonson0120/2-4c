#pragma once
#pragma once
#include "AbstractScene.h"
#include"common.h"
class GameMainScene : public AbstractScene
{
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
};