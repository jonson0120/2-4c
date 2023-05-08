#include "Enemy.h"
#include "Grim_Reaper.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"
#include "GameMainScene.h"
#include <math.h>

#define MAX_SPEED 2
#define MIN_SPEED -2

Grim_Reaper::Grim_Reaper() : Enemy()
{
	image = 0;

	DropItem_Image = LoadGraph("images/shard.png", TRUE);

	enex = 0;
	eney = 0;

	MapData[eney][enex];

	Width = 90;
	Height = 100;

	Enemy_Damage = 1;
	Player_Damage = 1;
	Enemy_Hp = 10;
	Player_Hp = 10;

	Power = 1;

	MakeEnemy = FALSE;

	direction = 0;

	E_AttackFlg = FALSE;

	Attack = 0;
	HitCool = 0;

	speed = 0;
	fall = 12;

	SickleImg = LoadGraph("images/sickle.png", TRUE);
	LoadDivGraph("images/enemysicklemen.png", 2, 2, 1, 90, 100, EImages);
	Anim = 0;
	Turnflg = false;
}

void Grim_Reaper::Update(Player* player)
{
	//プレイヤー認識範囲
	if (enex + BLOCK_SIZE >= player->GetX() && enex - BLOCK_SIZE <= player->GetX() &&
		eney + BLOCK_SIZE >= player->GetY() && eney - BLOCK_SIZE <= player->GetY() && !E_AttackFlg)
	{
		//認識範囲内にいれば攻撃開始
		E_AttackFlg = true;

		//プレイヤーの方向を向く
		if (player->GetX() < enex) Turnflg = false;
		else Turnflg = true;
	}
	else if (!E_AttackFlg) {
		//通常の移動----------
		if (!Turnflg)
		{
			enex -= 3;
			if (!MapData[eney / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
				MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		else
		{
			enex += 3;
			if (!MapData[eney / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE] ||
				MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		//-------------------
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


	int fallinit = 12;

	if (fall < fallinit)
	{
		fall += (fallinit * 2) / 45;
		if (fall > fallinit)
		{
			fall = fallinit;
		}
	}
	eney += fall;

	while ((!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + 1 - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - 1 + Width / 2) / BLOCK_SIZE]))
	{
		eney++;
		fall = 0;
	}

	while ((!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]))
	{
		eney--;
	}

	//攻撃行動
	if (E_AttackFlg)
	{
		Attack++;

		//攻撃終了
		if (180 < Attack)
		{
			//鎌を振りかぶれば攻撃終了
			E_AttackFlg = false;
			Attack = 0;
		}
	}

	//プレイヤーに当たった時攻撃
	if (enex == player->GetX() && eney == player->GetY())
	{
		player->HitEnemy(1);
	}

	if (HitCool)HitCool--;

	Anim++;
}

void Grim_Reaper::makeEnemy()
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
}

void Grim_Reaper::Draw(int x, int y) const
{
	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		int WalkAnim = Anim / 60 % 2;

		//敵の表示
		if (!E_AttackFlg)DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, Turnflg, false);
		else
		{
			if (Attack < 60) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, Turnflg, false);
		}
	}

	if (Enemy_Hp == 0)
	{
		DeleteGraph(EImages[0]);

		DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), DropItem_Image, TRUE);
	}

	if (E_AttackFlg == true && Attack >= 120)
	{
		DrawSickle(x,y);
	}
	
	//DrawFormatString(100, 100, 0xffffff, "%.1f", fall);
	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
	//		  enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
}

void Grim_Reaper::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
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
bool Grim_Reaper::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//プレイヤーからの攻撃
void Grim_Reaper::HitPlayer(float damage) {
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}

//プレイヤーへの攻撃
void Grim_Reaper::DrawSickle(int x, int y)const
{
	double stX = 0, stY = 0;	//振りかぶる前の座標
	double finX = 0, finY = 0;	//振りかぶった後の座標
	double Dis = 0;				//体の中心からの距離
	double stAng, finAng = 0;	//振りかぶる角度

	if (E_AttackFlg)
	{
		stAng = 90;
		finAng = 0;
		stX = enex;
		stY = eney;
		Dis = Width * 1.2;

		finAng = (Attack - 120) * 2;

		if (Turnflg)
		{
			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90)) - x + (SCREEN_WIDTH / 2);
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90)) - y + (SCREEN_HEIGHT / 2);
			DrawRotaGraph(finX, finY, 1.0, (3.14 / 180) * finAng + 0.5, SickleImg, true, true);
		}
		else
		{
			finX = stX + Dis * cos((3.14 / 180) * (-finAng - 90)) - x + (SCREEN_WIDTH / 2);
			finY = stY + Dis * sin((3.14 / 180) * (-finAng - 90)) - y + (SCREEN_HEIGHT / 2);
			DrawRotaGraph(finX, finY, 1.0, (3.14 / 180) * -finAng - 0.5, SickleImg, true, false);
		}
	}
}

bool Grim_Reaper::EnemyAttack(int x, int y)
{
	if (E_AttackFlg && 120 < Attack)
	{
		double finX = 0, finY = 0;
		double stAng = 90;
		double finAng = 0;
		double stX = enex;
		double stY = eney;
		double Dis = Width;

		finAng = (Attack - 120) * 2;

		if (Turnflg)
		{
			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finX = stX + Dis * cos((3.14 / 180) * (-finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (-finAng - 90));
		}

		float DisX = pow(finX - x, 2);
		float DisY = pow(finY - y, 2);
		Dis = (int)sqrt((int)(DisX + DisY));
		if (Dis < 20)return true;
		else return false;
	}
	else return false;
}