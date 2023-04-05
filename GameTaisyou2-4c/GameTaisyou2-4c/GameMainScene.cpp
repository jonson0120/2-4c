#include"DxLib.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"
#include"UI.h"
#include<math.h>
#include<stdlib.h>

#include"Slime.h"

GameMainScene::GameMainScene()
{
	enemy = new Enemy * [ENEMY_MAX];
	item = new Item * [ITEM_MAX];

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i] = nullptr;
	}
	enemy[0] = new Slime();

	for (int i = 0; i < ITEM_MAX; i++)
	{
		item[i] = nullptr;
	}
	item[0] = new Item(1, weapons::mace, { 880,880 });

	MapExitX = 0;
	MapExitY = 0;
	MakeMap();
	player.SetMapData(MapData);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr) 
		{
			enemy[i]->SetMapData(MapData);
			enemy[i]->makeEnemy();
		}
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)
		{
			item[i]->SetMapData(MapData);
		}
	}
	//enemy2.SetMapData(MapData);

	treasurebox.SetMapData(MapData);
	

	LoadDivGraph("images/Block.png", 4, 4, 1, 160, 160, MapImg);

	time = 0;

	count = 0;

	CameraX = 0;
	CameraY = 0;

	Bright = 255;
	Bright_minus = 10;
	AnimTimer = 0;

	Exit_flg = false;
	Anim_flg = false;
	MakeMap_flg = false;
	MoveStop_flg = true;
}

AbstractScene* GameMainScene::Update() 
{
	if (MoveStop_flg == true)
	{
		if (player.WaitSearch())SearchEnemy();
		player.Update();
		ui.Update();
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if(enemy[i]!=nullptr)enemy[i]->Update(&player);
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)item[i]->Update(&player);
	}
	//enemy2.Update(&player);
	CameraX = player.GetX();
	CameraY = player.GetY();

	
	treasurebox.Update(&player);

	switch (player.GetEquip())
	{
	case weapons::dagger:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitDagger(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()))enemy[i]->HitPlayer(player.GetPower());
		}
		break;
	case weapons::mace:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitMace(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()))enemy[i]->HitPlayer(player.GetPower());
		}
		break;
	case weapons::spear:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitSpear(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()))enemy[i]->HitPlayer(player.GetPower());
		}
		break;
	case weapons::katana:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitKatana(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()))enemy[i]->HitPlayer(player.GetPower());
		}
		break;
	default:
		break;
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr) 
		{
			if (enemy[i]->CheckHp())enemy[i] = nullptr;
		}
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr)break;
		if (i == ENEMY_MAX - 1)MapData[MapExitX][MapExitY] = 3;
	}
	/*if (player.GetX() / 160 == MapExitY && player.GetY() / 160 == MapExitX) Exit_flg = true;*/

	ExitCheck();
	if (Exit_flg == true) NextMap();
	x= MapExitY * 160 + 80;
	y= MapExitX * 160 + 131;
	time++;
	return this;
}

void GameMainScene::Draw() const
{

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (MapData[i][j] < 4)DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY(), MapImg[MapData[i][j]], TRUE);
		}
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)item[i]->Draw({ player.GetX() ,player.GetY() });
	}
	
	//DrawFormatString(0, 500, 0xff0000, "%d", Space);
	treasurebox.Draw(player.GetX(), player.GetY());
	player.Draw();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr)enemy[i]->Draw(player.GetX(), player.GetY());
	}

	//enemy2.Draw(player.GetX(), player.GetY());

	ui.Draw();

	DrawFormatString(0, 500, 0xff0000, "%d", AnimTimer);
	DrawFormatString(0, 550, 0xff0000, "%d", Bright);
	DrawFormatString(0, 600, 0xff0000, "%d",CameraX);
	DrawFormatString(50, 600, 0xff0000, "%d", CameraY);
	DrawFormatString(0, 650, 0xff0000, "%d", x);
	DrawFormatString(50, 650, 0xff0000, "%d", y);
	DrawCircle(160 * (4 + MapExitY) + 80 - player.GetX(), 360 + 160 * MapExitX + 120 - player.GetY(), 4, 0xff0000, TRUE);
	DrawFormatString(500, 200, 0xffffff, "%d", hit);
	DrawFormatString(0, 700, 0xff0000, "%d", count);
}

//マップ生成
void GameMainScene::MakeMap()
{
	//壁・空間パターン
	int parts_max = 0;
	int map_parts[][3][3] = {

		{{1,1,1},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {1,0,1},
		 {1,1,1},},

		{{0,1,0},
		 {1,1,1},
		 {0,1,0},},

		{{0,0,0},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {0,0,0},
		 {1,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,0,0},},

		{{0,1,1},
		 {0,1,1},
		 {0,1,1},},

		{{1,0,1},
		 {1,0,1},
		 {1,0,1},},

		{{1,1,0},
		 {1,1,0},
		 {1,1,0},},

		{{0,1,1},
		 {1,0,1},
		 {1,1,0},},

		{{1,1,0},
		 {1,0,1},
		 {0,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {1,1,0},},

		{{1,1,0},
		 {1,1,1},
		 {1,1,1},},

		{{0,1,1},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,0},
		 {1,0,0},
		 {0,0,1},},

		{{1,0,0},
		 {0,0,1},
		 {0,1,1},},

		{{0,1,1},
		 {0,0,1},
		 {1,0,0},},

		{{0,0,1},
		 {1,0,0},
		 {1,1,0},},

		{{1,1,1},
		 {1,1,1},
		 {1,1,0},},

		{{1,1,0},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,1,1},},

		{{0,1,1},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,0,1},},

		{{1,1,1},
		 {1,1,0},
		 {1,1,0},},

		{{1,0,0},
		 {1,1,1},
		 {1,1,1},},

		{ {0,1,1},
		  {0,1,1},
		  {1,1,1}, },
	};
	parts_max = sizeof(map_parts) / sizeof(*map_parts);

	//出口生成チェック
	bool MakeExit = false;

	int Space = 0;				//空間の数

	int x = player.GetX();

	//マップデータ作成
	do {

		//チェックに使用するデータリセット-------
		Space = 0;
		MakeExit = false;
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				CheckData[i][j] = 0;
				MapData[i][j] = 0;
			}
		}
		//-------------------------------------------
		
		//壁生成----------------------------------------------------
		for (int i = 1; i < MAP_HEIGHT - 1; i += 3)
		{
			for (int j = 1; j < MAP_WIDTH - 1; j += 3)
			{
				int parts = GetRand(parts_max - 1);	//使用するパターンを決める

				//パターンに応じて壁を作る
				MapData[i][j] = map_parts[parts][0][0];
				MapData[i + 1][j] = map_parts[parts][1][0];
				MapData[i + 2][j] = map_parts[parts][2][0];

				MapData[i][j + 1] = map_parts[parts][0][1];
				MapData[i + 1][j + 1] = map_parts[parts][1][1];
				MapData[i + 2][j + 1] = map_parts[parts][2][1];

				MapData[i][j + 2] = map_parts[parts][0][2];
				MapData[i + 1][j + 2] = map_parts[parts][1][2];
				MapData[i + 2][j + 2] = map_parts[parts][2][2];
			}
		}
		//---------------------------------------------------------------

		//マップ端の壁・天井-----------------------
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (j == MAP_WIDTH - 1 || j == 0)
				{
					MapData[i][j] = 0;
				}
				//else MapData[i][j] = 1;

				if (i == MAP_HEIGHT - 1 || i == 0)
				{
					MapData[i][j] = 0;
				}
			}
		}
		//------------------------------------------

		//プレイヤーの初期位置を足場のある空間にする
		MapData[player.GetY() / 160][player.GetX() / 160] = 1;
		MapData[player.GetY() / 160][(player.GetX() / 160) + 1] = 1;
		MapData[player.GetY() / 160 + 1][player.GetX() / 160] = 0;

		//空間数チェック
		CheckSpace(player.GetY() / 160, player.GetX() / 160, &Space);

		//出口を設置
		/*for (int j = MAP_WIDTH - 1; 0 < j && !MakeExit; j--)
		{
			for (int i = MAP_HEIGHT - 1; 0 < i && !MakeExit; i--)
			{
				if (CheckData[i][j] && MapData[i][j] == 1 && MapData[i + 1][j] == 0)
				{
					MapData[i][j] = 2;
					MapExitX = i;
					MapExitY = j;
					MakeExit = true;
				}
			}
		}*/


		//空間数が一定以下なら再生成
	} while (Space < 70);

	MakeExit = MakeExit;
	
	while (MakeExit == false)
	{
		int i = GetRand(MAP_HEIGHT);
		int j = GetRand(MAP_WIDTH - 3) + 2;
		if (i != CameraY / 160 && j != CameraX / 160) {
			if (CheckData[i][j] && MapData[i][j] == 1 && MapData[i + 1][j] == 0)
			{
				MapData[i][j] = 2;
				MapExitX = i;
				MapExitY = j;
				MakeExit = true;
				//break;
			}
		}
		//空間数が一定以下なら再生成
	} while (Space < 70);

	//孤立した空間を埋める
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (CheckData[i][j] == 0)MapData[i][j] = 5;
		}
	}
	MapData[player.GetY() / 160][player.GetX() / 160] = 3;
}

int GameMainScene::CheckSpace(int y, int x, int* cnt)
{
		
		//対象ブロックが外枠なら処理を抜ける
		if (x == 0 || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1 || y == 0)return 0;
	
		CheckData[y][x] = 1;
		(*cnt)++;
	
		if (MapData[y + 1][x] && !CheckData[y + 1][x])CheckSpace(y + 1, x, cnt);
		else CheckData[y + 1][x] = 1;

		if (MapData[y - 1][x] && !CheckData[y - 1][x])CheckSpace(y - 1, x, cnt);
		else CheckData[y - 1][x] = 1;

		if (MapData[y][x + 1] && !CheckData[y][x + 1])CheckSpace(y, x + 1, cnt);
		else CheckData[y][x + 1] = 1;

		if (MapData[y][x - 1] && !CheckData[y][x - 1])CheckSpace(y, x - 1, cnt);
		else CheckData[y][x - 1] = 1;
	
		return 0;
}

void GameMainScene::NextMap() {
	AnimTimer++;
	MoveStop_flg = false;
	
	if (0 <= Bright && Anim_flg == false) {
		// フェードアウト処理
		if (AnimTimer % 5 == 0) {
			// 描画輝度をセット
			SetDrawBright(Bright, Bright, Bright);
			Bright -= Bright_minus;
		}
		if (Bright <= 0)MakeMap_flg = true;
	}
	else {
		if (AnimTimer % 5 == 0) {
			// 描画輝度をセット
			SetDrawBright(Bright, Bright, Bright);
			Bright += Bright_minus;
			Anim_flg = true;
			if (Bright >= 255) {
				Exit_flg = false;
				Anim_flg = false;
				MoveStop_flg = true;
				AnimTimer = 0, Bright = 255;
			}
		}
	}
		//次のマップを生成する処理
	if (MakeMap_flg == true) {
		MapExitX = 0;
		MapExitY = 0;
		player.Spawn();
		MakeMap();
		player.SetMapData(MapData);

		enemy[0] = nullptr;
		enemy[0] = new Slime();
		enemy[1] = nullptr;
		enemy[1] = new Slime();
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)enemy[i]->SetMapData(MapData);
		}

		//enemy2.SetMapData(MapData);
		treasurebox.SetMapData(MapData);

		MakeMap_flg = false;
		count = 0;
	}
}

void GameMainScene::ExitCheck() {
	if (MapExitY * 160 + 100 > player.GetX() && MapExitY * 160 + 60 < player.GetX() && player.GetY() == MapExitX * 160 + 131) {
		//Yボタン長押しで処理に入る
		if (PAD_INPUT::OnPressed(XINPUT_BUTTON_Y)) {
			count++;
			MoveStop_flg = false;
			if (count >= 75) {
				for (int i = 0; i < ENEMY_MAX; i++)
				{
					if (enemy[i] != nullptr)break;
					if (i == 9)Exit_flg = true;
				}
			}
		}
		if (PAD_INPUT::OnRelease(XINPUT_BUTTON_Y))count = 0, MoveStop_flg = true;
	}
}

void GameMainScene::SearchEnemy() 
{
	//近くの敵のナンバーと距離
	int NearEnemy = -1;
	int NearDistance = -1;

	//プレイヤーと敵の座標
	int PlayerX = player.GetX();
	int PlayerY = player.GetY();
	int EnemyX = 0, EnemyY = 0;

	for (int i = 0; i < 10; i++)
	{
		if (enemy[i] != nullptr) 
		{
			EnemyX = enemy[i]->E_GetX();
			EnemyY = enemy[i]->E_GetY();

			int Dis = sqrt(pow(PlayerX - EnemyX, 2) + pow(PlayerY - EnemyY, 2));

			if (NearDistance < 0 || Dis < NearDistance)
			{
				NearEnemy = i;
				NearDistance = Dis;
			}
		}
	}

	if (0 <= NearEnemy) player.SetNear(enemy[NearEnemy]->E_GetX(), enemy[NearEnemy]->E_GetY(),NearDistance);
	else player.SetNear(-1, -1, -1);
}