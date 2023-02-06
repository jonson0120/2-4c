#pragma once
#include"DxLib.h"
#include"common.h"

class Player
{
private:
	int image;	//�摜

	int x, y;		//X,Y���W
	float speed;	//�ړ����x

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

	int JoypadX, JoypadY;	//�p�b�h���͒l

public:

	void InitPad();
	Player();
	void Update();
	void Draw() const;

	int GetX()const { return (int)x; }
	int GetY()const { return (int)y; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

};

