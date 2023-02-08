#include "Player.h"
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

    LoadDivGraph("images/Player.png", 2, 36, 52, 72, 104, PImages);

	JoypadX = 0;
	JoypadY = 0;

	TurnFlg=false;
}

void Player::Update() {
	InitPad();

		//横移動
		if (JoypadX >= MARGIN) {
			x += speed;
			while (!MapData[y / 160][(x + Width / 2) / 160])x--;
			TurnFlg = FALSE;
		}
		if (JoypadX <= -MARGIN) {
			x -= speed;
			while (!MapData[y / 160][(x - Width / 2) / 160])x++;
			TurnFlg = TRUE;
		}

		//落下とジャンプ
		float fallinit = 16;
		y += fall;

		if (PAD_INPUT::OnClick(XINPUT_BUTTON_A))
		{
			if (jump < 1)
			{
				fall = -fallinit;
				jump++;
			}
		}

		if (fall != fallinit) 
		{
			fall += (fallinit * 2) / 45;
			if (fall > fallinit)
			{
				fall = fallinit;
			}
		}

		while (!MapData[(y + Height / 2) / 160][(x - Width / 2) / 160])
		{
			y--;
			jump = 0;
		}
}

void Player::Draw() const {
	DrawRotaGraph(SCREEN_WIDTH / 2 - (Width / 2), SCREEN_HEIGHT / 2 - (Height / 2)-10 , 1.0f, 0, PImages[0], TRUE, TurnFlg);
	//DrawBoxAA(SCREEN_WIDTH / 2 - (Width / 2) , SCREEN_HEIGHT / 2 - (Height / 2), 
			  //SCREEN_WIDTH / 2 + (Width / 2), SCREEN_HEIGHT / 2 + (Height / 2), 0xff0000, TRUE);

	DrawFormatString(0, 30, 0xffffff, "%d", GetX());
	DrawFormatString(0, 45, 0xffffff, "%d", GetY());


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