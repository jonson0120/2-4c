#include"DxLib.h"
#include"TestMap.h"
#include"KeyManager.h"
#include"TitleScene.h"
#include"AbstractScene.h"
#include"UI.h"

#include<math.h>
#include<stdlib.h>

#include"GameOver.h"
#include"Slime.h"
#include "Grim_Reaper.h"
#include"Bat.h"
#include "DeepSlime.h"
#include "Weapon.h"

TestMap::TestMap()
{
	enemy = new Enemy * [ENEMY_MAX];
	item = new Item * [ITEM_MAX];

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i] = nullptr;
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		item[i] = nullptr;
	}
	item[0] = new Weapon(weapons::dagger, { 240,1600 });
	item[1] = new Weapon(weapons::mace, { 320,1600 });
	item[2] = new Weapon(weapons::spear, { 400, 1600 });
	item[3] = new Weapon(weapons::katana, { 480,1600 });

	MapExitX = 0;
	MapExitY = 0;

	MapType = 0;
	MakeMap();
	player.SetMapData(MapData);
	player.SetY(BLOCK_SIZE * 9.5);

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

	LoadDivGraph("images/Block.png", 4, 4, 1, 160, 160, MapImg);

	time = 0;

	count = 0;

	CameraX = 0;
	CameraY = 0;

	Bright = 255;
	Bright_minus = 10;
	AnimTimer = 0;

	LoadDivGraph("images/Art.png", 8, 8, 1, 64, 64, Art);
	LoadDivGraph("images/Info.png", 3, 3, 1, 128,128, info);

	Exit_flg = false;
	Anim_flg = false;
	MakeMap_flg = false;
	MoveStop_flg = true;
	Pressed_flg = false;
}

AbstractScene* TestMap::Update()
{
	if (player.GetLife() <= 0)
	{
		player.Reset();
	}


	if (MoveStop_flg == true)
	{
		if (player.WaitSearch())SearchEnemy();
		player.Update();
		ui.Update(&player);
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->Update(&player);
			if (enemy[i]->EnemyAttack(player.GetX(), player.GetY()))player.HitEnemy(enemy[i]->GetPower(), enemy[i]->E_GetX());
		}
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)
		{
			item[i]->Update(&player);

			if (item[i]->GetType() == Equip)
			{
				bool Second;
				if (player.Secondary() == weapons::NONE)Second = true;
				else Second = false;

				if (Second && item[i]->GetGet())item[i] = nullptr;

				if (item[i] == nullptr || item[i]->GetGet())break;
			}

			if (item[i]->GetType() == ItemType::Sh)
			{
			}
		}
	}
	//enemy2.Update(&player);
	CameraX = player.GetX();
	CameraY = player.GetY();

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
		if (i == ENEMY_MAX - 1)MapData[MapExitX][MapExitY] = 3, Pressed_flg = true;
	}
	/*if (player.GetX() / 160 == MapExitY && player.GetY() / 160 == MapExitX) Exit_flg = true;*/

	ExitCheck();
	if (Exit_flg == true) NextMap();
	x = MapExitY * 160 + 80;
	y = MapExitX * 160 + 131;
	time++;

	if (PAD_INPUT::OnClick(XINPUT_BUTTON_Y)) 
	{
		if (player.GetY() / BLOCK_SIZE == 8 && player.GetX() / BLOCK_SIZE == 1) 
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				enemy[i] = nullptr;
			}
			MapType = 0;
			MakeMap();
			player.SetMapData(MapData);
		}

		if (player.GetY() / BLOCK_SIZE == 8 && player.GetX() / BLOCK_SIZE == 2)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				enemy[i] = nullptr;
			}
			MapType = 1;
			MakeMap();
			player.SetMapData(MapData);
		}

		if (player.GetY() / BLOCK_SIZE == 8 && player.GetX() / BLOCK_SIZE == 3)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				enemy[i] = nullptr;
			}
			MapType = 2;
			MakeMap();
			player.SetMapData(MapData);
		}

		if (player.GetY() / BLOCK_SIZE == 6 && player.GetX() / BLOCK_SIZE == 3)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				enemy[i] = nullptr;
			}
		}

		if (player.GetY() / BLOCK_SIZE == 6 && player.GetX() / BLOCK_SIZE == 2)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				if (enemy[i] == nullptr) 
				{
					enemy[i] = new Slime(0);
					enemy[i]->SetMapData(MapData);
					break;
				}
			}
		}

		if (player.GetY() / BLOCK_SIZE == 6 && player.GetX() / BLOCK_SIZE == 1)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				if (enemy[i] == nullptr)
				{
					enemy[i] = new Bat(0);
					enemy[i]->SetMapData(MapData);
					break;
				}
			}
		}

		if (player.GetY() / BLOCK_SIZE == 4 && player.GetX() / BLOCK_SIZE == 2)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				if (enemy[i] == nullptr)
				{
					enemy[i] = new Grim_Reaper(0);
					enemy[i]->SetMapData(MapData);
					break;
				}
			}
		}

		if (player.GetY() / BLOCK_SIZE == 4 && player.GetX() / BLOCK_SIZE == 3)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				if (enemy[i] == nullptr)
				{
					enemy[i] = new DeepSlime(0);
					enemy[i]->SetMapData(MapData);
					break;
				}
			}
		}

		if (player.GetY() / BLOCK_SIZE == 1 && player.GetX() / BLOCK_SIZE == 1)
		{
			for (int i = 0; i < ENEMY_MAX; i++)
			{
				for (int i = 0; i < ENEMY_MAX; i++)
				{
					enemy[i] = nullptr;
				}
				return new Title();
			}
		}
	}

	SortEnemy();
	SortItem();

	return this;
}

void TestMap::SortEnemy()
{
	for (int i = 0; i < ENEMY_MAX - 1; i++)
	{
		if (enemy[i] == nullptr)
		{
			enemy[i] = enemy[i + 1];
			enemy[i + 1] = nullptr;
		}
	}
}

void TestMap::SortItem()
{
	for (int i = 0; i < ITEM_MAX - 1; i++)
	{
		if (item[i] == nullptr)
		{
			item[i] = item[i + 1];
			item[i + 1] = nullptr;
		}
	}
}

void TestMap::Draw() const
{

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (MapData[i][j] < 4)DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY(), MapImg[MapData[i][j]], TRUE);
		}
	}

	DrawRotaGraph(BLOCK_SIZE * 3.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 7.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, info[0], true);
	DrawRotaGraph(BLOCK_SIZE * 1.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 7.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, info[1], true);
	DrawRotaGraph(BLOCK_SIZE * 1.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 1.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, info[2], true);

	DrawRotaGraph(BLOCK_SIZE * 1.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 8.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[0], true);
	DrawRotaGraph(BLOCK_SIZE * 2.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 8.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[1], true);
	DrawRotaGraph(BLOCK_SIZE * 3.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 8.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[2], true);
	DrawRotaGraph(BLOCK_SIZE * 3.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 6.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[3], true);
	DrawRotaGraph(BLOCK_SIZE * 2.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 6.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[4], true);
	DrawRotaGraph(BLOCK_SIZE * 1.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 6.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[6], true);
	DrawRotaGraph(BLOCK_SIZE * 2.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 4.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[5], true);
	DrawRotaGraph(BLOCK_SIZE * 3.5 - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * 4.5 - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Art[7], true);

	//DrawFormatString(0, 500, 0xff0000, "%d", Space);

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)item[i]->Draw({ player.GetX() ,player.GetY() });
	}

	player.Draw();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr)enemy[i]->Draw(player.GetX(), player.GetY());
	}

	//enemy2.Draw(player.GetX(), player.GetY());

	ui.Draw();

	//DrawFormatString(0, 500, 0xff0000, "%d", AnimTimer);
	//DrawFormatString(0, 550, 0xff0000, "%d", Bright);
	//DrawFormatString(0, 600, 0xff0000, "%d",CameraX);
	//DrawFormatString(50, 600, 0xff0000, "%d", CameraY);
	//DrawFormatString(0, 650, 0xff0000, "%d", x);
	//DrawFormatString(50, 650, 0xff0000, "%d", y);
	//DrawCircle(160 * (4 + MapExitY) + 80 - player.GetX(), 360 + 160 * MapExitX + 120 - player.GetY(), 4, 0xff0000, TRUE);
	//DrawFormatString(500, 200, 0xffffff, "%d", hit);
	//DrawFormatString(0, 700, 0xff0000, "%d", count);
}

//マップ生成
void TestMap::MakeMap()
{
	//壁・空間パターン

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


		//マップ端の壁・天井-----------------------
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (j == MAP_WIDTH - 1 || j == 0)
				{
					MapData[i][j] = 0;
				}
				else MapData[i][j] = 1;

				if (i == MAP_HEIGHT - 1 || i == 0)
				{
					MapData[i][j] = 0;
				}

				if (i == 7 && j < 4) MapData[i][j] = 0;
				if (i == 5 && 1 < j && j < 5) MapData[i][j] = 0;

				if (MapType == 1) 
				{
					if (i == 9 && 4 < j && j < 8) MapData[i][j] = 0;
					if (i == 8 && 8 < j) MapData[i][j] = 0;
				}

				if (MapType == 2)
				{
					if (i == 5 && 9 < j) MapData[i][j] = 0;
					if (i == 6 && 8 < j) MapData[i][j] = 0;
					if (i == 7 && 7 < j) MapData[i][j] = 0;
					if (i == 8 && 6 < j) MapData[i][j] = 0;
					if (i == 9 && 5 < j) MapData[i][j] = 0;
				}
			}
		}
		//------------------------------------------

		//プレイヤーの初期位置を足場のある空間にする

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


	//孤立した空間を埋める
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (CheckData[i][j] == 0)MapData[i][j] = 5;
		}
	}
}

int TestMap::CheckSpace(int y, int x, int* cnt)
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

void TestMap::NextMap() {
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
		enemy[1] = nullptr;
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)enemy[i]->SetMapData(MapData);
		}

		for (int i = 0; i < ITEM_MAX; i++)
		{
			item[i] = nullptr;
		}

		MakeMap_flg = false;
		Pressed_flg = false;
		count = 0;
	}
}

void TestMap::ExitCheck() {
	if (MapExitY * 160 + 100 > player.GetX() && MapExitY * 160 + 60 < player.GetX() && player.GetY() == MapExitX * 160 + 131) {
		if (Pressed_flg == true) {
			//Yボタン長押しで処理に入る
			if (PAD_INPUT::OnPressed(XINPUT_BUTTON_Y)) {
				count++;
				if (count >= 90) {
					for (int i = 0; i < ENEMY_MAX; i++)
					{
						if (enemy[i] != nullptr)break;
						if (i == 9)Exit_flg = true;
					}
				}
			}
			if (PAD_INPUT::OnRelease(XINPUT_BUTTON_Y))count = 0;
		}
	}
}

void TestMap::SearchEnemy()
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

	if (0 <= NearEnemy) player.SetNear(enemy[NearEnemy]->E_GetX(), enemy[NearEnemy]->E_GetY(), NearDistance);
	else player.SetNear(-1, -1, -1);
}