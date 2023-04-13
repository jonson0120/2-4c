#pragma once
#include "Player.h"
#include "PadInput.h"
#include"common.h"

class TreasureBox : public Player
{
private:

	//宝箱画像
	int lidImage;	//蓋画像
	int BoxImage;	//箱画像
	int Button;		//ボタン画像

	//宝箱の位置
	int lidx, lidy;    //蓋のx,y座標
	int Boxx, Boxy;    //箱のx,y座標
	int Width, Height;	//敵の高さ
	bool MakeTreasureBox;

	int MapData[MAP_HEIGHT][MAP_WIDTH];

	bool CanOpen;		//宝箱を開けるか
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

