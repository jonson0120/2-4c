#include "GameEnd.h"
#include"TitleScene.h"
#include"KeyManager.h"
#include"GameMainScene.h"
#include"common.h"

GameEnd::GameEnd()
{
	SetDrawBright(255, 255, 255);
	GameEndImage = LoadGraph("images/GameEnd.png");
	GameClearBGM = LoadSoundMem("sound/GameClear.mp3");
	ChangeVolumeSoundMem(255 * 70 / 100, GameClearBGM);
	PlaySoundMem(GameClearBGM, DX_PLAYTYPE_LOOP);
}

AbstractScene* GameEnd::Update()
{
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		StopSoundMem(GameClearBGM);
		return new Title();
	}
	return this;
}

void GameEnd::Draw()const
{
	DrawGraph(0, 0, GameEndImage, FALSE);
	/*DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"GameOver",GetColor(255,0,0));*/

}
