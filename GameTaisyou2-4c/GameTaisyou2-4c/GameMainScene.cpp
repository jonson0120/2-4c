#include"DxLib.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"

GameMainScene::GameMainScene()
{
	MakeMap();
	LoadDivGraph("images/Block.png", 2, 2, 1, 160, 160, MapImg);

}

AbstractScene* GameMainScene::Update() 
{
	
	return this;
}

void GameMainScene::Draw() const
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			DrawGraph(160 * j, 160 * i, MapImg[MapData[i][j]], TRUE);
		}
	}
}

void GameMainScene::MakeMap() 
{
	for (int i = 0; i < 12; i++) 
	{
		for (int j = 0; j < 13; j++)
		{
			if (j == 12 || j == 0)
			{
				MapData[i][j] = 0;
			}
			else
			{
				MapData[i][j] = 1;
			}

			if (i == 11 || i == 0)
			{
				MapData[i][j] = 0;
			}
		}
	}
}