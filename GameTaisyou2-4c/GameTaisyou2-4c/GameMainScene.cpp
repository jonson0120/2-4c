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
	player.Update();
	enemy.Update();
	CameraX = player.GetX();
	CameraY = player.GetY();

	switch (player.GetEquip())
	{
	case weapons::dagger:
		if (player.HitDagger(enemy.GetX(), enemy.GetY(), enemy.GetWidth(), enemy.GetHeight()))hit++;

	default:
		break;
	}

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
	
	//DrawFormatString(0, 0, 0xffffff, "%d", hit);
	ui.Draw();
	player.Draw();
	enemy.Draw(player.GetX(),player.GetY());

}

void GameMainScene::MakeMap()
{
	int map_parts[16][2][2] = {
		{{0,0},
		 {0,0}},

		{{1,0},
		 {0,0}},

		{{1,1},
		 {0,0}},

		{{1,1},
		 {1,0}},

		{{1,1},
		 {1,1}},

		{{0,1},
		 {1,1}},

		{{0,0},
		 {1,1}},

		{{0,0},
		 {0,1}},

		{{0,0},
		 {1,0}},

		{{0,0},
		 {1,1}},

		{{0,0},
		 {0,1}},

		{{1,0},
		 {1,1}},

		{{1,1},
		 {0,1}},

		{{1,1},
		 {1,0}},

		{{1,0},
		 {0,1}},

		{{0,1},
		 {1,0}},
	
	};

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

	/*for (int i = 1; i < MAP_HEIGHT - 1; i += 2)
	{
		for (int j = 1; j < MAP_WIDTH - 1; j += 2)
		{
			int parts = GetRand(16 - 1);
			MapData[i][j] = map_parts[parts][0][0];
			MapData[i + 1][j] = map_parts[parts][1][0];
			MapData[i][j + 1] = map_parts[parts][0][1];
			MapData[i + 1][j + 1] = map_parts[parts][1][1];
		}
	}*/
}