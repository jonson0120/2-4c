#include "GameOver.h"
#include"KeyManager.h"
#include"GameMainScene.h"
#include"common.h"

GameOver::GameOver()
{
	
	
}

AbstractScene* GameOver::Update()
{
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		return new GameMainScene();
	}
	return this;
}

void GameOver::Draw()const
{
	DrawString(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, "Bƒ{ƒ^ƒ“‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢", GetColor(255, 0, 0));
	DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"‚Ç‚ñ‚Ü‚¢",GetColor(255,0,0));
	
}
