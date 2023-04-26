#include"DxLib.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"
#include"UI.h"

#include<math.h>
#include<stdlib.h>

#include"TreasureBox.h"

#include"GameOver.h"
#include"Slime.h"
#include"Bat.h"
#include "Grim_Reaper.h"
#include "DeepSlime.h"
#include"Bomber.h"

#include"Item.h"
#include"Weapon.h"
#include"Shard.h"

#define DEBUG

GameMainScene::GameMainScene()
{
	enemy = new Enemy * [ENEMY_MAX];
	item = new Item * [ITEM_MAX];
	treasurebox = new TreasureBox * [TREASURE_MAX];

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i] = nullptr;
	}
	enemy[0] = new Slime();

	for (int i = 0; i < ENEMY_MAX+1; i++)
	{
		Damage[i] = { 0,0,0,0 };
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		item[i] = nullptr;
	}

	Level = 1;
	SafeZone = false;

	MaplimitX = 7;
	MaplimitY = 5;

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
			item[i]->SetItem();
		}
	}
	//enemy2.SetMapData(MapData);
	for (int i = 0; i < TREASURE_MAX; i++)
	{
		treasurebox[i] = nullptr;
	}

	treasurebox[0] = new TreasureBox();
	treasurebox[0]->SetMapData(MapData);
	
	LoadDivGraph("images/Block02.png", 4, 4, 1, 160, 160, MapImg);

	time = 0;


	LoadDivGraph("images/number.png", 44, 11, 4, 10, 16, hierarchy_font);

	count = 0;

	CameraX = 0;
	CameraY = 0;

	Bright = 0;
	Bright_minus = 10;
	AnimTimer = 0;

	LoadDivGraph("images/Gauge.png", 2, 2, 1, 34, 34, DoorIcon);
	DoorIcon[2] = LoadGraph("images/DoorIcon.png");
	DoorIcon[3] = LoadGraph("images/ItemIcon.png");

	Exit_flg = true;
	Anim_flg = true;
	MakeMap_flg = false;
	MoveStop_flg = true;
	Pressed_flg = false;

	SetDrawBright(Bright, Bright, Bright);

}

AbstractScene* GameMainScene::Update() 
{
	if (player.GetLife() <= 0)
	{
		return new GameOver();
	}
	

	if ((!MoveStop_flg || Bright == 0) && !UpGrade)
	{
		if (player.WaitSearch())SearchEnemy();
		player.Update();
	}

	if (!UpGrade && SafeZone && player.GetX() / BLOCK_SIZE == 6 && PAD_INPUT::OnClick(XINPUT_BUTTON_Y))
	{
		player.Reset();
		UpGrade = true;
	}

	if (UpGrade) 
	{
		if (!ui.UpGradeUI(&player))UpGrade = false;
	}

	ui.Update(&player);


	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr && !MoveStop_flg)
		{
			enemy[i]->Update(&player);
			if (enemy[i]->EnemyAttack(player.GetX(), player.GetY()) && !player.GetCool())
			{
				player.HitEnemy(enemy[i]->GetPower(), enemy[i]->E_GetX());
				Damage[0] = { player.GetX(),player.GetY(),enemy[i]->GetPower(),30 };
			}
		}
	}

	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)
		{
			if (item[i]->GetType() == Equip)
			{
				bool Second;
				if (player.Secondary() == weapons::NONE)Second = true;
				else Second = false;

				item[i]->Update(&player);

				if (Second && item[i]->GetGet())item[i] = nullptr;

				if (item[i] == nullptr || item[i]->GetGet())break;
			}

			if (item[i]->GetType() == ItemType::Sh)
			{
				item[i]->Update(&player);
				if (item[i]->GetGet())item[i] = nullptr;
			}
		}
	}
	//enemy2.Update(&player);
	CameraX = player.GetX();
	CameraY = player.GetY();

	for (int i = 0; i < TREASURE_MAX; i++)
	{
		if (treasurebox[i] != nullptr)
		{
			treasurebox[i]->Update(&player);
			if (treasurebox[i]->DropItem())
			{
				weapons drop;
				switch (GetRand(2))
				{
				case 0:
					drop = weapons::mace;
					break;
				case 1:
					drop = weapons::spear;
					break;
				case 2:
					drop = weapons::katana;
					break;
				default:
					drop = weapons::mace;
					break;
				}

				for (int i = 0; i < ITEM_MAX; i++)
				{
					if (item[i] == nullptr)
					{
						item[i] = new Weapon(drop, { treasurebox[i]->Box_GetX(), treasurebox[i]->Box_GetY() });
						item[i]->SetMapData(MapData);
						break;
					}
				}
			}
		}
	}

	int DMG = 0;
	switch (player.GetEquip())
	{
	case weapons::dagger:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitDagger(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()) && !enemy[i]->GetCool()) {
				DMG = player.GetAtk() * player.GetPower();
				enemy[i]->HitPlayer(DMG);
				Damage[i + 1] = { enemy[i]->E_GetX(),enemy[i]->E_GetY(),DMG,30 };
			}
		}
		break;
	case weapons::mace:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitMace(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()) && !enemy[i]->GetCool()) {
				DMG = player.GetAtk() * player.GetPower();
				enemy[i]->HitPlayer(DMG);
				Damage[i + 1] = { enemy[i]->E_GetX(),enemy[i]->E_GetY(),DMG,30 };
			}
		}
		break;
	case weapons::spear:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitSpear(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()) && !enemy[i]->GetCool()) {
				DMG = player.GetAtk() * player.GetPower();
				enemy[i]->HitPlayer(DMG);
				Damage[i + 1] = { enemy[i]->E_GetX(),enemy[i]->E_GetY(),DMG,30 };
			}
		}
		break;
	case weapons::katana:
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)if (player.HitKatana(enemy[i]->E_GetX(), enemy[i]->E_GetY(),
				enemy[i]->GetWidth(), enemy[i]->GetHeight()) && !enemy[i]->GetCool()) {
				DMG = player.GetAtk() * player.GetPower();
				enemy[i]->HitPlayer(DMG);
				Damage[i + 1] = { enemy[i]->E_GetX(),enemy[i]->E_GetY(),DMG,30 };
			}
		}
		break;
	default:
		break;
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr) 
		{
			if (enemy[i]->CheckHp())
			{
				int Dlop = 0;
				for (int j = 0; j < ITEM_MAX - 1 && Dlop < 2; j++)
				{
					if (item[j] == nullptr)
					{
						item[j] = new Shard({ enemy[i]->E_GetX(),enemy[i]->E_GetY() });
						item[j]->SetMapData(MapData);
						Dlop++;
					}
				}
				enemy[i] = nullptr;
				break;
			}
		}
	}

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr)break;
		if (i == ENEMY_MAX - 1)MapData[MapExitX][MapExitY] = 3, Pressed_flg = true;
	}
	/*if (player.GetX() / 160 == MapExitY && player.GetY() / 160 == MapExitX) Exit_flg = true;*/


	ExitCheck();
	if (Exit_flg == true)
	{
		MoveStop_flg = true;
		NextMap();
	}
	else
	{
		MoveStop_flg = false;
	}

	for (int i = 0; i < ENEMY_MAX + 1; i++)
	{
		if (--Damage[i].NumB < 0)Damage[i].NumB = 0;
	}


	x= MapExitY * 160 + 80;
	y= MapExitX * 160 + 131;
	time++;

	SortEnemy();
	SortItem();

#ifdef DEBUG

	if (CheckHitKey(KEY_INPUT_Q))
	{
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)
			{
				enemy[i] = nullptr;
			}
		}
	}

	if (CheckHitKey(KEY_INPUT_W))
	{
		player.SetX(MapExitY * 160 + 80);
		player.SetY(MapExitX * 160 + 131);
	}

	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B) && CheckHitKey(KEY_INPUT_E) && !SafeZone)
	{
		Level++;
		if (Level % 5 == 0 && !SafeZone)
		{
			if (MaplimitX - 2 == MaplimitY)MaplimitY++;
			else MaplimitX++;
		}
	}

	if ((PAD_INPUT::OnClick(XINPUT_BUTTON_B) && CheckHitKey(KEY_INPUT_A))|| CheckHitKey(KEY_INPUT_S))
	{
		player.AddShard();
	}

#endif // DEBUG

	return this;
}

void GameMainScene::SortEnemy()
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

void GameMainScene::SortItem()
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

void GameMainScene::Draw() const
{

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (MapData[i][j] < 4)DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY(), MapImg[MapData[i][j]], TRUE);
		}
	}
	
	//DrawFormatString(0, 500, 0xff0000, "%d", Space);

	for (int i = 0; i < TREASURE_MAX; i++)
	{
		if (treasurebox[i] != nullptr)treasurebox[i]->Draw(player.GetX(), player.GetY());
	}


	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (item[i] != nullptr)item[i]->Draw({ player.GetX() ,player.GetY() });
	}

	player.Draw();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i] != nullptr)enemy[i]->Draw(player.GetX(), player.GetY());
	}

	if (MapExitY * 160 + 100 > player.GetX() && MapExitY * 160 + 60 < player.GetX() && player.GetY() == MapExitX * 160 + 131 && !Exit_flg) {

		int DoorX = 160 * (4 + MapExitY) + 80 - player.GetX();
		int DoorY = 360 + 160 * MapExitX + 120 - player.GetY() - BLOCK_SIZE * 0.7;

		DrawRotaGraph(DoorX, DoorY, 1.2, 0, DoorIcon[0], true);
		DrawCircleGauge(DoorX, DoorY, 100 * (count / 60.f), DoorIcon[1], 0, 1.2, false, false);
		DrawRotaGraph2(DoorX, DoorY, 18, 16, 1, 0, DoorIcon[2], true);
	}

	if (!UpGrade && SafeZone && player.GetX() / BLOCK_SIZE == 6) 
	{
		DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 , 1, 0, DoorIcon[3], true);
	}
	//enemy2.Draw(player.GetX(), player.GetY());

	for (int i = 0; i < ENEMY_MAX + 1; i++)
	{
		if (0 < Damage[i].NumB) DrawDamage(Damage[i], i);
	}

	ui.Draw();
	if (UpGrade)ui.UpGradeDraw();

	DrawRotaGraph(1200, 60, 6.0, 0, hierarchy_font[Level % 10], TRUE);
	DrawRotaGraph(1140, 60, 6.0, 0, hierarchy_font[Level / 10 % 10], TRUE);

#ifdef DEBUG

	DrawFormatString(0, 500, 0xff0000, "%d", Level);
	DrawFormatString(0, 510, 0xff0000, "%d", SafeZone);

#endif // DEBUG
	//DrawFormatString(0, 550, 0xff0000, "%d", Bright);
	//DrawFormatString(0, 600, 0xff0000, "%d",CameraX);
	//DrawFormatString(50, 600, 0xff0000, "%d", CameraY);
	//DrawFormatString(0, 650, 0xff0000, "%d", x);
	//DrawFormatString(50, 650, 0xff0000, "%d", y);
	//DrawCircle(160 * (4 + MapExitY) + 80 - player.GetX(), 360 + 160 * MapExitX + 120 - player.GetY(), 4, 0xff0000, TRUE);
	//DrawFormatString(500, 200, 0xffffff, "%d", hit);
	//DrawFormatString(0, 700, 0xff0000, "%d", count);
}

void GameMainScene::DrawDamage(LocNum LocDmg,int num) const
{
	int X = LocDmg.X;
	int Y = LocDmg.Y;
	int Dmg = LocDmg.NumA;

	int Dis = 7;
	float Size = 1.2;
	if (num != 0)
	{
		if (LocDmg.NumA >= 100)
		{
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) - (11 * Size + Dis), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg / 100 % 10], true);
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg / 10 % 10], true);
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) + (11 * Size + Dis), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg % 10], true);
		}
		else if (LocDmg.NumA >= 10)
		{
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) - Dis, Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg / 10 % 10], true);
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) + Dis, Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg % 10], true);
		}
		else
		{
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg], true);
		}
	}
	else
	{
		if (LocDmg.NumA >= 100)
		{
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) - (11 * Size + Dis), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg / 100 % 10 + 33], true);
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg / 10 % 10 + 33], true);
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) + (11 * Size + Dis), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg % 10 + 33], true);
		}
		else if (LocDmg.NumA >= 10)
		{
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) - Dis, Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg / 10 % 10 + 33], true);
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2) + Dis, Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg % 10 + 33], true);
		}
		else
		{
			DrawRotaGraph(X - player.GetX() + (SCREEN_WIDTH / 2), Y - player.GetY() + (SCREEN_HEIGHT / 2) - 50 + LocDmg.NumB, Size, 0, hierarchy_font[Dmg + 33], true);
		}
	}
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
	if (!SafeZone)
	{
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
					if (j == MAP_WIDTH - 1 || j == 0 || MaplimitX <= j)
					{
						MapData[i][j] = 0;
					}
					//else MapData[i][j] = 1;

					if (i == MAP_HEIGHT - 1 || i == 0 || MaplimitY <= i)
					{
						MapData[i][j] = 0;
					}
				}
			}
			//------------------------------------------

			int SetY = BLOCK_SIZE * (GetRand(MAP_HEIGHT - 3) + 1) + BLOCK_SIZE - player.GetHeight() / 2 - 1;
			player.SetY(SetY);

			//プレイヤーの初期位置を足場のある空間にする
			MapData[player.GetY() / 160][player.GetX() / 160] = 1;
			MapData[player.GetY() / 160][(player.GetX() / 160) + 1] = 1;
			MapData[player.GetY() / 160 + 1][player.GetX() / 160] = 0;

			//空間数チェック
			CheckSpace(player.GetY() / 160, player.GetX() / 160, &Space);

			//空間数が一定以下なら再生成
		} while ((float)Space < (float)(MaplimitX * MaplimitY) * 0.6);

		//孤立した空間を埋める
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (CheckData[i][j] == 0)MapData[i][j] = 0;
			}
		}

		MakeExit = MakeExit;

		while (MakeExit == false)
		{
			int i = GetRand(MAP_HEIGHT);
			int j = GetRand(MAP_WIDTH - 3) + 2;
				if (MapData[i][j] == 1 && MapData[i + 1][j] == 0)
				{
					MapData[i][j] = 2;
					MapExitX = i;
					MapExitY = j;
					MakeExit = true;
					//break;
				}
		}

	}
	else
	{
	player.SetY(BLOCK_SIZE * 9.5);

	Space = 0;
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			CheckData[i][j] = 0;
			MapData[i][j] = 0;
		}
	}

		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (MAP_WIDTH - 3 <= j || j == 0)
				{
					MapData[i][j] = 0;
				}
				else MapData[i][j] = 1;

				if (MAP_HEIGHT - 1 <= i || i <= 5)
				{
					MapData[i][j] = 0;
				}
			}
		}

		MapData[MAP_HEIGHT - 2][MAP_WIDTH - 3] = 2;
		MapExitX = MAP_HEIGHT - 2;
		MapExitY = MAP_WIDTH - 3;

		CheckSpace(player.GetY() / 160, player.GetX() / 160, &Space);
	}

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
	
	if (0 <= Bright && Anim_flg == false) {
		// フェードアウト処理
		if (AnimTimer % 5 == 0) {
			// 描画輝度をセット
			SetDrawBright(Bright, Bright, Bright);
			Bright -= Bright_minus;
		}
		if (Bright <= 0)
		{
			MakeMap_flg = true;

			if (SafeZone)SafeZone = false;
			else if (++Level % 10 == 0)SafeZone = true;
		}
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
				AnimTimer = 0, Bright = 255;
			}
		}
	}
		//次のマップを生成する処理
	if (MakeMap_flg == true) {

		//フロアを広くする
		if (Level % 5 == 0 && !SafeZone) 
		{
			if (MaplimitX - 2 == MaplimitY)MaplimitY++;
			else MaplimitX++;
		}

		MapExitX = 0;
		MapExitY = 0;
		player.Spawn();
		MakeMap();
		player.SetMapData(MapData);

		
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)enemy[i] = nullptr;
		}

		for (int i = 0; i < ITEM_MAX; i++)
		{
			item[i] = nullptr;
		}

		for (int i = 0; i < TREASURE_MAX; i++)
		{
			treasurebox[i] = nullptr;
		}

		if (!SafeZone) {
			MakeEnemy();

			treasurebox[0] = new TreasureBox();
		}
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			if (enemy[i] != nullptr)enemy[i]->SetMapData(MapData);
		}
;
		for (int i = 0; i < TREASURE_MAX; i++)
		{
			if (treasurebox[i] != nullptr)treasurebox[i]->SetMapData(MapData);
		}


		//enemy2.SetMapData(MapData);

		MakeMap_flg = false;
		Pressed_flg = false;
		count = 0;
	}
}

//敵を出現させる
void GameMainScene::MakeEnemy() 
{
	int Spawn = 2 + (Level / 4);	//敵の出現数

	for (int i = 0; i < Spawn; i++)
	{
		enemy[i] = nullptr;
		switch (GetRand(3))
		{
		case 0:
			enemy[i] = new Slime();
			break;
		case 1:
			enemy[i] = new Bat();
			break;
		case 2:
			enemy[i] = new Grim_Reaper();
			break;
		case 3:
			enemy[i] = new DeepSlime();
			break;
		}

	}
}

void GameMainScene::ExitCheck() {
	if (MapExitY * 160 + 100 > player.GetX() && MapExitY * 160 + 60 < player.GetX() && player.GetY() == MapExitX * 160 + 131) {
		if (Pressed_flg == true) {
			//Yボタン長押しで処理に入る
			if (PAD_INPUT::OnPressed(XINPUT_BUTTON_Y)) {
				count++;
				if (count >= 60) {
					for (int i = 0; i < ENEMY_MAX; i++)
					{
						if (enemy[i] != nullptr)break;
						if (i == 9)Exit_flg = true;
					}
				}
			}
			else if (--count < 0)count = 0;
		}
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