#include "Player.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"

Player::Player() {
	image = 0;

	x = 500;
	y = 500;

	speed = 8;

	JoypadX = 0;
	JoypadY = 0;

}

void Player::Update() {
	InitPad();

	float Gravity = 16;

		if (JoypadX >= MARGIN) {
			x += speed;
		}
		if (JoypadX <= -MARGIN) {
			x -= speed;
		}

		//y += Gravity;
}

void Player::Draw() const {
	DrawBoxAA(SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 16, SCREEN_WIDTH / 2 + 16, SCREEN_HEIGHT / 2 + 16, 0xff0000, TRUE);
	DrawFormatString(0, 30, 0xffffff, "%d", GetX());
	DrawFormatString(0, 45, 0xffffff, "%d", GetY());

	DrawFormatString(0, 75, 0xffffff, "%d", GetX() / 160);


	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			DrawFormatString(50 + 10 * j, 50 + 10 * i, 0xffffff, "%d", MapData[i][j]);
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