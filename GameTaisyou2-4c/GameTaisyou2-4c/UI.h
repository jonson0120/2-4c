#pragma once
#include "Player.h"
#include"PadInput.h"
#include"Player.h"


class UI
{
private:

	Player player;

	int MaxHP;
	float PlayerHP;

	bool potionflag;

	int Damage;

	int Weapon;

	int DaggerImage;
	int MaceImage;
	int SpearImage;
	int KatanaImage;
	 

	int PotionImage1;
	int PotionImage2;
	int PotionImage3;

	
	int PotionCount;
public:
	
	UI();
	void Update();
	void Draw()const;
};

