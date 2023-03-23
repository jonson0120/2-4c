#pragma once
#include "Player.h"
#include "PadInput.h"

class TreasureBox : public Player
{
private:

	////�󔠉摜
	//int OnBoxImage;
	//int UnderBoxImage;

	//�󔠂̈ʒu
	int OnBox;
	int UnderBox;

	
	bool ButtonFlag;

public:
	TreasureBox();
	void Update();
	void Draw()const;

};

