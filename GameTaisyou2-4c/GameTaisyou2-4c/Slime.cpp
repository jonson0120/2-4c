#include "Slime.h"
#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"
#include "GameMainScene.h"

#include <math.h>

#define MAX_SPEED 3
#define MIN_SPEED -3

Slime::Slime() : Enemy()
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
	Enemy_Hp = 5;
	Player_Hp = 10;

	Power = 1;

	MakeEnemy = FALSE;

	direction = 0;

	E_AttackFlg = FALSE;

	HighJump = false;
	Attack = 0;

	AttackCool = 0;
	HitCool = 0;

	speed = 0;
	fall = 12;
	jump = 0;

	LoadDivGraph("images/Slime.png", 5, 5, 1, 64, 64, EImages);
	Anim = 0;
	Turnflg = false;
}

void Slime::Update(Player* player)
{
	//ジャンプ強度
	float fallinit = 12;

	//プレイヤー認識範囲
	if (enex + BLOCK_SIZE >= player->GetX() && enex - BLOCK_SIZE <= player->GetX() &&
		eney + BLOCK_SIZE >= player->GetY() && eney - BLOCK_SIZE <= player->GetY() && !E_AttackFlg && !AttackCool)
	{
		//認識範囲内にいれば攻撃開始
		E_AttackFlg = true;

		//プレイヤーの方向を向く
		if (player->GetX() < enex) Turnflg = true;
		else Turnflg = false;
	}
	else if(!E_AttackFlg && !AttackCool) {
		//通常の移動----------
		if (Turnflg)
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

	//攻撃行動
	if (E_AttackFlg) 
	{
		Attack++;

		//ジャンプ直前の待機
		if (Attack == 60) 
		{
			//プレイヤーが一定以上高い位置にいると縦方向ジャンプになる
			if (BLOCK_SIZE - 30 < eney - player->GetY())
			{
				fall = -fallinit * 1.2;
				HighJump = true;
			}
			//水平方向ジャンプ
			else
			{
				fall = -fallinit * 0.5;
				HighJump = false;
			}
		}
		//ジャンプ
		else if (60 < Attack && MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
		{
			int jump = 9;

			//縦方向ジャンプ
			if (HighJump)
			{
				if (Turnflg)enex -= jump / 3;
				else enex += jump / 3;
			}
			//横方向ジャンプ
			else
			{
				if (Turnflg)enex -= jump;
				else enex += jump;
			}
		}
		//攻撃終了
		else if (60 < Attack && !MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
		{
			//ジャンプして着地すれば攻撃終了
			E_AttackFlg = false;
			AttackCool = 60;
			Attack = 0;
		}
	}

	//壁に当たった時止める
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]||
			!MapData[(eney +  Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
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

	//落下とジャンプ

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
		jump = 0;
	}

	while ((!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]))
	{
		eney--;
		jump = 0;
	}


	//プレイヤーに当たった時攻撃
	//if (enex == player->GetX() && eney == player->GetY())
	//{
	//	player->HitEnemy(float damage);
	//}

	if (HitCool)HitCool--;
	if (AttackCool)AttackCool--;

	Anim++;
}

void Slime::makeEnemy()
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

void Slime::Draw(int x,int y) const
{
	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		int WalkAnim = Anim / 18 % 2;

		//敵の表示
		if (!E_AttackFlg)
		{
			if (AttackCool)DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[2], TRUE, Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, Turnflg, false);
		}
		else 
		{
			if (Attack < 60) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[2], TRUE, Turnflg, false);
			else if (fall < 0) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[3], TRUE, Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[4], TRUE, Turnflg, false);
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

void Slime::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
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
bool Slime::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//プレイヤーからの攻撃
void Slime::HitPlayer(float damage) {
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}

//プレイヤーへの攻撃
bool Slime::EnemyAttack(int x, int y) 
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