#include "TreasureBox.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"
#include"GameMainScene.h"
#include<cstdlib>
TreasureBox::TreasureBox()
{

	//宝箱画像
	lidImage = LoadGraph("images/Treasure2.png", TRUE);     //蓋画像
	BoxImage = LoadGraph("images/Treasure3.png", TRUE);     //箱画像

	//蓋のx,y座標
	lidx = 0;
	lidy = 0;

	//箱のx,y座標
	Boxx = 0;
	Boxy = 0;

	Width = 64;
	Height = 64;

	MapData[lidx][lidy];
	MapData[Boxx][Boxy];

	MakeTreasureBox = FALSE;

	lidflag = 0;

	fall = 12;

	OpenBoxflag=false;

	DropFlag = false;
	/*figure = rand % 100 + 1;*/
}

void TreasureBox::Update(Player* player)
{

	float fallinit = 12;

	
		if (fall < fallinit)
		{
			fall += (fallinit * 2) / 45;
			if (fall > fallinit)
			{
				fall = fallinit;
			}
		}
		Boxy += fall;
		lidy += fall;



		while ((!MapData[(lidy - Height / 4) / BLOCK_SIZE][(lidx - Width / 2) / BLOCK_SIZE]) ||
			(!MapData[(lidy - Height / 4) / BLOCK_SIZE][(lidx + Width / 2) / BLOCK_SIZE]))
		{
			lidy++;
			fall = 0;

		}

		while ((!MapData[(lidy + Height / 4) / BLOCK_SIZE][(lidx - Width / 2) / BLOCK_SIZE]) ||
			(!MapData[(lidy + Height / 4) / BLOCK_SIZE][(lidx + Width / 2) / BLOCK_SIZE]))
		{
			lidy--;

		}
	

	//------------------------------------------------------------------
	while ((!MapData[(Boxy - Height / 5) / BLOCK_SIZE][(Boxx - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(Boxy - Height / 5) / BLOCK_SIZE][(Boxx + Width / 2) / BLOCK_SIZE]))
	{
		Boxy++;
		fall = 0;

	}

	while ((!MapData[(Boxy + Height / 5) / BLOCK_SIZE][(Boxx - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(Boxy + Height / 5) / BLOCK_SIZE][(Boxx + Width / 2) / BLOCK_SIZE]))
	{
		Boxy--;

	}
	//プレイヤー認識範囲
	if (lidx + BLOCK_SIZE * 0.4 >= player->GetX() && lidx - BLOCK_SIZE * 0.4 <= player->GetX() &&
		lidy + BLOCK_SIZE >= player->GetY() && lidy - BLOCK_SIZE <= player->GetY() && !OpenBoxflag)
	{
		if (PAD_INPUT::OnClick(XINPUT_BUTTON_Y))
		{
			OpenBoxflag = true;
		}
	}

	{//宝箱
		if (OpenBoxflag == false)
		{
			lidflag = 0;
		}

		if (OpenBoxflag == true)
		{
			lidflag--;
			if (lidflag < -40)
			{
				lidflag = -40;


			}
			
		}
	}
	

	
}

void TreasureBox::Draw(int x, int y)const
{

	if (MakeTreasureBox == TRUE)
	{
		DrawRotaGraph(Boxx - x + (SCREEN_WIDTH / 2), Boxy - y + (SCREEN_HEIGHT / 2), 0.3, 0, BoxImage, TRUE);
		DrawRotaGraph(lidx - x + (SCREEN_WIDTH / 2), lidy - y + (SCREEN_HEIGHT / 2) +(lidflag/2), 0.3, 0, lidImage, TRUE);
	}
	if (OpenBoxflag == true)
	{
		DrawString(0, 500, "TRUE", 0xffffff);
	}

}
void TreasureBox::MakeBox()
{
	OpenBoxflag = false;

	MakeTreasureBox = FALSE;
	while (MakeTreasureBox == FALSE)
	{
		int i = GetRand(11) + 2;
		int j = GetRand(9) + 1;
		if (MapData[j][i] == 1 && MapData[j + 1][i] == 0)
		{
			lidx = i * BLOCK_SIZE + BLOCK_SIZE / 2;
			lidy = j * BLOCK_SIZE + BLOCK_SIZE / 2;

			Boxx = i * BLOCK_SIZE + BLOCK_SIZE / 2;
			Boxy = j * BLOCK_SIZE + BLOCK_SIZE / 2;

			MakeTreasureBox = TRUE;	
		}
	}
}

void TreasureBox::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}

	DropFlag = false;
	MakeBox();
}

bool TreasureBox::DropItem()
{
	if (-40 == lidflag && !DropFlag)
	{
		DropFlag = true;
		return true;
	}
	else 
	{
		return false;
	}
}
