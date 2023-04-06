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
	DrawString(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, "Bボタン押してください", GetColor(255, 0, 0));
	DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"どんまい",GetColor(255,0,0));
	
}
