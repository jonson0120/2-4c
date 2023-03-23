#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"
#include "GameMainScene.h"

#define MAX_SPEED 3
#define MIN_SPEED -3

Enemy::Enemy()
{
	image = 0;

	DropItem_Image = LoadGraph("shard.png", TRUE);

	enex = 0;
	eney = 0;

	MapData[eney][enex];

	Width = 64;
	Height = 64;

	Enemy_Damage = 1;
	Player_Damage = 1;
	Enemy_Hp = 10;
	Player_Hp = 10;

	MakeEnemy = FALSE;

	direction = 0;

	E_AttackFlg = FALSE;

	HitCool = 0;

	speed = 0;
	fall = 12;
	jump = 0;

	LoadDivGraph("images/Enemy.png", 5, 5, 1, 64, 64, EImages);
}

void Enemy::Update(Player* player)
{
	//落下とジャンプ
	float fallinit = 12;

	if (fall < fallinit)
	{
		fall += (fallinit * 2) / 45;
		if (fall > fallinit)
		{
			fall = fallinit;
		}
	}
	eney += fall;

	while ((!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]))
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


	//プレイヤー認識範囲
	if (enex + BLOCK_SIZE * 1.5 >= player->GetX() && enex - BLOCK_SIZE * 1.5 <= player->GetX() &&
		eney + BLOCK_SIZE >= player->GetY() && eney - BLOCK_SIZE <= player->GetY())
	{
		//プレイヤー追尾
		if (enex >= player->GetX() && MIN_SPEED != speed)
		{
			--speed;
		}

		if (enex <= player->GetX() && MAX_SPEED != speed)
		{
			++speed;
		}

		if (eney >= player->GetY() && fall <= fallinit && jump == 0 && MAX_SPEED != jump)
		{
			fall *= -1;
			jump++;
		}

		if (enex == player->GetX())
		{
			speed = 0;
		}
		enex += speed;
	}

	//壁に当たった時止める
	while (!MapData[eney / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
	{
		enex--;
		speed = 0;
	}

	while (!MapData[eney / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
	{
		enex++;
		speed = 0;
	}

	//プレイヤーに当たった時攻撃
	//if (enex == player->GetX() && eney == player->GetY())
	//{
	//	player->HitEnemy(float damage);
	//}

	if (HitCool)HitCool--;
}

void Enemy::makeEnemy()
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

void Enemy::Draw(int x,int y) const
{
	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		//敵の表示
		DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[0], TRUE);
	}

	if (Enemy_Hp == 0)
	{
		DeleteGraph(EImages[0]);

		DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), DropItem_Image, TRUE);
	}

	DrawFormatString(100, 100, 0xffffff, "%.1f", fall);

	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
	//		  enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
}

void Enemy::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
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
bool Enemy::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//プレイヤーからの攻撃
void Enemy::HitPlayer(float damage) { 
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}