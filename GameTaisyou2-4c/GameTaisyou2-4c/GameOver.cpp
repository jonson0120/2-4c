#include "GameOver.h"
#include"TitleScene.h"
#include"KeyManager.h"
#include"GameMainScene.h"
#include"common.h"

GameOver::GameOver()
{
	GameOverImage = LoadGraph("images/GameOver.png");
}

AbstractScene* GameOver::Update()
{
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		return new Title();
	}
	return this;
}

void GameOver::Draw()const
{
	DrawGraph(0, 0, GameOverImage, FALSE);
	SetFontSize(30);
	DrawString(430, 300 , "Bƒ{ƒ^ƒ“‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢", GetColor(255, 0, 0));
	/*DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"GameOver",GetColor(255,0,0));*/
	
}
