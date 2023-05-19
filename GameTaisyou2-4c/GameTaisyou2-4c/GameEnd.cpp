#include "GameEnd.h"
#include"TitleScene.h"
#include"KeyManager.h"
#include"GameMainScene.h"
#include"common.h"

GameEnd::GameEnd(int BgmSet[7])
{

	this->BgmSet[TITLE] = BgmSet[TITLE];
	this->BgmSet[HOWTO] = BgmSet[HOWTO];
	this->BgmSet[DUNGEON] = BgmSet[DUNGEON];
	this->BgmSet[BOSS] = BgmSet[BOSS];
	this->BgmSet[SAFEZONE] = BgmSet[SAFEZONE];
	this->BgmSet[GAMECLEAR] = BgmSet[GAMECLEAR];
	this->BgmSet[GAMEOVER] = BgmSet[GAMEOVER];

	GameEndImage = LoadGraph("images/GameEnd.png");
	GameClearBGM = BgmSet[GAMECLEAR];
	ChangeVolumeSoundMem(255 * 70 / 100, GameClearBGM);
	PlaySoundMem(GameClearBGM, DX_PLAYTYPE_LOOP);

	SetDrawBright(255, 255, 255);
}

AbstractScene* GameEnd::Update()
{
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		StopSoundMem(GameClearBGM);
		return new Title(BgmSet);
	}
	return this;
}

void GameEnd::Draw()const
{
	DrawGraph(0, 0, GameEndImage, FALSE);
	/*DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"GameOver",GetColor(255,0,0));*/

}
