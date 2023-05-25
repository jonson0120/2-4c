#include "DxLib.h"
#include "Enemy2.h"
#include "Enemy.h"
#include "Player.h"
#include "GameMainScene.h"
#include "common.h"

#define MAX_SPEED 4
#define MIN_SPEED -4
//
//Enemy2::Enemy2()
//{
//	//image = 0;
//
//	ene2x = 0;
//	ene2y = 0;
//
//	Width = 64;
//	Height = 64;
//
//	Enemy2_Damage = 1;
//	Player_Damage = 1;
//	Enemy2_Hp = 1;
//	Player_Hp = 10;
//
//	MakeEnemy2 = FALSE;
//
//	direction = 0;
//
//	E2_AttackFlg = FALSE;
//
//	speed = 0;
//	fall = 14;
//	jump = 0;
//
//	image = LoadGraph("flyenemy.png", TRUE);
//	//LoadDivGraph("images/Enemy2.png", 5, 5, 1, 64, 64, E2Images);
//}
//
//void Enemy2::Update(Player* player)
//{
//	//�����ƃW�����v
//	float fallinit = 14;
//	ene2y += fall;
//	while (!MapData[(ene2y + Height / 2) / BLOCK_SIZE][(ene2x - Width / 2) / BLOCK_SIZE])
//	{
//		ene2y--;
//		jump = 0;
//	}
//
//	if (fall < fallinit)
//	{
//		fall += (fallinit * 2) / 45;
//		if (fall > fallinit)
//		{
//			fall = fallinit;
//		}
//	}
//
//	//�ǂɓ����������~�߂�
//	while (!MapData[ene2y / BLOCK_SIZE][(ene2x + Width / 2) / BLOCK_SIZE])
//	{
//		if (MIN_SPEED != ene2x)
//		{
//			ene2x--;
//			speed = 0;
//		}
//	}
//
//	while (!MapData[ene2y / BLOCK_SIZE][(ene2x - Width / 2) / BLOCK_SIZE])
//	{
//		if (MIN_SPEED != ene2x)
//		{
//			ene2x++;
//			speed = 0;
//		}
//	}
//
//	//�v���C���[�F���͈�
//	if (ene2x + BLOCK_SIZE * 1.5 >= player->GetX() && ene2x - BLOCK_SIZE * 1.5 <= player->GetX() &&
//		ene2y + BLOCK_SIZE >= player->GetY() && ene2y - BLOCK_SIZE <= player->GetY())
//	{
//		//�v���C���[�ǔ�
//		if (ene2x >= player->GetX() && MIN_SPEED != speed)
//		{
//			--speed;
//		}
//
//		if (ene2x <= player->GetX() && MAX_SPEED != speed)
//		{
//			++speed;
//		}
//
//		if (ene2y >= player->GetY() && fall <= fallinit && jump == 0 && MAX_SPEED != jump)
//		{
//			fall *= -1;
//			jump++;
//		}
//
//		if (ene2x == player->GetX())
//		{
//			speed = 0;
//		}
//		ene2x += speed;
//	}
//
//	//�v���C���[�ɓ����������U��
//	//if (enex == player->GetX() && eney == player->GetY())
//	//{
//	//	player->HitEnemy(float damage);
//	//}
//}
//
//void Enemy2::makeEnemy2()
//{
//	while (MakeEnemy2 == FALSE)
//	{
//		int i = rand() % 11;
//		int j = rand() % 14;
//		if (MapData[i][j] == 1 && MapData[i + 1][j] == 0)
//		{
//			MapData[i][j] = 2;
//			ene2x = i * BLOCK_SIZE + BLOCK_SIZE / 2;
//			ene2y = j * BLOCK_SIZE + BLOCK_SIZE / 2;
//			MakeEnemy2 = TRUE;
//		}
//	}
//}
//
//void Enemy2::Draw(int x, int y) const
//{
//	//if (MakeEnemy2 == TRUE)
//	//{
//	//	//�G�̕\��
//	//	DrawRotaGraph(ene2x - x + (SCREEN_WIDTH / 2), ene2y - y + (SCREEN_HEIGHT / 2), 1.0, 0, E2Images[0], TRUE);
//	//}
//
//	if (MakeEnemy2 == TRUE)
//	{
//		//�G�̕\��
//		DrawRotaGraph(ene2x - x + (SCREEN_WIDTH / 2), ene2y - y + (SCREEN_HEIGHT / 2), 1.0, 0, image, TRUE);
//	}
//
//
//	DrawFormatString(100, 100, 0xffffff, "%.1f", fall);
//
//	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
//	//		  enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
//}
//
//void Enemy2::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
//{
//	for (int i = 0; i < MAP_HEIGHT; i++)
//	{
//		for (int j = 0; j < MAP_WIDTH; j++)
//		{
//			this->MapData[i][j] = MapData[i][j];
//		}
//	}
//
//	makeEnemy2();
//}