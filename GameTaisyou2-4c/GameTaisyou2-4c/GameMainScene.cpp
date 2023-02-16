#include"DxLib.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"
#include"UI.h"

GameMainScene::GameMainScene()
{
	MakeMap();
	player.SetMapData(MapData);
	enemy.SetMapData(MapData);

	LoadDivGraph("images/Block.png", 2, 2, 1, 160, 160, MapImg);

	time = 0;

	CameraX = 0;
	CameraY = 0;

}

AbstractScene* GameMainScene::Update() 
{
	ui.Update();
	enemy.Update();
	player.Update();
	CameraX = player.GetX();
	CameraY = player.GetY();

	player.HitAttack(enemy.GetX(), enemy.GetY(), enemy.GetWidth(), enemy.GetHeight());

	time++;
	return this;
}

void GameMainScene::Draw() const
{

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY(), MapImg[MapData[i][j]], TRUE);
		}
	}
	DrawFormatString(0, 0, 0xffffff, "%d", time);


	ui.Draw();
	enemy.Draw(player.GetX(),player.GetY());
	player.Draw();
	

}

void GameMainScene::MakeMap() 
{
	for (int i = 0; i < MAP_HEIGHT; i++) 
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (j == MAP_WIDTH - 1 || j == 0)
			{
				MapData[i][j] = 0;
			}
			else
			{
				MapData[i][j] = 1;
			}

			if (i == MAP_HEIGHT - 1 || i == 0)
			{
				MapData[i][j] = 0;
			}
		}
	}
}