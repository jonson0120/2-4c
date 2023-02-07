#pragma once
#include"DxLib.h"
#include"common.h"

class Player
{
private:
	int image;	//�摜

	int x, y;		//X,Y���W
	int Width, Height;	//�v���C���[����

	float speed;	//�ړ����x
	float fall;	//�������x
	int jump;	//�W�����v��

	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^

	int JoypadX, JoypadY;	//�p�b�h���͒l

public:

	void InitPad();
	Player();
	void Update();
	void Draw() const;

	int GetX()const { return x; }
	int GetY()const { return y; }

	int GetWidth()const { return Width; }
	int GetHeight()const { return Height; }

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);

};

