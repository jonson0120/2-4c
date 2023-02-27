#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"

Enemy::Enemy()
{
	image = 0;

	enex = 2000; // 176 一番左下にするための座標
	eney = 1000; //1423 一番左下にするための座標

	Width = 64;
	Height = 64;

	direction = 0;

	speed = 4;
	fall = 12;
	jump = 0;

}

void Enemy::Update(Player* player)
{
	//落下とジャンプ
	float fallinit = 12;
	eney += fall;

	while (!MapData[(eney + Height / 2) / 160][(enex - Width / 2) / 160])
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

	//壁に当たったら跳ね返る
	if (!MapData[eney / 160][(enex + Width / 2) / 160])
	{
		while (!MapData[eney / 160][(enex + Width / 2) / 160])enex--;
		speed *= -1;
	}

	if (!MapData[eney / 160][(enex - Width / 2) / 160])
	{
		while (!MapData[eney / 160][(enex - Width / 2) / 160])enex++;
		speed *= -1;
	}

	//プレイヤー認識範囲
	if (enex + BLOCK_SIZE * 2 >= player->GetX() && enex - BLOCK_SIZE * 2 <= player->GetX() && eney + BLOCK_SIZE * 2 >= player->GetY())
	{
		//プレイヤー追尾
		if (enex >= player->GetX())
		{
			enex -= speed;
		}

		if (enex <= player->GetX())
		{
			enex += speed;
		}

		if (eney >= player->GetY() && fall >= fallinit && jump == 0)
		{
			fall *= -1;
			jump++;
		}
	}
}

void Enemy::Draw(int x,int y) const
{
	//敵の表示
	DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
		enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2),EnemyImage,TRUE);

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