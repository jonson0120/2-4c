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

	//敵座標
	enex = 0;
	eney = 0;

	//マップデータ
	MapData[eney][enex];

	//敵サイズ
	Width = 450;
	Height = 300;

	//敵ステータス
	Enemy_Hp = 1000;
	Power = 10;

	MakeEnemy = FALSE;

	E_AttackFlg = FALSE;

	//攻撃関連
	HighJump = false;
	Attack = 0;

	//攻撃後の硬直時間
	AttackCool = 0;

	//被弾後の無敵時間
	HitCool = 0;

	//移動関連
	speed = 0;
	fall = 12;
	jump = 0;

	//画像関連
	LoadDivGraph("images/Boss.png", 10, 10, 1, 530, 360, EImages);

	LoadDivGraph("images/claw.png", 6, 6, 1, 120, 330, ClawImg);

	RoarImg = LoadGraph("images/ripples.png");

	Anim = 0;
	Turnflg = false;
	if (GetRand(1))Turnflg = !Turnflg;
}

void Boss::Update(Player* player)
{
	//ジャンプ強度
	float fallinit = 12;

	//プレイヤー座標
	int PlayerX = player->GetX();

	//攻撃に移行
	int sight = 300;	//プレイヤーを認識する範囲
	ClawTime++;

	if (450 < RoarTime && !E_AttackFlg && !Roar) 
	{
		E_AttackFlg = true;
		Roar = true;
	}
	//爪攻撃
	if (ClawCool < ClawTime && !Claw && !E_AttackFlg)
	{
		if (player->GetX() < enex) Turnflg = true;
		else Turnflg = false;

		E_AttackFlg = true;
		Claw = true;
		ClawSpd = 0;
	}
	//飛び掛かり
	else if (enex + sight >= player->GetX() && enex - sight <= player->GetX() &&
			 eney + sight >= player->GetY() && eney - sight <= player->GetY() && !E_AttackFlg && !AttackCool && !Pounce)
	{
		//認識範囲内にいれば攻撃開始
		E_AttackFlg = true;
		Pounce = true;

		//プレイヤーの方向を向く
		if (player->GetX() < enex) Turnflg = true;
		else Turnflg = false;
	}
	else if (!E_AttackFlg && !AttackCool) {
		//通常の移動----------
		Movecnt++;

		if (PlayerX < enex)
		{
			int Dis = enex - PlayerX;	//プレイヤー間の距離を取る
			Turnflg = true;
			if (Moveswitch <= Movecnt)	//Moveswitch以上経過した時移動方向を変える
			{
				if (Dis < 500) MoveAng = 1;
				else MoveAng = -1;

				Movecnt = 0;
				Moveswitch = GetRand(60) + 30;	//方向転換タイミングを変える
			}
		}
		else
		{
			int Dis = PlayerX - enex;	//プレイヤー間の距離を取る
			Turnflg = false;
			if (Moveswitch <= Movecnt)	//Moveswitch以上経過した時移動方向を変える
			{
				if (Dis < 500) MoveAng = -1;
				else MoveAng = 1;

				Movecnt = 0;
				Moveswitch = GetRand(60) + 30;	//方向転換タイミングを変える
			}
		}

		enex += 4 * MoveAng;

		//-------------------
	}

	//攻撃行動
	if (E_AttackFlg)
	{
		Attack++;
		if (Pounce)
		{
			//ジャンプ直前の待機
			if (Attack <= 60)
			{
				JumpDis = (PlayerX - enex) / 15;	//プレイヤー間の距離を取る
				if (Attack == 60)
				{
					if (0 < JumpDis)Turnflg = false;
					else Turnflg = true;
				}
			}
			else if (Attack <= 90)
			{
				if (Attack < 80)
				{
					enex += JumpDis;
					FixX();
					eney -= 25;
					FixY();
				}
			}
			//ジャンプ
			else if (100 < Attack && MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
			{
				eney += 30;
				FixY();
			}
			//攻撃終了
			else if (90 < Attack && !MapData[(eney + 1 + Height / 2) / BLOCK_SIZE][enex / BLOCK_SIZE])
			{
				//ジャンプして着地すれば攻撃終了
				E_AttackFlg = false;
				AttackCool = 60;
				Attack = 0;
				Pounce = false;
				ClawTime += 20;
			}
		}

		else if (Claw)
		{
			//待機
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

		if (Roar)
		{
			if (Attack == 30)
			{
				player->SetKnockBack(30, enex);
			}
			else if (60 < Attack)
			{
				Roar = false;
				E_AttackFlg = false;
				RoarTime = 0;

				//飛び掛かりに派生
				E_AttackFlg = true;
				Pounce = true;
				Attack = 50;

				//プレイヤーの方向を向く
				if (player->GetX() < enex) Turnflg = true;
				else Turnflg = false;
			}
		}
	}
	else Attack = 0;

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

	//落下とジャンプ

	if (fall < fallinit)
	{
		//落下速度を加算
		fall += (fallinit * 2) / 45;

		//fallinit＝加速最大値
		if (fall > fallinit)
		{
			fall = fallinit;
		}
	}

	if (E_AttackFlg)fall = 0;
	eney += fall;

	FixY();

	//攻撃待機時間・無敵時間を減らす
	if (HitCool)HitCool--;
	if (AttackCool)AttackCool--;

	Anim++;
	if (enex == Oldx)RoarTime++;
	Oldx = enex;
}

void Boss::FixX() 
{
	//壁にめり込んだ時に補正
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE] ||
		!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
	{
		enex--;
		speed = 0;
	}

	//壁にめり込んだ時に補正
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
		!MapData[(eney + Height / 2) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
	{
		enex++;
		speed = 0;
	}

}

void Boss::FixY()
{
	//壁にめり込んだ時に補正
	while ((!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + 1 - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex - 1 + Width / 2) / BLOCK_SIZE]))
	{
		eney++;
		fall = 0;
		jump = 0;
	}

	//壁にめり込んだ時に補正
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

		//敵の表示
		if (!E_AttackFlg)
		{
			if (AttackCool)DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[2], TRUE, !Turnflg, false);
			else DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[WalkAnim], TRUE, !Turnflg, false);
		}
		else if(Pounce)
		{
			if (Attack < 60) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[3], TRUE, !Turnflg, false);
			else if (Attack < 100) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[4], TRUE, !Turnflg, false);
			else DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[0], TRUE, !Turnflg, false);
		}
		else if (Roar)
		{
			if (Attack < 30) DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[9], TRUE, !Turnflg, false);
			else
			{
				DrawRotaGraph(DrawX, DrawY, 1.0, 0, EImages[8], TRUE, !Turnflg, false);
				if (Turnflg)DrawRotaGraph(DrawX - 200, DrawY, Attack - 30, 0, RoarImg, TRUE, !Turnflg, false);
				else	   DrawRotaGraph(DrawX + 200, DrawY, Attack - 30, 0, RoarImg, TRUE, !Turnflg, false);
			}
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
		if (ClawSpd < 4)cAnim = 0 + (Anim / 15 % 2);
		else if (ClawSpd < 8)cAnim = 2 + (Anim / 15 % 2);
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

//体力確認
bool Boss::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//プレイヤーからの攻撃
void Boss::HitPlayer(float damage) {
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}

//プレイヤーへの攻撃
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

			if (ClawSpd < 4)ColY = 330;
			else if (ClawSpd < 8)ColY = 250;
			else ColY = 120;

			if (DisX < 60 && DisY < ColY / 2)return true;
			else return false;
		}
	}

	return false;
}