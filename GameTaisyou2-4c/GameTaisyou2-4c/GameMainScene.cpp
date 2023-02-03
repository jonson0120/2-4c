#include"DxLib.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"

GameMainScene::GameMainScene()
{

}

AbstractScene* GameMainScene::Update() 
{
	
	return this;
}

void GameMainScene::Draw() const
{
	DrawString(0, 0, "HELLO", 0xffffff, TRUE);
}