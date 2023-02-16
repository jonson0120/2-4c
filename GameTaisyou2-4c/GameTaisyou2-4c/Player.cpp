#include "Player.h"
#include "Enemy.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"
#include <math.h>

Player::Player() {
	image = 0;

	stat.Hp = 10;
	stat.Power = 0;

	x = 500;
	y = 500;

	Width = 32;
	Height = 48;

	speed = 8;
	fall = 16;
	jump = 0;

	Attack = 0;
	Equip = weapons::dagger;

	Yinput = Inp_UD::NONE;
	Combo = 0;
	range[0] = { 24,44 };
	range[1] = { 24,44 };

    LoadDivGraph("images/Player.png", 2, 36, 52, 72, 104, PImages);
	Weapon[0] = LoadGraph("images/Dagger.png");
	Weapon[1] = LoadGraph("images/mace.png");

	JoypadX = 0;
	JoypadY = 0;

	TurnFlg=false;
}

void Player::Update() {
	InitPad();

		//横移動
		if (JoypadX >= MARGIN) {
			x += speed;
			if (Attack < 1)TurnFlg = FALSE;
		}
		if (JoypadX <= -MARGIN) {
			x -= speed;
			if (Attack < 1)TurnFlg = TRUE;
		}
		while (!MapData[y / 160][(x + Width / 2) / 160])x--;
		while (!MapData[y / 160][(x - Width / 2) / 160])x++;

		//上下入力の更新
		if (JoypadY >= MARGIN * 2.5)
		{
			if (Attack < 1)Yinput = Inp_UD::UP;
		}
		else if (JoypadY <= -MARGIN * 2.5) 
		{
			if (Attack < 1)Yinput = Inp_UD::DOWN;
		}
		else 
		{
			if (Attack < 1)Yinput = Inp_UD::NONE;
		}

		if (PAD_INPUT::OnClick(XINPUT_BUTTON_LEFT_SHOULDER))
		{
			switch (Equip) 
			{
			case weapons::dagger:
				Equip = weapons::mace;
				Attack = 0;
				Combo = 0;
				stat.Power = 0;
				break;

			case weapons::mace:
				Equip = weapons::dagger;
				Attack = 0;
				Combo = 0;
				stat.Power = 0;
				break;
			}
		}

		//落下とジャンプ
		float fallinit = 16;

		if (PAD_INPUT::OnClick(XINPUT_BUTTON_A) && jump < 2)
		{
			fall = -fallinit;
			jump++;	
		}


		if (fall < fallinit)
		{
			fall += (fallinit * 2) / 45;
			if (fall > fallinit)
			{
				fall = fallinit;
			}
		}

		y += fall;

		while (!MapData[(y - Height / 2) / 160][(x - Width / 2) / 160] ||
			   !MapData[(y - Height / 2) / 160][(x + Width / 2) / 160])
		{
			y++;
			if (0 > fall)fall = 0;
		}

		while (!MapData[(y + Height / 2) / 160][(x - Width / 2) / 160] ||
			   !MapData[(y + Height / 2) / 160][(x + Width / 2) / 160])
		{
			y--;
			jump = 0;
			if (fall > 0)fall = 0;
		}


		//攻撃
		switch (Equip)
		{
		case weapons::dagger:	//短剣：ボタン単押しタイプ
			if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
			{
				if (Combo == 0)
				{
					Attack++;
					Combo++;
				}
				else if (Combo == 1 && 10 < Attack && Yinput != Inp_UD::UP)
				{
					Attack = 1;
					Combo++;
				}

			}
			break;

		case weapons::mace:		//メイス：ボタン長押しタイプ
			if (PAD_INPUT::OnClick(XINPUT_BUTTON_B) && Attack == 0)
			{
				Attack++;
			}
			break;

		default:
			break;
		}

		//攻撃ステータス・アニメーション管理
		if (Attack) 
		{
			switch (Equip)
			{
			case weapons::dagger:	//短剣：攻撃
				DaggerAtk();
				break;

			case weapons::mace:		//メイス：攻撃
				MaceAtk();
				break;

			default:
				break;
			}
		}
}

void Player::Draw() const {
	
	DrawBoxAA(SCREEN_WIDTH / 2 - (Width / 2), SCREEN_HEIGHT / 2 - (Height / 2),
		SCREEN_WIDTH / 2 + (Width / 2), SCREEN_HEIGHT / 2 + (Height / 2), 0xff0000, TRUE);

	DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (Height / 2) +6 , 1.0f, 0, PImages[0], TRUE, TurnFlg);

	//DrawFormatString(0, 30, 0xffffff, "%d", GetX());
	//DrawFormatString(0, 45, 0xffffff, "%d", GetY());

	//DrawFormatString(0, 60, 0xffffff, "%.1f", stat.Power);

	DrawString(0, 110, "LBで武器切り替え(暫定)", 0xff0000);
	switch (Equip)
	{
	case weapons::dagger:	//短剣
		DrawString(0, 130, "装備：短剣", 0xffffff);
		break;

	case weapons::mace:		//メイス
		DrawString(0, 130, "装備：メイス", 0xffffff);
		break;

	default:
		break;
	}



	//for (int i = 0; i < MAP_HEIGHT; i++)
	//{
	//	for (int j = 0; j < MAP_WIDTH; j++)
	//	{
	//		if (GetY() / 160 == i && GetX() / 160 == j) DrawFormatString(50 + 15 * j, 50 + 15 * i, 0xff0000, "9");
	//		else DrawFormatString(50 + 15 * j, 50 + 15 * i, 0xffffff, "%d", MapData[i][j]);
	//	}
	//}

	//攻撃描画
	if (Attack) 
	{
		switch (Equip)
		{
		case weapons::dagger:	//短剣
			DrawDagger();
			break;

		case weapons::mace:	//メイス
			DrawMace();
			break;

		default:
			break;
		}
	}
}

void Player::InitPad() {

	//スティック
	JoypadX = PAD_INPUT::GetPadThumbLX();
	JoypadY = PAD_INPUT::GetPadThumbLY();

}

void Player::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]) {

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}
}

//攻撃描画：短剣
void Player::DrawDagger()const
{
	float size = 0.3;
	if (Yinput == Inp_UD::UP)		//上方向入力：上振り
	{
		switch (TurnFlg)
		{
		case true:	//左向き時
			if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * Attack), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (-45 + ((90 / 10) * Attack)), Weapon[0], true, false);
			else if (Attack < 20) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * (Attack - 10)), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (45 - ((90 / 10) * (Attack - 10))), Weapon[0], true, true);
			break;

			case false:	//右向き時
			if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * Attack), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (45 - ((90 / 10) * Attack)), Weapon[0], true, true);
			else if (Attack < 20) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * (Attack - 10)), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (-45 + ((90 / 10) * (Attack - 10))), Weapon[0], true, false);
			break;
			default:
				break;
			}

	}
	else
	{
		switch (Combo)	//上入力なし：前方振り
		{
		case 1:					//コンボ１：斬り下ろし
			switch (TurnFlg)
			{
			case true:	//左向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 10 * Attack),
					size, (3.14 / 180) * (315 - ((90 / 10) * Attack)), Weapon[0], true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20),
					size, (3.14 / 180) * (315 - 90), Weapon[0], true, true);
				break;

			case false:	//右向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 10 * Attack),
					size, (3.14 / 180) * (45 + ((90 / 10) * Attack)), Weapon[0], true, false);
				else DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20),
					size, (3.14 / 180) * (45 + 90), Weapon[0], true, false);
				break;

			default:
				break;
			}
			break;

		case 2:					//コンボ２：斬り上げ
			switch (TurnFlg)
			{
			case true:	//左向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack),
					size, (3.14 / 180) * (225 + ((90 / 10) * Attack)), Weapon[0], true, false);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 20 * 20),
					size, (3.14 / 180) * (315), Weapon[0], true, false);
				break;

			case false:	//右向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack),
					size, (3.14 / 180) * (135 - ((90 / 10) * Attack)), Weapon[0], true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 20 * 20),
					size, (3.14 / 180) * (45), Weapon[0], true, true);
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}

//攻撃描画：メイス
void Player::DrawMace()const
{
	float size = 0.2;

	double stX = 0, stY = 0;		//振りかぶる前の座標
	double finX = 0, finY = 0;		//振りかぶった後の座標
	double Dis = 0;			//体の中心からの距離

	double stAng, finAng = 0;	//振りかぶる角度

	//上記の値を計算
	if (stat.Power == 0) {
		switch (TurnFlg)
		{
		case true:
			if (Attack < 20)
			{
				stAng = -90;
				finAng = 20;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = Width * 2;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

			}
			else if (Attack < 40)
			{
				stAng = -90;
				finAng = 45;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = Width * 2;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

			}
			else
			{
				stAng = -90;
				finAng = 70;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = Width * 2;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

			}
			break;
		case false:
			if (Attack < 20)
			{
				stAng = 90;
				finAng = -20;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = Width * 2;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else if (Attack < 40)
			{
				stAng = 90;
				finAng = -45;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = Width * 2;;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				stAng = 90;
				finAng = -70;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = Width * 2;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;

		default:
			break;
		}
	}
	else
	{
		int pow = stat.Power;
		switch (pow)
		{
		case 1:
			switch (TurnFlg)
			{
			case true:
				if (10 < Attack)
				{
					stAng = -90;
					finAng = -135 + (155 / 10 * (Attack - 10));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					stAng = -90;
					finAng = -135;
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				break;

			case false:
				if (10 < Attack)
				{
					stAng = 90;
					finAng = 135 - (155 / 10 * (Attack - 10));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					stAng = 90;
					finAng = 135;
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				break;
			}

		case 2:
			switch (TurnFlg)
			{
			case true:
				if (10 < Attack)
				{
					stAng = -90;
					finAng = -135 + (180 / 10 * (Attack - 10));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					stAng = -90;
					finAng = -135;
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				break;

			case false:
				if (10 < Attack)
				{
					stAng = 90;
					finAng = 135 - (180 / 10 * (Attack - 10));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					stAng = 90;
					finAng = 135;
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				break;
			}

		case 3:
			switch (TurnFlg)
			{
			case true:
				if (10 < Attack)
				{
					stAng = -90;
					finAng = -135 + (205 / 10 * (Attack - 10));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					stAng = -90;
					finAng = -135;
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				break;

			case false:
				if (10 < Attack)
				{
					stAng = 90;
					finAng = 135 - (205 / 10 * (Attack - 10));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					stAng = 90;
					finAng = 135;
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;
					Dis = Width * 2;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				break;
			}

		default:
			break;
		}
	}
	DrawRotaGraph(finX, finY, size, (3.14 / 180) * finAng, Weapon[1], true, false);
}

//攻撃：短剣
void Player::DaggerAtk() 
{
	Attack++;
	stat.Power = 1;
	if (Yinput == Inp_UD::UP)	//上方向入力
	{
		if (20 < Attack) {	//攻撃終了
			Attack = 0;
			Combo = 0;
			stat.Power = 0;
		}
	}
	else if (Combo == 1)	//上入力なし
	{
		if (35 < Attack)	//攻撃終了
		{
			Attack = 0;
			Combo = 0;
			stat.Power = 0;
		}
	}
	else if(20 < Attack)	//攻撃終了
	{
		Attack = 0;
		Combo = 0;
		stat.Power = 0;
	}
}

//攻撃：メイス
void Player::MaceAtk()
{

	if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && stat.Power == 0)Attack++;	//ボタン長押し中に力をためる
	else if (PAD_INPUT::OnRelease(XINPUT_BUTTON_B) && stat.Power == 0)		//離すとためた力に応じて強化
	{
		if (Attack < 20)
		{
			stat.Power = 1;
			Attack = 20;
		}
		else if (Attack < 40)
		{
			stat.Power = 2;
			Attack = 20;
		}
		else
		{
			stat.Power = 3;
			Attack = 20;
		}
	}

	if (60 < Attack) 
	{
		Attack = 60;
	}

	if (stat.Power)
	{
		Attack -= 1.5;
		if (Attack < 0)
		{
			Attack = 0;
			stat.Power = 0;
		}
	}
}

bool Player::HitDagger(int EneX, int EneY, int EneW, int EneH) {

	if (Attack && Attack < 10)
	{
		int WeaponX = 0;
		int WeaponY = 0;
		int RangeX = 0;
		int RangeY = 0;
		float Rad = 0;

		EneX = EneX - GetX() + SCREEN_WIDTH / 2;
		EneY = EneY - GetY() + SCREEN_HEIGHT / 2;

		switch (Combo)
		{
		case 1:
			switch (TurnFlg)
			{
			case true:
					WeaponX = SCREEN_WIDTH / 2 - (1.2 * Width);
					WeaponY = SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (315 - ((90 / 10) * Attack));
				break;

			case false:
			
					WeaponX = SCREEN_WIDTH / 2 + (1.2 * Width);
					WeaponY = SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (45 + ((90 / 10) * Attack));
				break;

			default:
				break;
			}
			break;

		case 2:
			switch (TurnFlg)
			{
			case true:
					WeaponX = SCREEN_WIDTH / 2 - (1.2 * Width);
					WeaponY = SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (225 + ((90 / 10) * Attack));
				break;

			case false:
					WeaponX = SCREEN_WIDTH / 2 + (1.2 * Width);
					WeaponY = SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (135 - ((90 / 10) * Attack));
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
		
		int DisX = EneX - WeaponX;
		int DisY = EneY - WeaponY;

		int Dis = sqrt(DisX * DisX + DisY * DisY);

		EneX += Dis * cos(Rad);
		EneY += Dis * sin(Rad);

		if (WeaponX - RangeX < EneX && WeaponY - RangeY < EneY && EneX < WeaponX + RangeX && EneY < WeaponY + RangeY) 
		{
			return true;
		}
	}

	return false;
}