﻿#include "Player.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"

Player::Player() {
	image = 0;

	x = 500;
	y = 500;

	Width = 32;
	Height = 32;

	speed = 8;
	fall = 16;
	jump = 0;

	JoypadX = 0;
	JoypadY = 0;

}

void Player::Update() {
	InitPad();

		//横移動
		if (JoypadX >= MARGIN) {
			x += speed;
		}
		if (JoypadX <= -MARGIN) {
			x -= speed;
		}
		while (!MapData[y / 160][(x + Width / 2) / 160])x--;
		while (!MapData[y / 160][(x - Width / 2) / 160])x++;

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

		while (!MapData[(y - Width / 2) / 160][(x - Width / 2) / 160] ||
			   !MapData[(y - Width / 2) / 160][(x + Width / 2) / 160])
		{
			y++;
			if (0 > fall)fall = 0;
		}

		while (!MapData[(y + Width / 2) / 160][(x - Width / 2) / 160] ||
			   !MapData[(y + Width / 2) / 160][(x + Width / 2) / 160])
		{
			y--;
			jump = 0;
			if (fall > 0)fall = 0;
		}

}

void Player::Draw() const {
	DrawBoxAA(SCREEN_WIDTH / 2 - (Width / 2), SCREEN_HEIGHT / 2 - (Height / 2),
			  SCREEN_WIDTH / 2 + (Width / 2), SCREEN_HEIGHT / 2 + (Height / 2), 0xff0000, TRUE);

	DrawFormatString(0, 30, 0xffffff, "%d", GetX());
	DrawFormatString(0, 45, 0xffffff, "%d", GetY());

	DrawFormatString(0, 60, 0xffffff, "%d", (int)fall);


	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (GetY() / 160 == i && GetX() / 160 == j) DrawFormatString(50 + 15 * j, 50 + 15 * i, 0xff0000, "9");
			else DrawFormatString(50 + 15 * j, 50 + 15 * i, 0xffffff, "%d", MapData[i][j]);
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