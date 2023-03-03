#pragma once
#include "Player.h"
#include"PadInput.h"

class UI : public Player
{
private:
	int MaxHP;
	float PlayerHP;

	bool flag;

	int Damage;

	int PotionImage1;
	int PotionImage2;
	int PotionImage3;

	int DaggerImage;

	int PotionCount;
public:
	
	UI();
	void Update();
	void Draw()const;

};

