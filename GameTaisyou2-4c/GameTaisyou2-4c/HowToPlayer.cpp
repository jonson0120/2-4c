#include "HowToPlayer.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"
#include <math.h>

HowToPlayer::HowToPlayer() {
	Walk = 0;

	stat.Hp = 10;
	stat.MaxHp = stat.Hp;
	stat.Atk = 1;
	stat.Power = 0;
	stat.Shard = 0;

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
	Equip[0] = weapons::NONE;
	Equip[1] = weapons::NONE;

	EquipNum = 0;

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

void HowToPlayer::Update() {
	InitPad();

	float Maxspeed = speedinit;	//最大速度
	float CorSpeed = 1;			//移動速度補正

	//壁面移動・Aボタン長押しで処理に入る
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_A) || (wall && PAD_INPUT::OnPressed(XINPUT_BUTTON_A)) ||
											   (jump && PAD_INPUT::OnPressed(XINPUT_BUTTON_A) && JoypadY >= MARGIN * 2))
	{
		//壁面移動・左壁
		//壁面移動中か左側が壁なら入る
		if ((wall == 1 || !MapData[y / 160][(x - 1 - Width / 2) / 160]))
		{
			if ((JoypadX <= -MARGIN && !Attack && PAD_INPUT::OnClick(XINPUT_BUTTON_A)) || wall == 3) {
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
			if ((MARGIN <= JoypadX && !Attack && PAD_INPUT::OnClick(XINPUT_BUTTON_A)) || wall == 3 ) {
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
			if (Equip[EquipNum] == weapons::NONE)
			{
				if (1 < ++EquipNum) EquipNum = 0;
			}
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

void HowToPlayer::Draw() const {

	//for (int i = 0; i < MAP_HEIGHT; i++)
	//{
	//	for (int j = 0; j < MAP_WIDTH_T; j++)
	//	{
	//		if (GetY() / 160 == i && GetX() / 160 == j) DrawFormatString(50 + 15 * j, 170 + 15 * i, 0xff0000, "9");
	//		else DrawFormatString(50 + 15 * j, 170 + 15 * i, 0xffffff, "%d", MapData[i][j]);
	//	}
	//}
	//

	//攻撃描画：プレイヤー後方
	if (Attack) 
	{
		switch (Equip[EquipNum])
		{
		case weapons::dagger:	//短剣
			DrawDagger();
			break;

		default:
			break;
		}
	}

	int cameraY = 150;

	if (HitCool % 4 < 2)
	{
		//腕：プレイヤー後方
		if (TurnFlg || wall == 1 || wall == 3)
		{
			DrawRotaGraph(Arm_R.X, Arm_R.Y + cameraY , 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);
		}
		else if (wall == 0)
		{
			DrawRotaGraph(Arm_L.X, Arm_L.Y + cameraY, 1, (3.14 / 180) * ArmAngle_L, ArmImg, true, true);
		}

		if (Attack && Equip[EquipNum] == weapons::katana)
			DrawRotaGraph(Arm_R.X, Arm_R.Y + cameraY, 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);

		//プレイヤー本体

		//上半身
		if (wall)
		{
			if (wall == 1)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4 + cameraY, 1.0f, 0, PImages[2], TRUE, true);
			if (wall == 2)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4 + cameraY, 1.0f, 0, PImages[1], TRUE, false);
			if (wall == 3)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4 + cameraY, 1.0f, 0, PImages[3 + TurnFlg], TRUE, TurnFlg);
		}
		else DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - Height / 4 + cameraY, 1.0f, 0, PImages[0], TRUE, TurnFlg);

		//下半身
		if (wall)
		{
			if (wall == 1)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4 + cameraY, 1.0f, 0, image_U[6], TRUE, true);
			if (wall == 2)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4 + cameraY, 1.0f, 0, image_U[5], TRUE, false);
			if (wall == 3)DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4 + cameraY, 1.0f, 0, image_U[5 + TurnFlg], TRUE, TurnFlg);
		}
		else if (MapData[(y + Height / 2 + 1) / 160][(x - Width / 2) / 160] &&
			MapData[(y + Height / 2 + 1) / 160][(x + Width / 2) / 160]) {
			DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4 + cameraY, 1.0f, 0, image_U[4], TRUE, TurnFlg);
		}
		else
		{
			DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + Height / 4 + cameraY, 1.0f, 0, image_U[Walk / 10], TRUE, TurnFlg);
		}

		//腕：プレイヤー前面
		if (TurnFlg)
		{
			if (wall == 0)DrawRotaGraph(Arm_L.X, Arm_L.Y + cameraY, 1, (3.14 / 180) * ArmAngle_L, ArmImg, true, true);
		}
		else
		{
			if (wall != 1 && (!Attack || Equip[EquipNum] != weapons::katana))
				DrawRotaGraph(Arm_R.X, Arm_R.Y + cameraY, 1, (3.14 / 180) * ArmAngle_R, ArmImg, true, false);
		}

	}

	//DrawCircle(a, b, 3.0f, 0xff0000, true);
	//DrawCircle(c, d, 3.0f, 0x00ff00, true);
	//DrawCircle(e, f, 3.0f, 0x00ff00, true);
}

void HowToPlayer::Spawn() {
	x = BLOCK_SIZE * 10 + BLOCK_SIZE / 2;
	y = BLOCK_SIZE * (MAP_HEIGHT - 3);

	y += BLOCK_SIZE - Height / 2;
}

void HowToPlayer::InitPad() {

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
void HowToPlayer::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH_T]) {

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH_T; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}
}

//攻撃描画：短剣
void HowToPlayer::DrawDagger()const
{
	float size = 0.3;

	if (Yinput == Inp_UD::UP)		//上方向入力：上振り
	{
		switch (TurnFlg)
		{
		case true:	//左向き時
			if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * Attack), SCREEN_HEIGHT / 2 - Height + 150,
				size, (3.14 / 180) * (-45 + ((90 / 10) * Attack)), Weapon[0], true, false);
			else if (Attack < 20) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * (Attack - 10)), SCREEN_HEIGHT / 2 - Height + 150,
				size, (3.14 / 180) * (45 - ((90 / 10) * (Attack - 10))), Weapon[0], true, true);
			break;

			case false:	//右向き時
			if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * Attack), SCREEN_HEIGHT / 2 - Height + 150,
				size, (3.14 / 180) * (45 - ((90 / 10) * Attack)), Weapon[0], true, true);
			else if (Attack < 20) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * (Attack - 10)), SCREEN_HEIGHT / 2 - Height + 150,
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
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 11 * Attack) + 150,
					size, (3.14 / 180) * (315 - ((90 / 10) * Attack)), Weapon[0], true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20) + 150,
					size, (3.14 / 180) * (315 - 90), Weapon[0], true, true);
				break;

			case false:	//右向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 11 * Attack) + 150,
					size, (3.14 / 180) * (45 + ((90 / 10) * Attack)), Weapon[0], true, false);
				else DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20) + 150,
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
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack) + 150,
					size, (3.14 / 180) * (225 + ((90 / 10) * Attack)), Weapon[0], true, false);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 20 * 20) + 150,
					size, (3.14 / 180) * (315), Weapon[0], true, false);
				break;

			case false:	//右向き時
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack) + 150,
					size, (3.14 / 180) * (135 - ((90 / 10) * Attack)), Weapon[0], true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 20 * 20) + 150,
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

//攻撃：短剣
void HowToPlayer::DaggerAtk()
{
	Attack++;
	stat.Power = 2;
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


//当たり判定：短剣
bool HowToPlayer::HitDagger(int EneX, int EneY, int EneW, int EneH) {

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

		for (int i = 0; i < 3; i++) {
			if (Yinput == Inp_UD::UP) {
				switch (TurnFlg)
				{
				case true:	//左向き時
					if (Attack < 10) {
						WeaponX = SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * Attack);
						WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2 - (10 * i);

						RangeX = range[0].X / 2;
						RangeY = range[0].Y / 2;

						Rad = (3.14 / 180) * (-45 + ((90 / 10) * Attack));
					}
					else if (Attack < 20)
					{
						WeaponX = SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * (Attack - 10));
						WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2 - (10 * i);

						RangeX = range[0].X / 2;
						RangeY = range[0].Y / 2;

						Rad = (3.14 / 180) * (45 - ((90 / 10) * (Attack - 10)));

					}
					break;
				case false:	//右向き時
					if (Attack < 10)
					{

						WeaponX = SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * Attack);
						WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2 - (10 * i);

						RangeX = range[0].X / 2;
						RangeY = range[0].Y / 2;

						Rad = (3.14 / 180) * (45 - ((90 / 10) * Attack));
					}
					else if (Attack < 20)
					{
						WeaponX = SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * (Attack - 10));
						WeaponY = SCREEN_HEIGHT / 2 - Height - range[0].Y / 2 - (10 * i);

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
						WeaponX = SCREEN_WIDTH / 2 - (1.6 * Width) + (10 * i);
						WeaponY = SCREEN_HEIGHT / 2 - top + ((top * 2) / 10 * Attack);
						RangeX = range[0].X / 2;
						RangeY = range[0].Y / 2;
						Rad = (3.14 / 180) * (315 - ((90 / 10) * Attack) + gap);
						break;

					case false:

						WeaponX = SCREEN_WIDTH / 2 + (1.6 * Width) - (10 * i);
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
						WeaponX = SCREEN_WIDTH / 2 - (1.6 * Width) + (10 * i);
						WeaponY = SCREEN_HEIGHT / 2 + top - ((top * 2.1) / 10 * Attack);
						RangeX = range[0].X / 2;
						RangeY = range[0].Y / 2;
						Rad = (3.14 / 180) * (225 + ((90 / 10) * Attack) + gap);
						break;

					case false:
						WeaponX = SCREEN_WIDTH / 2 + (1.6 * Width) - (10 * i);
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
	}

	return false;
}

//近くの敵の座標をセット
void HowToPlayer::SetNear(int X, int Y, int Dis)
{
	Search = false;
	Near_Enemy = { X,Y };
	NearEneDis = Dis;
}

//敵からの攻撃
void HowToPlayer::HitEnemy(float damage)
{
	if (!HitCool && !Dodgespd)
	{
		stat.Hp -= damage;
		if (stat.Hp < 0)stat.Hp = 0;
		HitCool = 30;
	}
}
