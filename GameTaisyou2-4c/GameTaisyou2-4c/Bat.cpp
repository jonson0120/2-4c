#include "Bat.h"
#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"
#include "GameMainScene.h"

#include <math.h>

#define MAX_SPEED 3
#define MIN_SPEED -3

Bat::Bat(int level) : Enemy()
{
	image = 0;

	DropItem_Image = LoadGraph("shard.png", TRUE);

	enex = 0;
	eney = 0;

	MapData[eney][enex];

	Width = 48;
	Height = 64;

	Enemy_Damage = 1;
	Player_Damage = 1;
	Enemy_Hp = 6;
	Player_Hp = 10;

	Power = 1; 
	AttackVec = 0;

	//レベルによる強化
	int Addhp = level / 4 + (level / 10 * 6);
	int addAtk = level / 7 + (level / 10);

	Enemy_Hp += Addhp;
	Power += addAtk;

	MakeEnemy = FALSE;

	direction = 0;

	E_AttackFlg = FALSE;

	HighJump = false;
	LowJump = false;
	Attack = 0;
	AttackSpeed = 0;
	Dive = 0;

	AttackCool = 0;
	HitCool = 0;

	speed = 0;
	//fall = 12;
	jump = 0;

	LoadDivGraph("images/bat3.png", 3, 3, 1, 76, 96, EImages);
	Anim = 0;
	Turnflg = false;
	if (GetRand(1))Turnflg = !Turnflg;
}

void Bat::Update(Player* player)
{
	AttackSpeed = 0;

	//プレイヤー認識範囲
	if (enex + BLOCK_SIZE*2 >= player->GetX() && enex - BLOCK_SIZE*2 <= player->GetX() &&
		eney + BLOCK_SIZE*2 >= player->GetY() && eney - BLOCK_SIZE*2 <= player->GetY() && !E_AttackFlg && !AttackCool)
	{
		//認識範囲内にいれば攻撃開始
		E_AttackFlg = true;
		Dive = 9;

		//プレイヤーの方向を向く
		if (player->GetX() < enex) Turnflg = true;
		else Turnflg = false;

	}
	else if (!E_AttackFlg && !AttackCool) {
		//通常の移動----------
		if (Turnflg)
		{
			enex -= 3;
			if (!MapData[eney / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		else
		{
			enex += 3;
			if (!MapData[eney / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		//-------------------
	}

	//攻撃行動
	if (E_AttackFlg)
	{
		Attack++;
		AttackSpeed++;
		

		//ジャンプ直前の待機
		if (Attack <= 60)
		{
			AttackVec = atan2(static_cast<float>(player->GetY()) - eney, static_cast<float>(player->GetX()) - enex);
		}
		//ジャンプ
		else if (Attack < 90)
		{
			enex += Dive * cos(AttackVec);
			while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE] ||
				!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
			{
				enex--;
				speed = 0;
			}

			while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
				!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
			{
				enex++;
				speed = 0;
			}

			eney += Dive * sin(AttackVec);
			while ((!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + 1 - Width / 2) / BLOCK_SIZE]) ||
				(!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - 1 + Width / 2) / BLOCK_SIZE]))
			{
				eney++;
			}

			while ((!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE]) ||
				(!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]))
			{
				eney--;
			}

		}
		//攻撃終了
		else {
			//速度が0になれば攻撃終了
			if (--Dive <= 0)
			{
				E_AttackFlg = false;
				AttackCool = 60;
				Dive = 0;
				Attack = 0;
			}

		}
	}


	//壁に当たった時止める
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE] ||
		!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
	{
		enex--;
		speed = 0;
	}

	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
		!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
	{
		enex++;
		speed = 0;
	}
	

	if (HitCool)HitCool--;
	if (AttackCool)AttackCool--;

	Anim++;
}

void Bat::makeEnemy()
{
	MakeEnemy = FALSE;
	while (MakeEnemy == FALSE)
	{
		int i = GetRand(11) + 2;
		int j = GetRand(9) + 1;
		if (MapData[j][i] == 1 && MapData[j + 1][i] == 0)
		{
			enex = i * BLOCK_SIZE + BLOCK_SIZE / 2;
			eney = j * BLOCK_SIZE + BLOCK_SIZE / 2;
			MakeEnemy = TRUE;
		}
	}

	enex += GetRand(80) - 40;
}

void Bat::Draw(int x, int y) const
{
	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		int WalkAnim = Anim / 18 % 2;

		//敵の表示
		if (!E_AttackFlg)
		{
			if (AttackCool)DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[0], TRUE, Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, Turnflg, false);
		}
		else
		{
			if (Attack < 60) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[1], TRUE, Turnflg, false);
			//else if (fall < 0) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[3], TRUE, Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[2], TRUE, Turnflg, false);
		}
	}

	if (Enemy_Hp == 0)
	{
		DeleteGraph(EImages[0]);

		DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), DropItem_Image, TRUE);
	}

	//DrawFormatString(100, 100, 0xffffff, "%.1f", fall);

	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
	//		  enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
}

void Bat::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}

	makeEnemy();
}

//体力確認
bool Bat::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//プレイヤーからの攻撃
void Bat::HitPlayer(float damage) {
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}

//プレイヤーへの攻撃
bool Bat::EnemyAttack(int x, int y)
{
	if (E_AttackFlg && 60 < Attack)
	{
		float DisX = pow(enex - x, 2);
		float DisY = pow(eney - y, 2);

		int Dis = (int)sqrt((int)(DisX + DisY));

		if (Dis < Width)return true;
		else return false;
	}
	else return false;
}