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
	Width = 500;
	Height = 300;

	//�G�X�e�[�^�X
	Enemy_Hp = 5;

	//�G�U����
	Power = 1;

	//���x���ɂ�鋭��
	int Addhp = level / 3 + (level / 10 * 10);
	int addAtk = level / 4 + (level / 10);

	Enemy_Hp += Addhp;
	Power += addAtk;

	Power = 1;

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
	LoadDivGraph("images/Boss.png", 10, 10, 1, 530, 360, EImages);

	LoadDivGraph("images/claw.png", 6, 6, 1, 120, 330, ClawImg);

	Anim = 0;
	Turnflg = false;
	if (GetRand(1))Turnflg = !Turnflg;
}

void Boss::Update(Player* player)
{
	//�W�����v���x
	float fallinit = 12;

	//�v���C���[���W
	int PlayerX = player->GetX();

	//�U���Ɉڍs
	int sight = 300;	//�v���C���[��F������͈�
	ClawTime++;
	if (ClawCool < ClawTime && !Claw && !E_AttackFlg)
	{
		E_AttackFlg = true;
		Claw = true;
		ClawSpd = 0;
	}
	else if (enex + sight >= player->GetX() && enex - sight <= player->GetX() &&
			 eney + sight >= player->GetY() && eney - sight <= player->GetY() && !E_AttackFlg && !AttackCool && !Pounce)
	{
		//�F���͈͓��ɂ���΍U���J�n
		E_AttackFlg = true;
		Pounce = true;

		//�v���C���[�̕���������
		if (player->GetX() < enex) Turnflg = true;
		else Turnflg = false;
	}
	else if (!E_AttackFlg && !AttackCool) {
		//�ʏ�̈ړ�----------
		Movecnt++;

		if (PlayerX < enex)
		{
			int Dis = enex - PlayerX;	//�v���C���[�Ԃ̋��������
			Turnflg = true;
			if (Moveswitch <= Movecnt)	//Moveswitch�ȏ�o�߂������ړ�������ς���
			{
				if (Dis < 500) MoveAng = 1;
				else MoveAng = -1;

				Movecnt = 0;
				Moveswitch = GetRand(60) + 30;	//�����]���^�C�~���O��ς���
			}
		}
		else
		{
			int Dis = PlayerX - enex;	//�v���C���[�Ԃ̋��������
			Turnflg = false;
			if (Moveswitch <= Movecnt)	//Moveswitch�ȏ�o�߂������ړ�������ς���
			{
				if (Dis < 500) MoveAng = -1;
				else MoveAng = 1;

				Movecnt = 0;
				Moveswitch = GetRand(60) + 30;	//�����]���^�C�~���O��ς���
			}
		}

		enex += 4 * MoveAng;

		//-------------------
	}

	//�U���s��
	if (E_AttackFlg)
	{
		Attack++;
		if (Pounce)
		{
			//�W�����v���O�̑ҋ@
			if (Attack <= 60)
			{
				JumpDis = (PlayerX - enex) / 15;	//�v���C���[�Ԃ̋��������
				if (Attack == 60)
				{
					if (0 < JumpDis)Turnflg = false;
					else Turnflg = true;
				}
			}
			else if (Attack <= 90)
			{
				if (Attack < 75)
				{
					enex += JumpDis;
					FixX();
					eney -= 30;
					FixY();
				}
			}
			//�W�����v
			else if (90 < Attack && MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
			{
				eney += 24;
				FixY();
			}
			//�U���I��
			else if (90 < Attack && !MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
			{
				//�W�����v���Ē��n����΍U���I��
				E_AttackFlg = false;
				AttackCool = 60;
				Attack = 0;
				Pounce = false;
			}
		}

		else if (Claw)
		{
			//�ҋ@
			if (Attack <= 60 && ClawSpd == 0)
			{
				if (Attack == 60)
				{
					ClawSpd += 0.1;

					if (!Turnflg)ClawX = enex + 260;
					else ClawX = enex - 260;

					ClawY = eney;

					ClawTurn = !Turnflg;
				}
			}
			else if (80 < Attack)
			{
				E_AttackFlg = false;
				Attack = 0;
			}

		}
	}

	if (0 < ClawSpd)
	{
		if (ClawTurn)ClawX += ClawSpd;
		else ClawX -= ClawSpd;
		ClawSpd += 0.05;
	}

	if (12 < ClawSpd)
	{
		Claw = false;
		ClawSpd = 0;
		ClawTime = 0;
	}

	FixX();

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

	if (E_AttackFlg)fall = 0;
	eney += fall;

	FixY();

	//�U���ҋ@���ԁE���G���Ԃ����炷
	if (HitCool)HitCool--;
	if (AttackCool)AttackCool--;

	Anim++;
}

void Boss::FixX() 
{
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

}

void Boss::FixY()
{
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

	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
	//	enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);

	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		int WalkAnim = Anim / 18 % 2;

		int DrawX = enex - x + (SCREEN_WIDTH / 2);
		int DrawY = eney - y + (SCREEN_HEIGHT / 2) + BLOCK_SIZE;

		//�G�̕\��
		if (!E_AttackFlg)
		{
			if (AttackCool)DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[2], TRUE, !Turnflg, false);
			else DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[WalkAnim], TRUE, !Turnflg, false);
		}
		else if(Pounce)
		{
			if (Attack < 60) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[3], TRUE, !Turnflg, false);
			else if (Attack < 90) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[4], TRUE, !Turnflg, false);
			else DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[0], TRUE, !Turnflg, false);
		}
		else if (Claw)
		{
			if (Attack < 60) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[5], TRUE, !Turnflg, false);
			else if (Attack < 80) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[6], TRUE, !Turnflg, false);
		}
	}

	if (0 < ClawSpd) 
	{
		int cX = ClawX - x + (SCREEN_WIDTH / 2);
		int cY = ClawY - y + (SCREEN_HEIGHT / 2) + BLOCK_SIZE;
		int cAnim = 0;
		if (ClawSpd < 3)cAnim = 0 + (Anim / 15 % 2);
		else if (ClawSpd < 6)cAnim = 2 + (Anim / 15 % 2);
		else cAnim = 4 + (Anim / 15 % 2);

		DrawRotaGraph(cX, cY, 1, 0, ClawImg[cAnim], true, ClawTurn);
	}

	if (Enemy_Hp == 0)
	{
		DeleteGraph(EImages[0]);

		DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), DropItem_Image, TRUE);
	}

	//DrawFormatString(100, 100, 0xffffff, "%.1f", fall);

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
	if (Pounce)
	{
		if (E_AttackFlg && 60 < Attack)
		{
			float DisX = fabs(enex - x);
			float DisY = fabs(eney - y);

			if (DisX < Width / 2 && DisY < Height / 2)return true;
			else return false;
		}
		else return false;
	}

	if (Claw) 
	{
		if (0 < ClawSpd) 
		{
			float DisX = fabs(ClawX - x);
			float DisY = fabs(ClawY - y);

			int ColY = 0;

			if (ClawSpd < 3)ColY = 330;
			else if (ClawSpd < 6)ColY = 250;
			else ColY = 120;

			if (DisX < 60 && DisY < ColY / 2)return true;
			else return false;
		}
	}

	return false;
}