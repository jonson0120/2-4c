#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"

#define MAX_SPEED 5
#define MIN_SPEED -5

Enemy::Enemy()
{
	image = 0;

	enex = 376; // 176 一番左下にするための座標
	eney = 1423; //1423 一番左下にするための座標

	MapData[eney][enex];

	Width = 64;
	Height = 64;

	Enemy_Damage = 1;
	Enemy_Hp = 2;
	Player_Hp = 10;

	direction = 0;

	E_AttackFlg = FALSE;

	speed = 0;
	fall = 12;
	jump = 0;

	LoadDivGraph("images/Enemy.png", 5, 16, 16, 32, 32, EImages);
}

void Enemy::Update(Player* player)
{
	//落下とジャンプ
	float fallinit = 12;
	eney += fall;
	while (!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
	{
		eney--;
		jump = 0;
	}

	if (fall < fallinit)
	{
		fall += (fallinit * 2) / 45;
		if (fall > fallinit)
		{
			fall = fallinit;
		}
	}

	//壁に当たった時止める
	while (!MapData[eney / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
	{
		if (MIN_SPEED != enex)
		{
			enex--;
			speed = 0;
		}
	}

	while (!MapData[eney / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
	{
		if (MIN_SPEED != enex)
		{
			enex++;
			speed = 0;
		}
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

	//プレイヤーに当たった時攻撃
	if (enex == player->GetX() && eney == player->GetY())
	{
		player->HitEnemy();
	}
}

void Enemy::Draw(int x,int y) const
{
	if (MapData[eney / BLOCK_SIZE][enex / BLOCK_SIZE] == 1 && MapData[eney / BLOCK_SIZE][(enex / BLOCK_SIZE) + 1] == 1 &&
		MapData[(eney / BLOCK_SIZE) + 1][enex / BLOCK_SIZE] == 0)
	{
		//敵の表示
		DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), EImages[0], FALSE);
	}

	DrawFormatString(100, 100, 0xffffff, "%.1f", fall);

	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			  //enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);

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
}