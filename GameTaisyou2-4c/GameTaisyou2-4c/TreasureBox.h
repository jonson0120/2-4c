#pragma once
#include "Player.h"
#include "PadInput.h"
#include"common.h"

class TreasureBox : public Player
{
private:

	//�󔠉摜
	int lidImage;     //�W�摜
	int BoxImage;     //���摜

	//�󔠂̈ʒu
	int lidx, lidy;    //�W��x,y���W
	int Boxx, Boxy;    //����x,y���W
	int Width, Height;	//�G�̍���
	bool MakeTreasureBox;

	int MapData[MAP_HEIGHT][MAP_WIDTH];

	bool ButtonFlag;

	float fall;

	int lidflag;

public:

	TreasureBox();
	void Update(Player* player);
	void Draw(int x, int y)const;

	int Box_GetX()const { return Boxx; }
	int Box_GetY()const { return Boxy; }

	int lid_GetX()const { return lidx; }
	int lid_GetY()const { return lidy; }

	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }


	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
	void MakeBox();

};

