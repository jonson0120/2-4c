#include "GameOver.h"
#include"TitleScene.h"
#include"KeyManager.h"
#include"GameMainScene.h"
#include"common.h"

GameOver::GameOver(int BgmSet[7])
{
	this->BgmSet[TITLE] = BgmSet[TITLE];
	this->BgmSet[HOWTO] = BgmSet[HOWTO];
	this->BgmSet[DUNGEON] = BgmSet[DUNGEON];
	this->BgmSet[BOSS] = BgmSet[BOSS];
	this->BgmSet[SAFEZONE] = BgmSet[SAFEZONE];
	this->BgmSet[GAMECLEAR] = BgmSet[GAMECLEAR];
	this->BgmSet[GAMEOVER] = BgmSet[GAMEOVER];

	GameOverImage = LoadGraph("images/GameOver.png");
	GameOverBGM = BgmSet[GAMEOVER];
	ChangeVolumeSoundMem(255 * 70 / 100, GameOverBGM);
	PlaySoundMem(GameOverBGM, DX_PLAYTYPE_LOOP);

	SetDrawBright(255, 255, 255);

}

AbstractScene* GameOver::Update()
{
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		StopSoundMem(GameOverBGM);
		return new Title(BgmSet);
	}
	return this;
}

void GameOver::Draw()const
{
	DrawGraph(0, 0, GameOverImage, FALSE);
	SetFontSize(30);
	//DrawString(430, 300, "Bボタンでタイトルに戻る", 0xff0000);
}
