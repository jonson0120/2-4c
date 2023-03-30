#include "TreasureBox.h"
#include"DxLib.h"
#include"PadInput.h"
#include"common.h"
#include"GameMainScene.h"

TreasureBox::TreasureBox()
{

	//ïÛî†âÊëú
	lidImage= LoadGraph("images/Treasure2.png", TRUE);     //äWâÊëú
	BoxImage= LoadGraph("images/Treasure3.png", TRUE);     //î†âÊëú

	//äWÇÃx,yç¿ïW
	lidx = 0;
	lidy = 0;    

	//î†ÇÃx,yç¿ïW
	Boxx = 0;
	Boxy = 0;    

	Width = 64;
	Height = 64;

	MapData[lidx][lidy];
	MapData[Boxx][Boxy];

	MakeTreasureBox = FALSE;

	lidflag = 0;

	fall = 14;

	ButtonFlag = false;

	
}

void TreasureBox::Update(Player*player)
{
	
	float fallinit = 14;
	

	if (ButtonFlag == false)
	{

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

	
	/*if (ButtonFlag == true)
	{
		lidflag--;
	}

	if (lidflag < -40)
	{
		lidflag = -40;
	}
	*/

	if (PAD_INPUT::OnClick(XINPUT_BUTTON_X))
	{
		ButtonFlag = true;
	}
}

void TreasureBox::Draw(int x,int y)const
{
	   
	if (MakeTreasureBox == TRUE)
	{
		DrawRotaGraph(Boxx - x + (SCREEN_WIDTH / 2), Boxy - y + (SCREEN_HEIGHT / 2), 0.2, 0, BoxImage, TRUE);
		DrawRotaGraph(lidx - x + (SCREEN_WIDTH / 2), lidy - y + (SCREEN_HEIGHT / 2)+(lidflag/2), 0.2, 0, lidImage, TRUE);
	}
	if (ButtonFlag == true)
	{
		DrawString(0, 500, "TRUE",0xffffff);
	}

}
void TreasureBox::MakeBox()
{
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

	MakeBox();
}
