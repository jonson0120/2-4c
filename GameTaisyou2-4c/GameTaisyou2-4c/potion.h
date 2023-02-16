#pragma once
#include"Player.h"
#include"PadInput.h"

class Potion:public Player
{
private:
	
public:
	void InitPad();
	Potion();
	void Update();
	void Draw()const;

};

