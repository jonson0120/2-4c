#include "GameOver.h"
#include"TitleScene.h"
#include"KeyManager.h"
#include"GameMainScene.h"
#include"common.h"

GameOver::GameOver()
{
	GameOverImage = LoadGraph("images/GameOver.png");
	GameOverBGM = LoadSoundMem("sound/GameOver.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, GameOverBGM);
	PlaySoundMem(GameOverBGM, DX_PLAYTYPE_LOOP);

}

AbstractScene* GameOver::Update()
{
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		StopSoundMem(GameOverBGM);
		return new Title();
	}
	return this;
}

void GameOver::Draw()const
{
	DrawGraph(0, 0, GameOverImage, FALSE);
	SetFontSize(30);
	DrawString(430, 300 , "Bボタンでタイトルに戻る", GetColor(255, 0, 0));
	/*DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"GameOver",GetColor(255,0,0));*/
	
}
