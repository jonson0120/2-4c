#pragma once
#include "Player.h"
#include"PadInput.h"



class UI : public Player
{
private:
	int MaxHP;
	float PlayerHP;

	bool potionflag;

	int Damage;

	enum weapons Equip;

	int DaggerImage;
	int MaceImage;
	 

	int PotionImage1;
	int PotionImage2;
	int PotionImage3;

	
	int PotionCount;
public:
	
	UI();
	void Update();
	void Draw()const;
};

