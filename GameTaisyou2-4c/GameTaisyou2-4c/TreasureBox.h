#pragma once
#include "Player.h"
#include "PadInput.h"
#include"common.h"

class TreasureBox : public Player
{
private:

	//
	int lidImage;	//
	int BoxImage;	//
	int Button;		//

	//
	int lidx, lidy;    //
	int Boxx, Boxy;    //
	int Width, Height;	//
	bool MakeTreasureBox;

	int MapData[MAP_HEIGHT][MAP_WIDTH];

	bool CanOpen;		//
	bool OpenBoxflag;

	float fall;

	int lidflag;

	int figure;

	bool DropFlag;
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

	bool DropItem();
};

