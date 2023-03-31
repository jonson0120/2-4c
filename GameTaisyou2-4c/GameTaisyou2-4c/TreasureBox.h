#pragma once
#include "Player.h"
#include "PadInput.h"
#include"common.h"

class TreasureBox : public Player
{
private:

	//•ó” ‰æ‘œ
	int lidImage;     //ŠW‰æ‘œ
	int BoxImage;     //” ‰æ‘œ

	//•ó” ‚ÌˆÊ’u
	int lidx, lidy;    //ŠW‚Ìx,yÀ•W
	int Boxx, Boxy;    //” ‚Ìx,yÀ•W
	int Width, Height;	//“G‚Ì‚‚³
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

