#include "DeepSlime.h"
#include "Enemy.h"
#include "DxLib.h"
#include "common.h"
#include "Player.h"
#include "GameMainScene.h"

#include <math.h>

#define MAX_SPEED 3
#define MIN_SPEED -3

DeepSlime::DeepSlime(int level) : Enemy()
{
	image = 0;


	//敵座標
	enex = 0;
	eney = 0;

	//マップデータ
	MapData[eney][enex];

	//敵サイズ
	Width = 48;
	Height = 64;

	//敵ステータス
	Enemy_Hp = 7;

	//敵攻撃力
	Power = 1;

	//レベルによる強化
	int Addhp = level / 3 + (level / 10 * 8);
	int addAtk = level / 6 + (level / 10);

	Enemy_Hp += Addhp;
	Power += addAtk;

	MakeEnemy = FALSE;

	E_AttackFlg = FALSE;
	AtkTurnFlg = false;

	//攻撃関連
	HighJump = false;
	Attack = 0;
	AtkSpot = { 0,0 };

	//攻撃後の硬直時間
	AttackCool = 0;

	//被弾後の無敵時間
	HitCool = 0;

	//移動関連
	speed = 0;
	fall = 12;
	Atkfall = 12;
	jump = 0;

	//画像関連
	LoadDivGraph("images/Deepslime.png", 7, 7, 1, 64, 64, EImages);
	Anim = 0;
	Turnflg = false;
	if (GetRand(1))Turnflg = !Turnflg;
}

void DeepSlime::Update(Player* player)
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

		AtkTurnFlg = Turnflg;

		AttackCool = 80;
		AtkSpot = { enex,eney };
	}

	if(!AttackCool) {
		//通常の移動----------
		if (Turnflg)
		{
			enex -= 3;
			//壁にぶつかると向きを反転させる
			if (!MapData[eney / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE] ||
				MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][(enex - Width / 2) / BLOCK_SIZE])
			{
				Turnflg = !Turnflg;
			}
		}
		else
		{
			enex += 3;
			//壁にぶつかると向きを反転させる
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
				Atkfall = -fallinit * 1.4;
				HighJump = true;
			}
			//水平方向ジャンプ
			else
			{
				Atkfall = -fallinit * 0.5;
				HighJump = false;
			}
			AtkSpot = { enex,eney };
		}
		//ジャンプ
		else if (60 < Attack && MapData[AtkSpot.Y / BLOCK_SIZE][AtkSpot.X / BLOCK_SIZE])
		{
			int jump = 11;

			//縦方向ジャンプ
			if (HighJump)
			{
				if (AtkTurnFlg)AtkSpot.X -= jump / 3;
				else AtkSpot.X += jump / 3;
			}
			//横方向ジャンプ
			else
			{
				if (AtkTurnFlg)AtkSpot.X -= jump;
				else AtkSpot.X += jump;
			}
		}
		//攻撃終了
		else if (60 < Attack && !MapData[AtkSpot.Y / BLOCK_SIZE][AtkSpot.X / BLOCK_SIZE])
		{
			//ジャンプして着地すれば攻撃終了
			E_AttackFlg = false;
			Attack = 0;
		}
	}

	//壁にめり込んだ時に補正
	while (!MapData[(eney - Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE]||
			!MapData[(eney +  Height / 2) / BLOCK_SIZE][(enex + Width / 2) / BLOCK_SIZE])
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

	if (Atkfall < fallinit)
	{
		//落下速度を加算
		Atkfall += (fallinit * 2) / 45;

		//fallinit＝加速最大値
		if (Atkfall > fallinit)
		{
			Atkfall = fallinit;
		}
	}

	eney += fall;
	AtkSpot.Y += Atkfall;

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


	//プレイヤーに当たった時攻撃
	//if (enex == player->GetX() && eney == player->GetY())
	//{
	//	player->HitEnemy(float damage);
	//}

	//攻撃待機時間・無敵時間を減らす
	if (HitCool)HitCool--;
	if (AttackCool)AttackCool--;

	Anim++;
}

void DeepSlime::makeEnemy()
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

	while ((MapData[(eney + Height / 2 + 1) / BLOCK_SIZE][enex / BLOCK_SIZE]))
	{
		eney++;
	}
	enex += GetRand(80) - 40;
}

void DeepSlime::Draw(int x,int y) const
{
	if (MakeEnemy == TRUE && HitCool % 4 < 2)
	{
		int WalkAnim = Anim / 18 % 2;

		//敵の表示
		if (!AttackCool)
		{
			DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[WalkAnim], TRUE, Turnflg, false);
		}
		else 
		{
			if (Attack < 60) DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[2], TRUE, Turnflg, false);
			else DrawRotaGraph(enex - x + (SCREEN_WIDTH / 2), eney - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[5], TRUE, Turnflg, false);
		}
	}

	if (60 < Attack)
	{
		DrawRotaGraph(AtkSpot.X - x + (SCREEN_WIDTH / 2), AtkSpot.Y - y + (SCREEN_HEIGHT / 2), 1.0, 0, EImages[6], TRUE, Turnflg, false);
	}
	//DrawFormatString(100, 100, 0xffffff, "%.1f", fall);

	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
	//		  enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
}

void DeepSlime::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
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
bool DeepSlime::CheckHp() {
	if (Enemy_Hp <= 0)return true;

	return false;
}

//プレイヤーからの攻撃
void DeepSlime::HitPlayer(float damage) {
	if (!HitCool) {
		Enemy_Hp -= damage;
		HitCool = 30;
	}
}

//プレイヤーへの攻撃
bool DeepSlime::EnemyAttack(int x, int y)
{
	if (E_AttackFlg && 60 < Attack)
	{
		float DisX = pow(AtkSpot.X - x, 2);
		float DisY = pow(AtkSpot.Y - y, 2);

		int Dis = (int)sqrt((int)(DisX + DisY));
		
		if (Dis < Width)return true;
		else return false;
	}
	else return false;
}