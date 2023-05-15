#include "Boss.h"
#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"
#include "GameMainScene.h"

#include <math.h>

#define MAX_SPEED 3
#define MIN_SPEED -3

Boss::Boss(int level) : Enemy()
{
	image = 0;

	DropItem_Image = LoadGraph("shard.png", TRUE);

	//�G���W
	enex = 0;
	eney = 0;

	//�}�b�v�f�[�^
	MapData[eney][enex];

	//�G�T�C�Y
	Width = 530;
	Height = 360;

	//�G�X�e�[�^�X
	Enemy_Hp = 5;

	//�G�U����
	Power = 1;

	//���x���ɂ�鋭��
	int Addhp = level / 3 + (level / 10 * 10);
	int addAtk = level / 4 + (level / 10);

	Enemy_Hp += Addhp;
	Power += addAtk;

	MakeEnemy = FALSE;

	E_AttackFlg = FALSE;

	//�U���֘A
	HighJump = false;
	Attack = 0;

	//�U����̍d������
	AttackCool = 0;

	//��e��̖��G����
	HitCool = 0;

	//�ړ��֘A
	speed = 0;
	fall = 12;
	jump = 0;

	//�摜�֘A
	LoadDivGraph("images/Boss.png", 5, 5, 1, 530, 360, EImages);
	Anim = 0;
	Turnflg = false;
	if (GetRand(1))Turnflg = !Turnflg;
}

void Boss::Update(Player* player)
{
	//�W�����v���x
	float fallinit = 12;

	//�v���C���[�F���͈�
	if (enex + BLOCK_SIZE >= player->GetX() && enex - BLOCK_SIZE <= player->GetX() &&
		eney + BLOCK_SIZE >= player->GetY() && eney - BLOCK_SIZE <= player->GetY() && !E_AttackFlg && !AttackCool)
	{
		//�F���͈͓��ɂ���΍U���J�n
		E_AttackFlg = true;

		//�v���C���[�̕���������
		if (player->GetX() < enex) Turnflg = true;
		else Turnflg = false;
	}
	else if (!E_AttackFlg && !AttackCool) {
		//�ʏ�̈ړ�----------
		if (Turnflg)
		{
			enex -= 3;
			//�ǂɂԂ���ƌ����𔽓]������
			if (!MapData[eney / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
				MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		else
		{
			enex += 3;
			//�ǂɂԂ���ƌ����𔽓]������
			if (!MapData[eney / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE] ||
				MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		//-------------------
	}

	//�U���s��
	if (E_AttackFlg)
	{
		Attack++;

		//�W�����v���O�̑ҋ@
		if (Attack == 60)
		{
			//�v���C���[�����ȏ㍂���ʒu�ɂ���Əc�����W�����v�ɂȂ�
			if (BLOCK_SIZE - 30 < eney - player->GetY())
			{
				fall = -fallinit * 1.2;
				HighJump = true;
			}
			//���������W�����v
			else
			{
				fall = -fallinit * 0.5;
				HighJump = false;
			}
		}
		//�W�����v
		else if (60 < Attack && MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
		{
			int jump = 9;

			//�c�����W�����v
			if (HighJump)
			{
				if (Turnflg)enex -= jump / 3;
				else enex += jump / 3;
			}
			//�������W�����v
			else
			{
				if (Turnflg)enex -= jump;
				else enex += jump;
			}
		}
		//�U���I��
		else if (60 < Attack && !MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
		{
			//�W�����v���Ē��n����΍U���I��
			E_AttackFlg = false;
			AttackCool = 60;
			Attack = 0;
		}
	}

	//�ǂɂ߂荞�񂾎��ɕ␳
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE] ||
		!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
	{
		enex--;
		speed = 0;
	}

	//�ǂɂ߂荞�񂾎��ɕ␳
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
		!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
	{
		enex++;
		speed = 0;
	}

	//�����ƃW�����v

	if (fall < fallinit)
	{
		//�������x�����Z
		fall += (fallinit * 2) / 45;

		//fallinit�������ő�l
		if (fall > fallinit)
		{
			fall = fallinit;
		}
	}

	eney += fall;

	//�ǂɂ߂荞�񂾎��ɕ␳
	while ((!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + 1 - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - 1 + Width / 2) / BLOCK_SIZE]))
	{
		eney++;
		fall = 0;
		jump = 0;
	}

	//�ǂɂ߂荞�񂾎��ɕ␳
	while ((!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]))
	{
		eney--;
		jump = 0;
	}


	//�v���C���[�ɓ����������U��
	//if (enex == player->GetX() && eney == player->GetY())
	//{
	//	player->HitEnemy(float damage);
	//}

	//�U���ҋ@���ԁE���G���Ԃ����炷
	if (HitCool)HitCool--;
	if (AttackCool)AttackCool--;

	Anim++;
}

void Boss::makeEnemy()
{
	MakeEnemy = FALSE;
	while (MakeEnemy == FALSE)
	{
		int i = 7;
		int j = 7;
		if (MapData[j][i] == 1)
		{
			enex = i * BLOCK_SIZE + BLOCK_SIZE / 2;
			eney = j * BLOCK_SIZE + BLOCK_SIZE / 2;
			MakeEnemy = TRUE;
		}
	}

	while ((MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][enex / BLOCK_SIZE]))
	{
		eney++;
	}

	enex += GetRand(80) - 40;
}

void Boss::Draw(int x, int y) const
{
	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		int WalkAnim = Anim / 18 % 2;

		//�G�̕\��
		if (!E_AttackFlg)
		{
			if (AttackCool)DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[2], TRUE, !Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, !Turnflg, false);
		}
		else
		{
			if (Attack < 60) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[3], TRUE, !Turnflg, false);
			else if (fall < 0) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[4], TRUE, !Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[0], TRUE, !Turnflg, false);
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

void Boss::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
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

//�̗͊m�F
bool Boss::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//�v���C���[����̍U��
void Boss::HitPlayer(float damage) {
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}

//�v���C���[�ւ̍U��
bool Boss::EnemyAttack(int x, int y)
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