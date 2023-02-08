#include "UI.h"
#include"DxLib.h"

UI::UI()
{

}

void UI::Update() 
{

}

void UI::Draw() const
{
	//âÒïú
	DrawCircle(140, 75, 25, GetColor(255, 0, 0), TRUE);
	DrawCircle(195, 75, 25, GetColor(255, 0, 0), TRUE);
	DrawCircle(250, 75, 25, GetColor(255, 0, 0), TRUE);

	//ïêäÌ
	DrawCircle(50, 50, 50, GetColor(255, 255, 255), TRUE);

	//HP
	DrawBox(110, 10, 500, 50, GetColor(0, 255, 0), TRUE);
	DrawBox(110, 10, 500, 50, GetColor(255,0, 0), FALSE);

	


}