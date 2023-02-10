﻿#include "Player.h"
#include "Enemy.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"
#include <math.h>

Player::Player() {
	image = 0;

	x = 500;
	y = 500;

	Width = 32;
	Height = 48;

	speed = 8;
	fall = 16;
	jump = 0;

	Attack = 0;
	Equip = dagger;
	Yinput = NONE;
	Combo = 0;
	range[0] = { 24,44 };

    LoadDivGraph("images/Player.png", 2, 36, 52, 72, 104, PImages);
	Weapon = LoadGraph("images/Dagger.png");

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
			if (Attack < 1)Yinput = UP;
		}
		else if (JoypadY <= -MARGIN * 2.5) 
		{
			if (Attack < 1)Yinput = DOWN;
		}
		else 
		{
			if (Attack < 1)Yinput = NONE;
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
		if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
		{
			switch (Equip)
			{
			case dagger:
				if (Combo == 0)
				{
					Attack++;
					Combo++;
				}
				else if (Combo == 1 && 10 < Attack && Yinput != UP)
				{
					Attack = 1;
					Combo++;
				}
				break;

			default:
				break;
			}

		}

		if (Attack) 
		{
			switch (Equip)
			{
			case dagger:
				DaggerAtk();
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

	DrawFormatString(0, 30, 0xffffff, "%d", GetX());
	DrawFormatString(0, 45, 0xffffff, "%d", GetY());

	DrawFormatString(0, 60, 0xffffff, "%d", Yinput);


	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (GetY() / 160 == i && GetX() / 160 == j) DrawFormatString(50 + 15 * j, 50 + 15 * i, 0xff0000, "9");
			else DrawFormatString(50 + 15 * j, 50 + 15 * i, 0xffffff, "%d", MapData[i][j]);
		}
	}

	if (Attack) 
	{
		switch (Equip)
		{
		case dagger:
			DrawDagger();
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
	if (Yinput == UP) 
	{
		switch (TurnFlg)
		{
		case true:
			if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * Attack), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (-45 + ((90 / 10) * Attack)), Weapon, true, false);
			else if (Attack < 20) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * (Attack - 10)), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (45 - ((90 / 10) * (Attack - 10))), Weapon, true, true);
			break;

			case false:
			if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width) - ((1.2 * Width) * 2 / 10 * Attack), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (45 - ((90 / 10) * Attack)), Weapon, true, true);
			else if (Attack < 20) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width) + ((1.2 * Width) * 2 / 10 * (Attack - 10)), SCREEN_HEIGHT / 2 - Height,
				size, (3.14 / 180) * (-45 + ((90 / 10) * (Attack - 10))), Weapon, true, false);	
			break;
			default:
				break;
			}

	}
	else
	{
		switch (Combo)
		{
		case 1:
			switch (TurnFlg)
			{
			case true:
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 10 * Attack),
					size, (3.14 / 180) * (315 - ((90 / 10) * Attack)), Weapon, true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20),
					size, (3.14 / 180) * (315 - 90), Weapon, true, true);
				break;

			case false:
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 10 * Attack),
					size, (3.14 / 180) * (45 + ((90 / 10) * Attack)), Weapon, true, false);
				else DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 - Height + ((Height * 2) / 20 * 20),
					size, (3.14 / 180) * (45 + 90), Weapon, true, false);
				break;

			default:
				break;
			}
			break;

		case 2:
			switch (TurnFlg)
			{
			case true:
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack),
					size, (3.14 / 180) * (225 + ((90 / 10) * Attack)), Weapon, true, false);
				else DrawRotaGraph(SCREEN_WIDTH / 2 - (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 20 * 20),
					size, (3.14 / 180) * (315), Weapon, true, false);
				break;

			case false:
				if (Attack < 10) DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 10 * Attack),
					size, (3.14 / 180) * (135 - ((90 / 10) * Attack)), Weapon, true, true);
				else DrawRotaGraph(SCREEN_WIDTH / 2 + (1.2 * Width), SCREEN_HEIGHT / 2 + Height - ((Height * 2.1) / 20 * 20),
					size, (3.14 / 180) * (45), Weapon, true, true);
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
void Player::DaggerAtk() 
{
	Attack++;
	if (Yinput == UP)
	{
		if (20 < Attack) {
			Attack = 0;
			Combo = 0;
		}
	}
	else if (Combo == 1)
	{
		if (35 < Attack)
		{
			Attack = 0;
			Combo = 0;
		}
	}
	else if(20 < Attack)
	{
		Attack = 0;
		Combo = 0;
	}
}

bool Player::HitAttack(int EneX, int EneY, int EneW, int EneH) {
	if (Attack) 
	{

	}

	return false;
}