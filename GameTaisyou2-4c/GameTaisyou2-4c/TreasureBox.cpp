#include "TreasureBox.h"
#include"DxLib.h"
#include"PadInput.h"

TreasureBox::TreasureBox()
{

	//�󔠉摜
	//OnBoxImage;
	//UnderBoxImage;

	//�󔠂̈ʒu
	OnBox;
	UnderBox;


	ButtonFlag=false;
}

void TreasureBox::Update()
{


	if (PAD_INPUT::OnClick(XINPUT_BUTTON_X))
	{
		ButtonFlag = true;
	}
}

void TreasureBox::Draw()const
{
	if (ButtonFlag == true)
	{
		DrawString(0, 500, "TRUE",0xffffff);
	}

}