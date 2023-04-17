#pragma once
#include "Player.h"
#include"PadInput.h"


class UI
{
private:
	Stat stat;

	float MaxHP;
	float PlayerHP;

	bool potionflag;

	int Damage;

	int Weapon;	//現在装備している武器
	int Second;	//現在装備していない武器

	int DaggerImage;
	int MaceImage;
	int SpearImage;
	int KatanaImage;
	int NoneImage;
	 
	int Gauge[3];

	int PotionImage1;
	int ButtonImg[2];

	int Number[44];

	int PotionCount;
	int PotionMax;

	int UseCount = 0;
public:
	
	UI();
	void Update(Player* player);
	void Draw()const;
};

