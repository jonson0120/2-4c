#include "Player.h"
#include "Enemy.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"
#include <math.h>

Player::Player() {
	Walk = 0;

	stat.Hp = 10;
	stat.MaxHp = stat.Hp;
	stat.Power = 0;
	PotionCount = 3;

	Width = 32;
	Height = 56;

	Spawn();
	Arm_L = { SCREEN_WIDTH / 2 + 10,SCREEN_HEIGHT / 2 };
	Arm_R = { SCREEN_WIDTH / 2 - 10,SCREEN_HEIGHT / 2 };
	ArmAngle_L = 0;
	ArmAngle_R = 0;

	speedinit = 8;
	speed = 0;
	Dodgespd = 0;

	fall = 0;
	Dodgefall = 0;
	jump = 0;
	wall = 0;

	HitCool = 0;
	Attack = 0;
	Equip[0] = weapons::dagger;
	Equip[1] = weapons::spear;

	Yinput = Inp_UD::NONE;
	Combo = 0;
	range[0] = { 24,44 };
	range[1] = { 26,75 };
	range[2] = { 25,93 };
	range[3] = { 11,82 };

	LoadDivGraph("images/Player_Top.png", 5, 5, 1, 36, 28, PImages);
	LoadDivGraph("images/Player_Under.png", 7, 7, 1, 48, 28, image_U);
	ArmImg = LoadGraph("images/arm.png");

	Weapon[0] = LoadGraph("images/Dagger.png");
	Weapon[1] = LoadGraph("images/mace2.png");
	Weapon[2] = LoadGraph("images/spear.png");
	Weapon[3] = LoadGraph("images/katana.png");

	LoadDivGraph("images/slash.png", 4, 4, 1, 8, 80, WeaponEffect);

	JoypadX = 0;
	JoypadY = 0;
	TriggerL = 0;
	TriggerR = 0;

	TurnFlg = false;

	Enemy_Damage = 1;
	//--------------------
	Atkpt = 0;
	Search = false;
	Near_Enemy = { -1,-1 };

	spear_angle = 0;
	for (int i = 0; i < Katana_num; i++)
	{
		katana_slash[i].X = 0;
		katana_slash[i].Y = 0;
		katana_angle[i] = 0;
	}

	//--------------------
}

void Player::Update() {
	InitPad();

	float Maxspeed = speedinit;	//最大速度
	float CorSpeed = 1;			//移動速度補正

	//壁面移動・Aボタン長押しで処理に入る
	if (PAD_INPUT::OnPressed(XINPUT_BUTTON_A))
	{
		//壁面移動・左壁
		//壁面移動中か左側が壁なら入る
		if ((wall == 1 || !MapData[y / 160][(x - 1 - Width / 2) / 160]))
		{
			if (JoypadX <= -MARGIN && !Attack) {
				fall = 0;	//落下速度0
				jump = 1;	//ジャンプ回数

				wall = 1;
			}

			if (Attack)CorSpeed = 0;	//壁面移動中は攻撃中に移動できない
			else CorSpeed = 0.5;		//壁面移動中は移動速度を半減させる

			//左側が壁でなくなったなら壁面移動を解除
			if (MapData[(y - Height / 2) / 160][(x - 1 - Width / 2) / 160]&&
				MapData[(y + Height / 2) / 160][(x - 1 - Width / 2) / 160])
			{
				wall = 0;
			}
		}

		//壁面移動・右壁
		//壁面移動中か右側が壁なら入る
		if ((wall == 2 || !MapData[y / 160][(x + 1 + Width / 2) / 160]))
		{
			if (MARGIN <= JoypadX && !Attack) {
				fall = 0;	//落下速度0
				jump = 1;	//ジャンプ回数

				wall = 2;
			}

			if (Attack)CorSpeed = 0;	//壁面移動中は攻撃中に移動できない
			else CorSpeed = 0.5;		//壁面移動中は移動速度を半減させる

			//右側が壁でなくなったなら壁面移動を解除
			if (MapData[(y - Height / 2) / 160][(x + 1 + Width / 2) / 160]&&
				MapData[(y + Height / 2) / 160][(x + 1 + Width / 2) / 160])
			{
				wall = 0;
			}
		}

		//壁面移動・天井
		//壁面移動中か上が壁なら入る
		if ((wall == 3 || !MapData[(y - 1 - Height / 2) / 160][(x - Width / 2) / 160] ||
						  !MapData[(y - 1 - Height / 2) / 160][(x + Width / 2) / 160]))
		{
			if (JoypadY >= MARGIN && !Attack) {
				fall = 0;	//落下速度0
				jump = 1;	//ジャンプ回数

				wall = 3;	//上入力されているなら壁面移動になる
			}

			if (Attack)CorSpeed = 0;	//壁面移動中は攻撃中に移動できない
			else CorSpeed = 0.5;		//壁面移動中は移動速度を半減させる

			//上が壁でなくなったなら壁面移動を解除
			if (MapData[(y - 1 - Height / 2) / 160][(x - Width / 2) / 160] &&
				MapData[(y - 1 - Height / 2) / 160][(x + Width / 2) / 160])
			{
				wall = 0;
			}
		}
	}
	else wall = 0;

		//横移動
		//スティック入力時
		if (JoypadX >= MARGIN) {
			if (wall != 1 && wall != 2)speed += 0.5;	//移動量を加算
			if (Attack < 1)TurnFlg = FALSE;				//向きを変える
			if (!wall)Walk++;							//歩行アニメーション進行
		}
		else if (JoypadX <= -MARGIN) {
			if (wall != 1 && wall != 2)speed -= 0.5;	//移動量を減算
			if (Attack < 1)TurnFlg = TRUE;				//向きを変える
			if (!wall)Walk++;							//歩行アニメーション進行
		}
		//非スティック入力時
		else 
		{
			//移動速度を0に近づける
			if (speed < 0 && 0 < ++speed) {
				speed = 0;
			}

			if (0 < speed && --speed < 0) {
				speed = 0;
			}
			Walk = 0;	//歩行アニメーションリセット
		}

		if (40 <= Walk)Walk = 0;

		//移動速度の最大値を適用
		if (speed < -Maxspeed)speed = -Maxspeed;
		if (Maxspeed < speed)speed = Maxspeed;

		//バックステップ回避
		if (PAD_INPUT::OnClick(XINPUT_BUTTON_RIGHT_SHOULDER) && Dodgespd == 0)
		{
			float Dodge = 18;
			if (TurnFlg)
			{
				Dodgespd = Dodge;
				Dodgefall = -Dodge;
				speed = 0;
			}
			else 
			{
				Dodgespd = -Dodge;
				Dodgefall = -Dodge;
				speed = 0;
			}
		}

		//移動速度を0に近づける
		if (Dodgespd != 0) {
			if (Dodgespd < 0 && 0 < ++Dodgespd) {
				Dodgespd = 0;
			}
		}

		if (Dodgespd != 0) {
			if (0 < Dodgespd && --Dodgespd < 0) {
				Dodgespd = 0;
			}
		}

		//移動速度を0に近づける
		if (Dodgefall != 0) {
			if (0 <= Dodgefall) {
				Dodgefall = 0;
			}
			else Dodgefall += 2;
		}

		if (Attack && !wall)CorSpeed = 0.5;	//攻撃中は移動速度を半減

		//武器種ごとの移動速度補正
		if (Attack) {

			//槍・攻撃中、空中にいなければ移動できない
			if (Equip[EquipNum] == weapons::spear && !MapData[(y + Height / 2 + 1) / 160][(x - Width / 2) / 160] &&
										   !MapData[(y + Height / 2 + 1) / 160][(x + Width / 2) / 160])
			{
				//CorSpeed = 0;
				speed = 0;
			}

			//刀・攻撃中、空中にいなければ移動できない
			if (Equip[EquipNum] == weapons::katana && !MapData[(y + Height / 2 + 1) / 160][(x - Width / 2) / 160] &&
				!MapData[(y + Height / 2 + 1) / 160][(x + Width / 2) / 160])
			{
				//CorSpeed = 0;
				speed = 0;
			}

		}

		x += (speed + Dodgespd) * CorSpeed;				//横軸移動

		//壁で移動を止める
		while (!MapData[y / 160][(x + Width / 2) / 160])
		{
			x--;
			speed = 0;
		}
		while (!MapData[y / 160][(x - Width / 2) / 160])
		{
			x++;
			speed = 0;
		}

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

		//武器切り替え・攻撃アニメーション・コンボ数をリセット
		if (PAD_INPUT::OnClick(XINPUT_BUTTON_LEFT_SHOULDER) && !Attack)
		{
			if (1 < ++EquipNum) EquipNum = 0;
			Attack = 0;
			Combo = 0;
			stat.Power = 0;
		}

		//落下とジャンプ
		float fallinit = 16;

		//非壁面移動時
		if (wall == 0)
		{
			//Aボタン・ジャンプ
			if (PAD_INPUT::OnClick(XINPUT_BUTTON_A) && jump < 2)
			{
				fall = -fallinit;	//落下速度をマイナスにする
				jump++;				//ジャンプ回数を増やす
			}

			//落下速度管理
			if (fall < fallinit)
			{	
				//落下速度を増やす
				fall += (fallinit * 2) / 45;
				if (fall > fallinit)
				{
					fall = fallinit;	//落下速度の最大値
				}
			}

			y += fall + Dodgefall;	//上下移動
		}
		else if (wall == 1 || wall == 2)
		{
			if (-MARGIN >= JoypadY && !Attack) {
				y += Maxspeed / 2;
			}
			else if (JoypadY >= MARGIN && !Attack) {
				y -= Maxspeed / 2;
			}
			else y++;
		}

		//床で落下が阻まれる
		while (!MapData[(y - Height / 2) / 160][(x - Width / 2) / 160] ||
			   !MapData[(y - Height / 2) / 160][(x + Width / 2) / 160])
		{
			y++;
			if (fall < 0)fall = 0;
		}

		//天井でジャンプが阻まれる
		while (!MapData[(y + Height / 2) / 160][(x - Width / 2) / 160] ||
			   !MapData[(y + Height / 2) / 160][(x + Width / 2) / 160])
		{
			y--;
			jump = 0;
			if (fall > 0)fall = 0;
		}

		//攻撃
		switch (Equip[EquipNum])
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

		case weapons::spear:		//槍：ボタン単押しタイプ
			if (PAD_INPUT::OnClick(XINPUT_BUTTON_B) && Attack == 0)
			{
				spear_angle = PadangL;

				if (JoypadX < MARGIN && -MARGIN < JoypadX && JoypadY < MARGIN && -MARGIN < JoypadY)
				{
					if (TurnFlg)spear_angle = -90;
					else spear_angle = 90;
				}

				Atkpt = wall;
				Attack++;
			}
			break;

		case weapons::katana:		//刀：ボタン単押しタイプ
			if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
			{
				if (Combo == 0)
				{
					Attack++;
					Combo++;
				}
				else if (Combo == 1 && 9 < Attack)
				{
					Attack = 1;
					Combo++;
				}
				else if (Combo == 2 && 12 < Attack && !wall)
				{
					Attack = 1;
					Combo++;
				}
			}
			break;

		default:
			break;
		}

		//攻撃ステータス・武器、腕アニメーション管理
		if (Attack) 
		{
			switch (Equip[EquipNum])
			{
			case weapons::dagger:	//短剣：攻撃
				DaggerAtk();
				break;

			case weapons::mace:		//メイス：攻撃
				MaceAtk();
				break;

			case weapons::spear:	//槍：攻撃
				SpearAtk();
				break;

			case weapons::katana:	//刀：攻撃
				KatanaAtk();
				break;

			default:
				break;
			}
		}
		else 
		{
			if (TurnFlg || wall == 1 || wall == 3)
			{
				Arm_L = { SCREEN_WIDTH / 2 + 12, SCREEN_HEIGHT / 2 };
				ArmAngle_L = 0;

				Arm_R = { SCREEN_WIDTH / 2 - 13, SCREEN_HEIGHT / 2 };
				ArmAngle_R = 0;

				if (wall == 1 || (wall == 3 && TurnFlg))
				{
					Arm_R = { SCREEN_WIDTH / 2 + 11, SCREEN_HEIGHT / 2 };
					ArmAngle_R = 0;
				}
			}
			else
			{

				Arm_L = { SCREEN_WIDTH / 2 + 13, SCREEN_HEIGHT / 2 };
				ArmAngle_L = 0;

				Arm_R = { SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 };
				ArmAngle_R = 0;
			}
		}

		//回復

		//Xボタンを押すと回復
		if (PAD_INPUT::OnClick(XINPUT_BUTTON_X) && 0 < PotionCount)
		{
			PotionCount--;

			stat.Hp += (stat.MaxHp  * 0.2);
		}

		//HP制限
		if (stat.MaxHp < stat.Hp)
		{
			stat.Hp = stat.MaxHp;
		}

		if (stat.Hp < 0)stat.Hp = 0;

		if (HitCool)HitCool--;
}

void Player::Draw() const {
	
	//DrawBoxAA(SCREEN_WIDTH / 2 - (Width / 2), SCREEN_HEIGHT / 2 - (Height / 2),
	//	SCREEN_WIDTH / 2 + (Width / 2), SCREEN_HEIGHT / 2 + (Height / 2), 0xff0000, TRUE);


	//DrawFormatString(0, 30, 0xffffff, "%d", GetX());
	//DrawFormatString(0, 45, 0xffffff, "%d", GetY());

	//DrawFormatString(0, 360, 0xffffff, "%d", wall);


	//DrawString(0, 110, "LBで武器切り替え(暫定)", 0xff0000);
	//switch (Equip[0])
	//{
	//case weapons::dagger:	//短剣
	//	DrawString(0, 130, "装備１：短剣", 0xffffff);
	//	break;

	//case weapons::mace:		//メイス
	//	DrawString(0, 130, "装備１：メイス", 0xffffff);
	//	break;

	//case weapons::spear:	//槍
	//	DrawString(0, 130, "装備１：槍", 0xffffff);
	//	break;

	//case weapons::katana:	//刀
	//	DrawString(0, 130, "装備１：刀", 0xffffff);
	//	break;

	//default:
	//	break;
	//}

	//switch (Equip[1])
	//{
	//case weapons::dagger:	//短剣
	//	DrawString(0, 150, "装備２：短剣", 0xffffff);
	//	break;

	//case weapons::mace:		//メイス
	//	DrawString(0, 150, "装備２：メイス", 0xffffff);
	//	break;

	//case weapons::spear:	//槍
	//	DrawString(0, 150, "装備２：槍", 0xffffff);
	//	break;

	//case weapons::katana:	//刀
	//	DrawString(0, 150, "装備２：刀", 0xffffff);
	//	break;

	//default:
	//	break;
	//}

	//for (int i = 0; i < MAP_HEIGHT; i++)
	//{
	//	for (int j = 0; j < MAP_WIDTH; j++)
	//	{
	//		if (GetY() / 160 == i && GetX() / 160 == j) DrawFormatString(50 + 15 * j, 170 + 15 * i, 0xff0000, "9");
	//		else DrawFormatString(50 + 15 * j, 170 + 15 * i, 0xffffff, "%d", MapData[i][j]);
	//	}
	//}
	

	//攻撃描画：プレイヤー後方
	if (Attack) 
	{
		switch (Equip[EquipNum])
		{
		case weapons::dagger:	//短剣
			DrawDagger();
			break;

		case weapons::mace:	//メイス
			DrawMace();
			break;

		case weapons::spear:	//槍
			if (!TurnFlg || wall)DrawSpear();
			break;

		case weapons::katana:	//刀
			if (!TurnFlg)DrawKatana();
			break;

		default:
			break;
		}
	}

	if (HitCool % 4 < 2)
	{
		//腕：プレイヤー後方
		if (TurnFlg || wall == 1 || wall == 3)
		{
			DrawRotaGraph(Arm_R.X, Arm_R.Y, 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);
		}
		else if (wall == 0)
		{
			DrawRotaGraph(Arm_L.X, Arm_L.Y, 1, (3.14 / 180) * ArmAngle_L, ArmImg, true, true);
		}

		if (Attack && Equip[EquipNum] == weapons::katana)
			DrawRotaGraph(Arm_R.X, Arm_R.Y, 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);

		//プレイヤー本体

		//上半身
		if (wall)
		{
			if (wall == 1)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4, 1.0f, 0, PImages[2], TRUE, true);
			if (wall == 2)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4, 1.0f, 0, PImages[1], TRUE, false);
			if (wall == 3)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4, 1.0f, 0, PImages[3 + TurnFlg], TRUE, TurnFlg);
		}
		else DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4, 1.0f, 0, PImages[0], TRUE, TurnFlg);

		//下半身
		if (wall)
		{
			if (wall == 1)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4, 1.0f, 0, image_U[6], TRUE, true);
			if (wall == 2)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4, 1.0f, 0, image_U[5], TRUE, false);
			if (wall == 3)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4, 1.0f, 0, image_U[5 + TurnFlg], TRUE, TurnFlg);
		}
		else if (MapData[(y + Height / 2 + 1) / 160][(x - Width / 2) / 160] &&
			MapData[(y + Height / 2 + 1) / 160][(x + Width / 2) / 160]) {
			DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4, 1.0f, 0, image_U[4], TRUE, TurnFlg);
		}
		else
		{
			DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4, 1.0f, 0, image_U[Walk / 10], TRUE, TurnFlg);
		}

		//腕：プレイヤー前面
		if (TurnFlg)
		{
			if (wall == 0)DrawRotaGraph(Arm_L.X, Arm_L.Y, 1, (3.14 / 180) * ArmAngle_L, ArmImg, true, true);
		}
		else
		{
			if (wall != 1 && (!Attack || Equip[EquipNum] != weapons::katana))
				DrawRotaGraph(Arm_R.X, Arm_R.Y, 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);
		}

		//攻撃描画：プレイヤー前方
		if (Attack)
		{
			switch (Equip[EquipNum])
			{
			case weapons::spear:	//槍
				if (TurnFlg && !wall)DrawSpear();
				break;

			case weapons::katana:	//刀
				if (TurnFlg)
				{
					DrawKatana();
					DrawRotaGraph(Arm_R.X, Arm_R.Y, 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);
					DrawRotaGraph(Arm_L.X, Arm_L.Y, 1, (3.14 / 180) * ArmAngle_L, ArmImg, true, true);
				}
				break;

			default:
				break;
			}
		}
	}

	//DrawCircle(a, b, 3.0f, 0xff0000, true);
	//DrawCircle(c, d, 3.0f, 0x00ff00, true);
	//DrawCircle(e, f, 3.0f, 0x00ff00, true);
}

void Player::Spawn() {
	x = BLOCK_SIZE + BLOCK_SIZE / 2;
	y = BLOCK_SIZE * (GetRand(MAP_HEIGHT - 3) + 1);

	y += BLOCK_SIZE - Height / 2;
}

void Player::InitPad() {

	//スティック
	JoypadX = PAD_INPUT::GetPadThumbLX();
	JoypadY = PAD_INPUT::GetPadThumbLY();

	//トリガー
	TriggerL = PAD_INPUT::GetPadLeftTrigger();
	TriggerR = PAD_INPUT::GetPadRightTrigger();

	//入力角度
	float angle = atan2((float)JoypadX, (float)JoypadY);
	if (angle < 0)angle += 3.14;

	angle = angle / 2 / 3.14 * 360;
	if (JoypadX < 0)angle -= 180;

	PadangL = angle;

}
//マップデータ
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
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 11 * Attack),
					size, (3.14 / 180) * (315 - ((90 / 10) * Attack)), Weapon[0], true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20),
					size, (3.14 / 180) * (315 - 90), Weapon[0], true, true);
				break;

			case false:	//右向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 11 * Attack),
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

//攻撃描画：槍
void Player::DrawSpear()const
{
	float size = 0.2;

	double stX = 0, stY = 0;		//振りかぶる前の座標
	double finX = 0, finY = 0;		//振りかぶった後の座標
	double Dis = 0;			//体の中心からの距離
	int thrust = 20;	//攻撃距離

	double finAng = spear_angle;	//攻撃する角度

	//上記の値を計算

	switch (Atkpt)
	{
	case 0:
		switch (TurnFlg)
		{
		case true:
			if (Attack < 8)
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust * Attack;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust * (8 - (Attack - 8));
				if (Dis < thrust)Dis = thrust;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;
		case false:
			if (Attack < 8)
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust * Attack;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust * (8 - (Attack - 8));
				if (Dis < thrust)Dis = thrust;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;

		default:
			break;
		}
		break;

	case 1:

		if (Attack < 11)
		{
			finAng = 25 + (130 / 10 * (Attack - 1));
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;
			Dis = Width * 1.5;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finAng = 155;
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;
			Dis = Width * 1.5;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		break;

	case 2:

		if (Attack < 11)
		{
			finAng = -25 - (130 / 10 * (Attack - 1));
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;
			Dis = Width * 1.5;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finAng = -155;
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;
			Dis = Width * 1.5;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		break;

	case 3:

		if (Attack < 11)
		{
			finAng = 115 + (130 / 10 * (Attack - 1));
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;
			Dis = Width * 1.5;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finAng = 245;
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;
			Dis = Width * 1.5;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		break;

	default:
		break;
	}

	DrawRotaGraph(finX, finY, size, (3.14 / 180) * finAng, Weapon[2], true, false);
}

//攻撃描画：刀
void Player::DrawKatana()const
{
	float size = 0.2;
	float sizeY = 1;

	double stX = 0, stY = 0;		//振りかぶる前の座標
	double finX = 0, finY = 0;		//振りかぶった後の座標
	double Dis = 0;			//体の中心からの距離

	bool TurnX = true;		//画像のX反転

	double finAng = 0;	//攻撃する角度
	int thrust = 85;	//攻撃距離

	int slashanim = 0;	//斬撃アニメーション

	//画像サイズ取得
	int imgX;
	int imgY;

	GetGraphSize(Weapon[3], &imgX, &imgY);

	//上記の値を計算

	if (Combo == 1) {
		switch (TurnFlg)
		{
		case true:

			TurnX = true;
			if (Attack < 6)
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = -thrust + thrust * 2 / 6 * Attack;
				sizeY = -1.0 + 2.0 / 6.0 * Attack;

				finAng = -80;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

			}
			else
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust;
				sizeY = 1;

				finAng = -80;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

			}
			break;
		case false:

			TurnX = false;
			if (Attack < 6)
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = -thrust + thrust * 2 / 6 * Attack;
				sizeY = -1.0 + 2.0 / 6.0 * Attack;

				finAng = 80;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

			}
			else
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust;
				sizeY = 1;

				finAng = 80;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;

		default:
			break;
		}
	}

	if (Combo == 2) {

		switch (TurnFlg)
		{
		case true:

			TurnX = false;
			if (Attack <= 9)
			{
				finAng = -140 + (80 / 8 * (Attack - 1));
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				finAng = -60;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;

		case false:

			TurnX = true;
			if (Attack <= 9)
			{
				finAng = 140 - (80 / 8 * (Attack - 1));
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				finAng = 60;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;

		default:
			break;
		}
	}

	if (Combo == 3) {
		thrust = thrust - imgY / 2 * size;

		switch (TurnFlg)
		{
		case true:

			TurnX = true;
			if (Attack < 15)
			{
				finAng = -60 + (180 / 12 * (Attack - 1));
				if (120 < finAng)finAng = 120;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust + Attack * 2;

				finX = stX + Dis * cos((3.14 / 180) * (-60 - 90));
				finY = stY + Dis * sin((3.14 / 180) * (-60 - 90));
			}
			else if (Attack < 18)
			{
				finAng = 120;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust - (thrust / 4 * (Attack - 15)) + (30 - 30 / 4 * (Attack - 15));
				finX = stX + Dis * cos((3.14 / 180) * (-60 - 90));
				finY = stY + Dis * sin((3.14 / 180) * (-60 - 90));
			}
			else
			{
				finAng = 120;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = 0;

				finX = stX + Dis * cos((3.14 / 180) * (-60 - 90));
				finY = stY + Dis * sin((3.14 / 180) * (-60 - 90));
			}
			break;

		case false:

			TurnX = false;
			if (Attack < 15)
			{
				finAng = 60 - (180 / 12 * (Attack - 1));
				if (finAng < -120)finAng = -120;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust + Attack * 2;

				finX = stX + Dis * cos((3.14 / 180) * (60 - 90));
				finY = stY + Dis * sin((3.14 / 180) * (60 - 90));
			}
			else if (Attack < 18)
			{
				finAng = -120;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = thrust - (thrust / 4 * (Attack - 15)) + (30 - 30 / 4 * (Attack - 15));

				finX = stX + Dis * cos((3.14 / 180) * (60 - 90));
				finY = stY + Dis * sin((3.14 / 180) * (60 - 90));
			}
			else
			{
				finAng = -120;
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				Dis = 0;

				finX = stX + Dis * cos((3.14 / 180) * (60 - 90));
				finY = stY + Dis * sin((3.14 / 180) * (60 - 90));
			}
			break;

		default:
			break;
		}

		if (Attack >= 18) {
			if (Attack <= 25)slashanim = (Attack - 18) / 2;
			else slashanim = 3 - (Attack - 26) / 2;
		}

	}

	switch (Combo)
	{
	case 1:
		DrawRotaGraph3(finX, finY, imgX / 2, imgY / 2, size, sizeY * size,
			(3.14 / 180) * finAng, Weapon[3], true, TurnX, false);
		break;

	case 2:
		DrawRotaGraph3(finX, finY, imgX / 2, imgY / 2, size, sizeY * size,
			(3.14 / 180) * finAng, Weapon[3], true, TurnX, false);
		break;

	case 3:
		DrawRotaGraph3(finX, finY, imgX / 2, imgY, size, sizeY * size,
			(3.14 / 180) * finAng, Weapon[3], true, TurnX, false);

		if (18 < Attack) {
			for (int i = 0; i < Katana_num; i++)
			{
				DrawRotaGraph(katana_slash[i].X - GetX() + SCREEN_WIDTH / 2,
							  katana_slash[i].Y - GetY() + SCREEN_HEIGHT / 2,
					1, (3.14 / 180)* katana_angle[i], WeaponEffect[slashanim], true, false, false);
			}
		}
		break;

	default:
		break;
	}
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

	//腕アニメーション
	if (Yinput == Inp_UD::UP)		//上方向入力：上振り
	{
		switch (TurnFlg)
		{
		case true:	//左向き時
			if (Attack < 10)
			{
				Arm_R = { (int)(SCREEN_WIDTH / 2 - (0.5 * Width) + ((0.5 * Width) * 2 / 10 * Attack)), SCREEN_HEIGHT / 2 - Height / 2 - 10 };
				ArmAngle_R = -45 + ((90 / 10) * Attack);
			}
			else if (Attack < 20) 
			{
				Arm_R = { (int)(SCREEN_WIDTH / 2 + (0.5 * Width) - ((0.5 * Width) * 2 / 10 * (Attack - 10))), SCREEN_HEIGHT / 2 - Height / 2 - 10 };
				ArmAngle_R = 45 - ((90 / 10) * (Attack - 10));
			}

			Arm_L = { SCREEN_WIDTH / 2 + 12, SCREEN_HEIGHT / 2 };
			ArmAngle_L = 0;
			break;

		case false:	//右向き時
			if (Attack < 10)
			{
				Arm_R = { (int)(SCREEN_WIDTH / 2 + (0.5 * Width) - ((0.5 * Width) * 2 / 10 * Attack)), SCREEN_HEIGHT / 2 - Height / 2 - 10 };
				ArmAngle_R = 45 - ((90 / 10) * Attack);
			}
			else if (Attack < 20)
			{
				Arm_R = { (int)(SCREEN_WIDTH / 2 - (0.5 * Width) + ((0.5 * Width) * 2 / 10 * (Attack - 10))), SCREEN_HEIGHT / 2 - Height / 2 - 10 };
				ArmAngle_R = -45 + ((90 / 10) * (Attack - 10));
			}

			Arm_L = { SCREEN_WIDTH / 2 + 13, SCREEN_HEIGHT / 2 };
			ArmAngle_L = 0;
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
				if (Attack < 10) 
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 - (0.7 * Width)),
							  (int)(SCREEN_HEIGHT / 2 - Height * 0.7 + ((Height * 1.4) / 10 * Attack)) - 12 };
					ArmAngle_R = 315 - ((90 / 10) * Attack);
				} 
				else 
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 - (0.7 * Width)),
							  (int)(SCREEN_HEIGHT / 2 - Height * 0.7 + ((Height * 1.4) / 20 * 20)) - 12 };
					ArmAngle_R = 315 - 90;
				} 
				break;

			case false:	//右向き時
				if (Attack < 10)
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 + (0.6 * Width)),
							  (int)(SCREEN_HEIGHT / 2 - Height * 0.7 + ((Height * 1.4) / 10 * Attack)) - 12 };
					ArmAngle_R = 45 + ((90 / 10) * Attack);
				}  
				else
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 + (0.6 * Width)),
							  (int)(SCREEN_HEIGHT / 2 - Height * 0.7 + ((Height * 1.4) / 20 * 20)) - 12 };
					ArmAngle_R = 45 + 90;
				} 
				break;

			default:
				break;
			}
			break;

		case 2:					//コンボ２：斬り上げ
			switch (TurnFlg)
			{
			case true:	//左向き時
				if (Attack < 10) 
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 - (0.7 * Width)),
							  (int)(SCREEN_HEIGHT / 2 + Height * 0.8 - ((Height * 1.6) / 10 * Attack)) };
					ArmAngle_R = 225 + ((90 / 10) * Attack);
				}
				else 
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 - (0.7 * Width)),
							  (int)(SCREEN_HEIGHT / 2 + Height * 0.8 - ((Height * 1.6) / 20 * 20)) };
					ArmAngle_R = 315;
				}
				break;

			case false:	//右向き時
				if (Attack < 10) 
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 + (0.7 * Width)),
							  (int)(SCREEN_HEIGHT / 2 + Height * 0.8 - ((Height * 1.6) / 10 * Attack)) };
					ArmAngle_R = 135 - ((90 / 10) * Attack);
				} 
				else
				{
					Arm_R = { (int)(SCREEN_WIDTH / 2 + (0.7 * Width)),
							  (int)(SCREEN_HEIGHT / 2 + Height * 0.8 - ((Height * 1.6) / 20 * 20)) };
					ArmAngle_R = 45;
				}
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

//攻撃：メイス
void Player::MaceAtk()
{

	if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && stat.Power == 0)Attack++;	//ボタン長押し中に力をためる
	else if (PAD_INPUT::OnRelease(XINPUT_BUTTON_B) && stat.Power == 0)		//離すとためた力に応じて強化
	{
		if (Attack < 20 || wall != 0)
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

	
	if (wall != 0 && 20 <= Attack)
	{
		Attack = 20 - 1;
	}
	else if (60 < Attack) 
	{
		Attack = 60;
	}
	
	if (stat.Power)
	{
		Attack -= 1.0;
		if (Attack <= 0)
		{
			Attack = 0;
			stat.Power = 0;
			return;
		}
	}

	//腕のアニメーション
	int Dis = Width;

	if (stat.Power == 0) {
		switch (TurnFlg)
		{
		case true:
			if (Attack < 20)
			{
				ArmAngle_R = 20;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else if (Attack < 40)
			{
				ArmAngle_R = 45;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				ArmAngle_R = 70;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;
		case false:
			if (Attack < 20)
			{
				ArmAngle_R = -20;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else if (Attack < 40)
			{
				ArmAngle_R = -45;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				ArmAngle_R = -70;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
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
					ArmAngle_R = -135 + (155 / 10 * (Attack - 10));

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				else
				{
					ArmAngle_R = -135;

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				break;

			case false:
				if (10 < Attack)
				{
					ArmAngle_R = 135 - (155 / 10 * (Attack - 10));

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				else
				{
					ArmAngle_R = 135;

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				break;
			}

		case 2:
			switch (TurnFlg)
			{
			case true:
				if (10 < Attack)
				{
					ArmAngle_R = -135 + (180 / 10 * (Attack - 10));

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				else
				{
					ArmAngle_R = -135;

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				break;

			case false:
				if (10 < Attack)
				{
					ArmAngle_R = 135 - (180 / 10 * (Attack - 10));

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				else
				{
					ArmAngle_R = 135;

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				break;
			}

		case 3:
			switch (TurnFlg)
			{
			case true:
				if (10 < Attack)
				{
					ArmAngle_R = -135 + (205 / 10 * (Attack - 10));

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				else
				{
					ArmAngle_R = -135;

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				break;

			case false:
				if (10 < Attack)
				{
					ArmAngle_R = 135 - (205 / 10 * (Attack - 10));

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				else
				{
					ArmAngle_R = 135;

					Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
							  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
				}
				break;
			}

		default:
			break;
		}
	}
}

//攻撃：槍
void Player::SpearAtk()
{
	Attack += 0.8;
	if (20 < Attack)
	{
		Attack = 0;
		stat.Power = 0;
		return;
	}
	else stat.Power = 2;

	//	腕のアニメーション
	double thrust = 20;	//攻撃距離
	double Dis = Width; 
	switch (Atkpt)
	{
	case 0:

		ArmAngle_R = spear_angle;
		ArmAngle_L = spear_angle;

		switch (TurnFlg)
		{
		case true:
			if (Attack < 8)
			{
				Dis = thrust * (Attack - 0.5);

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };

				Arm_L = { (int)(SCREEN_WIDTH / 2 + (Dis - thrust) *cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + (Dis - thrust) * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				Dis = thrust * (8 - (Attack - 7.5));
				if (Dis < thrust / 2)Dis = thrust / 2;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };

				Arm_L = { (int)(SCREEN_WIDTH / 2 + (Dis - thrust) * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + (Dis - thrust) * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;
		case false:
			if (Attack < 7)
			{
				Dis = thrust * (Attack - 0.5);

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };

				Arm_L = { (int)(SCREEN_WIDTH / 2 + (Dis - thrust) * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + (Dis - thrust) * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				Dis = thrust * (8 - (Attack - 7.5));
				if (Dis < thrust / 2)Dis = thrust / 2;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };

				Arm_L = { (int)(SCREEN_WIDTH / 2 + (Dis - thrust) * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + (Dis - thrust) * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;

		default:
			break;
		}
		break;

	case 1:

		if (Attack < 11)
		{
			ArmAngle_R = 25 + (130 / 10 * (Attack - 1));
			Dis = Width * 1.5;

			Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
					  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
		}
		else
		{
			ArmAngle_R = 155;
			Dis = Width * 1.5;

			Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
					  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
		}
		break;

	case 2:

		if (Attack < 11)
		{
			ArmAngle_R = -25 - (130 / 10 * (Attack - 1));
			Dis = Width * 1.5;

			Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
					  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
		}
		else
		{
			ArmAngle_R = -155;
			Dis = Width * 1.5;

			Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
					  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
		}
		break;

	case 3:

		if (Attack < 11)
		{
			ArmAngle_R = 115 + (130 / 10 * (Attack - 1));
			Dis = Width * 1.5;

			Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
					  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
		}
		else
		{
			ArmAngle_R = 245;
			Dis = Width * 1.5;

			Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
					  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
		}
		break;

	default:
		break;
	}
}

//攻撃：刀
void Player::KatanaAtk()
{
	Attack += 0.8;
	if (33 < Attack)
	{
		Attack = 0;
		stat.Power = 0;
		Combo = 0;
	}
	else stat.Power = 2;

	if (3 <= Combo && Attack < 18)
	{
		//斬撃の座標を決める
		if (0 <= Near_Enemy.X && NearEneDis <= 250) katana_slash[0] = Near_Enemy;

		else
		{
			if (TurnFlg)katana_slash[0] = { GetX() - 250 , GetY() };
			else katana_slash[0] = { GetX() + 250,GetY() };
		}

		katana_angle[0] = GetRand(360);

		Range base = katana_slash[0];
		for (int i = 1; i < Katana_num; i++)
		{
			katana_slash[i] = { base.X + GetRand(100) - 50,base.Y + GetRand(80) - 40 };
			katana_angle[i] = GetRand(360);
		}

		Search = true;
	}

	//腕アニメーション
	int Dis = 0;
	int thrust = 85;	//攻撃距離

	if (Combo == 1) {
		switch (TurnFlg)
		{
		case true:

			if (Attack < 6)
			{
				ArmAngle_R = -80;
				Dis = -thrust + thrust * 1.6 / 6 * Attack;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				ArmAngle_R = -80;
				Dis = thrust * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;
		case false:

			if (Attack < 6)
			{
				ArmAngle_R = 80;
				Dis = -thrust + thrust * 1.6 / 6 * Attack;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				ArmAngle_R = 80;
				Dis = thrust * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;

		default:
			break;
		}
	}

	if (Combo == 2) {

		switch (TurnFlg)
		{
		case true:

			if (Attack <= 9)
			{
				ArmAngle_R = -140 + (80 / 8 * (Attack - 1));
				Dis = thrust * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				ArmAngle_R = -60;
				Dis = thrust * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;

		case false:

			if (Attack <= 9)
			{
				ArmAngle_R = 140 - (80 / 8 * (Attack - 1));
				Dis = thrust * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			else
			{
				ArmAngle_R = 60;
				Dis = thrust * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (ArmAngle_R - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (ArmAngle_R - 90))) };
			}
			break;

		default:
			break;
		}
	}

	if (Combo == 3) {
		thrust *= 0.6;

		switch (TurnFlg)
		{
		case true:

			if (Attack < 15)
			{
				ArmAngle_R = -60 + (180 / 13 * (Attack - 1));
				Dis = thrust + Attack * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (-60 - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (-60 - 90))) };
			}
			else if (Attack < 18)
			{
				ArmAngle_R = 120;
				Dis = thrust - (thrust / 4 * (Attack - 15)) + (9 - 9 / 4 * (Attack - 15));

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (-60 - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (-60 - 90))) };
			}
			else
			{
				ArmAngle_R = 120;
				Dis = 0;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (-60 - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (-60 - 90))) };
			}
			break;

		case false:

			if (Attack < 15)
			{
				ArmAngle_R = 60 - (180 / 13 * (Attack - 1));
				Dis = thrust + Attack * 0.6;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (60 - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (60 - 90))) };
			}
			else if (Attack < 18)
			{
				ArmAngle_R = -120;
				Dis = thrust - (thrust / 4 * (Attack - 15)) + (9 - 9 / 4 * (Attack - 15));

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (60 - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (60 - 90))) };
			}
			else
			{
				ArmAngle_R = -120;
				Dis = 0;

				Arm_R = { (int)(SCREEN_WIDTH / 2 + Dis * cos((3.14 / 180) * (60 - 90))),
						  (int)(SCREEN_HEIGHT / 2 + Dis * sin((3.14 / 180) * (60 - 90))) };
			}
			break;

		default:
			break;
		}
	}

	if (!wall)
	{
		if (TurnFlg)
		{
			Arm_L = { SCREEN_WIDTH / 2 + 15, SCREEN_HEIGHT / 2 + 5 };
			ArmAngle_L = 25;
		}
		else
		{
			Arm_L = { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 5 };
			ArmAngle_L = -25;
		}
	}
}

//当たり判定：短剣
bool Player::HitDagger(int EneX, int EneY, int EneW, int EneH) {

	if (stat.Power) {
		int WeaponX = 0;
		int WeaponY = 0;
		int RangeX = 0;
		int RangeY = 0;
		float Rad = 0;

		float gap = 0;
		float top = Height * 1.1;

		EneX = EneX - GetX() + SCREEN_WIDTH / 2;
		EneY = EneY - GetY() + SCREEN_HEIGHT / 2;

		if (Yinput == Inp_UD::UP) {
			switch (TurnFlg)
			{
			case true:	//左向き時
				if (Attack < 10) {
					WeaponX = SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * Attack);
					WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2;

					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;

					Rad = (3.14 / 180) * (-45 + ((90 / 10) * Attack));
				}
				else if (Attack < 20)
				{
					WeaponX = SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * (Attack - 10));
					WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2;

					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;

					Rad = (3.14 / 180) * (45 - ((90 / 10) * (Attack - 10)));

				}
				break;
			case false:	//右向き時
				if (Attack < 10)
				{

					WeaponX = SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * Attack);
					WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2;

					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;

					Rad = (3.14 / 180) * (45 - ((90 / 10) * Attack));
				}
				else if (Attack < 20)
				{
					WeaponX = SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * (Attack - 10));
					WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2;

					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;

					Rad = (3.14 / 180) * (-45 + ((90 / 10) * (Attack - 10)));
				}
				break;
			default:
				break;
			}

			int DisX = EneX - WeaponX;
			int DisY = EneY - WeaponY;

			int Dis = sqrt(pow(DisX, 2) + pow(DisY, 2));

			a = EneX;
			b = EneY;
			c = WeaponX;
			d = WeaponY;

			if (WeaponX < EneX + EneW / 2 && WeaponY < EneY + EneH / 2 && EneX - EneW / 2 < WeaponX && EneY - EneH / 2 < WeaponY)
			{
				return true;
			}

		}
		else if (Attack && Attack < 10)
		{

			switch (Combo)
			{
			case 1:
				switch (TurnFlg)
				{
				case true:
					WeaponX = SCREEN_WIDTH / 2 - (1.6 * Width);
					WeaponY = SCREEN_HEIGHT / 2 - top + ((top * 2) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (315 - ((90 / 10) * Attack) + gap);
					break;

				case false:

					WeaponX = SCREEN_WIDTH / 2 + (1.6 * Width);
					WeaponY = SCREEN_HEIGHT / 2 - top + ((top * 2) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (45 + ((90 / 10) * Attack) + gap);
					break;

				default:
					break;
				}
				break;

			case 2:
				switch (TurnFlg)
				{
				case true:
					WeaponX = SCREEN_WIDTH / 2 - (1.6 * Width);
					WeaponY = SCREEN_HEIGHT / 2 + top - ((top * 2.1) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (225 + ((90 / 10) * Attack) + gap);
					break;

				case false:
					WeaponX = SCREEN_WIDTH / 2 + (1.6 * Width);
					WeaponY = SCREEN_HEIGHT / 2 + top - ((top * 2.1) / 10 * Attack);
					RangeX = range[0].X / 2;
					RangeY = range[0].Y / 2;
					Rad = (3.14 / 180) * (135 - ((90 / 10) * Attack) + gap);
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

			int Dis = sqrt(pow(DisX, 2) + pow(DisY, 2));

			a = EneX;
			b = EneY;
			c = WeaponX;
			d = WeaponY;

			if (WeaponX < EneX + EneW / 2 && WeaponY < EneY + EneH / 2 && EneX - EneW / 2 < WeaponX && EneY - EneH / 2 < WeaponY)
			{
				return true;
			}
		}
	}

	return false;
}

//当たり判定：メイス
bool Player::HitMace(int EneX, int EneY, int EneW, int EneH) {

	if (stat.Power)
	{

		EneX = EneX - GetX() + SCREEN_WIDTH / 2;
		EneY = EneY - GetY() + SCREEN_HEIGHT / 2;

		for (int i = 0; i < 2; i++) {
			float size = 0.2;

			double stX = 0, stY = 0;		//振りかぶる前の座標
			double finX = 0, finY = 0;		//振りかぶった後の座標
			double Dis = Width * (2 + i); //メイスの当たり判定
			int RangeX = 0;
			int RangeY = 0;

			float Rad = 0;

			double stAng, finAng = 0;	//振りかぶる角度


			int power = stat.Power;
			switch (power)
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
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					else
					{
						stAng = -90;
						finAng = -135;
						stX = SCREEN_WIDTH / 2;
						stY = SCREEN_HEIGHT / 2;
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

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
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					else
					{
						stAng = 90;
						finAng = 135;
						stX = SCREEN_WIDTH / 2;
						stY = SCREEN_HEIGHT / 2;
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

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
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					else
					{
						stAng = -90;
						finAng = -135;
						stX = SCREEN_WIDTH / 2;
						stY = SCREEN_HEIGHT / 2;
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

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
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					else
					{
						stAng = 90;
						finAng = 135;
						stX = SCREEN_WIDTH / 2;
						stY = SCREEN_HEIGHT / 2;
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

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
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					else
					{
						stAng = -90;
						finAng = -135;
						stX = SCREEN_WIDTH / 2;
						stY = SCREEN_HEIGHT / 2;
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

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
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					else
					{
						stAng = 90;
						finAng = 135;
						stX = SCREEN_WIDTH / 2;
						stY = SCREEN_HEIGHT / 2;
						RangeX = range[1].X / 2;
						RangeY = range[1].Y / 2;

						finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
						finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
					}
					break;
				}

			default:
				break;
			}


			int DisX = EneX - (int)finX;
			int DisY = EneY - (int)finY;

			Dis = sqrt(pow(DisX, 2) + pow(DisY, 2));
			Rad = (3.14 / 180) * (finAng + 135);

			a = EneX;
			b = EneY;
			c = finX;
			d = finY;

			if (finX < EneX + EneW / 2 && finY < EneY + EneH / 2 && EneX - EneW / 2 < finX && EneY - EneH / 2 < finY)
			{
				return true;
			}
		}
	}
	
	return false;
}

//当たり判定：槍
bool Player::HitSpear(int EneX, int EneY, int EneW, int EneH) {


	float size = 0.2;

	double stX = 0, stY = 0;		//振りかぶる前の座標
	double finX = 0, finY = 0;		//振りかぶった後の座標
	double Dis = 0;			//体の中心からの距離
	int RangeX = 0;
	int RangeY = 0;

	double finAng = spear_angle;	//攻撃する角度
	int thrust = 20;	//攻撃距離

	EneX = EneX - GetX() + SCREEN_WIDTH / 2;
	EneY = EneY - GetY() + SCREEN_HEIGHT / 2;

	if (Attack && !wall&& Atkpt==0) {
		//上記の値を計算
		switch (TurnFlg)
		{
		case true:
			if (Attack < 8)
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				RangeX = range[2].X / 2;
				RangeY = range[2].Y / 2;

				Dis = RangeY + (thrust * Attack);

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;
				RangeX = range[2].X / 2;
				RangeY = range[2].Y / 2;

				Dis = RangeY + (thrust * (8 - (Attack - 8)));
				if (Dis < RangeY)return false;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;
		case false:
			if (Attack < 7)
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;

				RangeX = range[2].X / 2;
				RangeY = range[2].Y / 2;

				Dis = RangeY + (thrust * Attack);

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			else
			{
				stX = SCREEN_WIDTH / 2;
				stY = SCREEN_HEIGHT / 2;

				RangeX = range[2].X / 2;
				RangeY = range[2].Y / 2;

				Dis = RangeY + (thrust * (8 - (Attack - 8)));
				if (Dis < RangeY)return false;

				finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
				finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
			}
			break;

		default:
			break;
		}
	}

	if (Attack && Atkpt == 1) {
		if (Attack < 11)
		{
			finAng = 25 + (130 / 10 * (Attack - 1));
			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;

			RangeX = range[2].X / 2;
			RangeY = range[2].Y / 2;

			Dis = range[2].Y;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finAng = 155;

			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;

			RangeX = range[2].X / 2;
			RangeY = range[2].Y / 2;

			Dis = range[2].Y;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
	}

	if (Attack && Atkpt == 2) {
		if (Attack < 11)
		{
			finAng = -25 - (130 / 10 * (Attack - 1));

			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;

			RangeX = range[2].X / 2;
			RangeY = range[2].Y / 2;

			Dis = range[2].Y;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finAng = -155;

			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;

			RangeX = range[2].X / 2;
			RangeY = range[2].Y / 2;

			Dis = range[2].Y;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
	}

	if (Attack && Atkpt == 3)
	{
		if (Attack < 11)
		{
			finAng = 115 + (130 / 10 * (Attack - 1));

			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;

			RangeX = range[2].X / 2;
			RangeY = range[2].Y / 2;

			Dis = range[2].Y;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
		else
		{
			finAng = 245;

			stX = SCREEN_WIDTH / 2;
			stY = SCREEN_HEIGHT / 2;

			RangeX = range[2].X / 2;
			RangeY = range[2].Y / 2;

			Dis = range[2].Y;

			finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
			finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
		}
	}
	if (Attack) {
		int DisX = EneX - finX;
		int DisY = EneY - finY;


		Dis = sqrt(pow(DisX, 2) + pow(DisY, 2));


		a = EneX;
		b = EneY;
		c = finX;
		d = finY;

		if (finX < EneX + EneW / 2 && finY < EneY + EneH / 2 && EneX - EneW / 2 < finX && EneY - EneH / 2 < finY)
		{
			return true;
		}
	}
	return false;
}

//当たり判定：刀
bool Player::HitKatana(int EneX, int EneY, int EneW, int EneH) {

	EneX = EneX - GetX() + SCREEN_WIDTH / 2;
	EneY = EneY - GetY() + SCREEN_HEIGHT / 2;

	for (int i = 0; i < 3; i++) {
		

		float size = 0.2;
		float sizeY = 1;

		double stX = 0, stY = 0;		//振りかぶる前の座標
		double finX = 0, finY = 0;		//振りかぶった後の座標
		double Dis = Width * (2 + i);		//体の中心からの距離
		int RangeX = 0;
		int RangeY = 0;

		double finAng = 0;	//攻撃する角度
		int thrust = 85;	//攻撃距離

		//画像サイズ取得
		int imgX;
		int imgY;

		int KScope = 0;  //斬撃範囲

		GetGraphSize(Weapon[3], &imgX, &imgY);
		//上記の値を計算

		if (Combo == 1) {
			switch (TurnFlg)
			{
			case true:

				if (Attack < 6)
				{
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;

					RangeX = range[3].X / 2;
					RangeY = range[3].Y / 2;

					Dis = (-thrust + thrust * 2 / 6 * Attack) + (imgY / 2 * size);
					sizeY = -1 + 2 / 6 * Attack;

					finAng = -80;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

				}
				else
				{
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;

					RangeX = range[3].X / 2;
					RangeY = range[3].Y / 2;

					Dis = thrust;
					sizeY = 1;

					finAng = -80;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

				}
				break;

			case false:
				if (Attack < 6)
				{
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;

					RangeX = range[3].X / 2;
					RangeY = range[3].Y / 2;

					Dis = (-thrust + thrust * 2 / 6 * Attack) + (imgY / 2 * size);
					sizeY = -1 + 2 / 6 * Attack;

					finAng = 80;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));

				}
				else
				{
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;

					RangeX = range[3].X / 2;
					RangeY = range[3].Y / 2;

					Dis = thrust;
					sizeY = 1;

					finAng = 80;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
			default:
				break;
			}
		}

		if (Combo == 2) {
			switch (TurnFlg)
			{
			case true:
				if (Attack <= 9)
				{
					finAng = -140 + (90 / 9 * (Attack - 1));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;

					RangeX = range[3].X / 2;
					RangeY = range[3].Y / 2;

					Dis = thrust + (imgY / 2 * size) - 30 * i;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					return false;
				}
			default:
				break;

			case false:
				if (Attack <= 9)
				{
					finAng = 140 - (90 / 9 * (Attack - 1));
					stX = SCREEN_WIDTH / 2;
					stY = SCREEN_HEIGHT / 2;

					RangeX = range[3].X / 2;
					RangeY = range[3].Y / 2;

					Dis = thrust + (imgY / 2 * size) - 30 * i;

					finX = stX + Dis * cos((3.14 / 180) * (finAng - 90));
					finY = stY + Dis * sin((3.14 / 180) * (finAng - 90));
				}
				else
				{
					return false;
				}
				break;
			}
		}

		if (Combo == 3) {
			thrust = thrust - imgY / 2 * size;

			switch (TurnFlg)
			{
			case true:
				if (Attack < 15)
				{
					
				}
				else if (Attack < 18)
				{
					
				}
				else
				{
					//斬撃範囲
					KScope = 40;

					Dis = 0;

					finX = katana_slash[0].X - GetX() + SCREEN_WIDTH / 2;
					finY = katana_slash[0].Y - GetY() + SCREEN_HEIGHT / 2;
				}
			default:
				break;

			case false:
				if (Attack < 15)
				{
					
				}
				else if (Attack < 18)
				{
					
				}
				else
				{
					//斬撃範囲

					KScope = 40;

					Dis = 0;

					finX = katana_slash[0].X - GetX() + SCREEN_WIDTH / 2;
					finY = katana_slash[0].Y - GetY() + SCREEN_HEIGHT / 2;
				}
				break;
			}


		}

			int DisX = EneX - finX;
			int DisY = EneY - finY;

			a = EneX;
			b = EneY;

			if (i == 0) {
				c = finX;
				d = finY;
			}

			if (i == 2) {
				e = finX;
				f = finY;
			}

			if (finX - KScope < EneX + EneW / 2 && finY - KScope < EneY + EneH / 2 && EneX - EneW / 2 < finX + KScope && EneY - EneH / 2 < finY + KScope)
			{
				return true;
			}

		}
		return false;
}

//近くの敵の座標をセット
void Player::SetNear(int X, int Y, int Dis)
{
	Search = false;
	Near_Enemy = { X,Y };
	NearEneDis = Dis;
}

//敵からの攻撃
void Player::HitEnemy(float damage)
{
	if (!HitCool && !Dodgespd)
	{
		stat.Hp -= damage;
		if (stat.Hp < 0)stat.Hp = 0;
		HitCool = 30;
	}
}
