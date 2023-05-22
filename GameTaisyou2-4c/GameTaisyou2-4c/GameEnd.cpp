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
	
	//画像ロード
	LoadDivGraph("images/booty.png", 4, 4, 1, 145, 100, BootyImg);
	LoadDivGraph("images/HowToBlock.png", 2, 2, 1, 160, 160, Block);
	LoadDivGraph("images/credit.png", 4, 2, 2, 700, 270, String);

	PlaySoundMem(GameClearBGM, DX_PLAYTYPE_LOOP);

	MainPos = -320;
	BlockPos = 0;
	StrPos = SCREEN_WIDTH * 1.5;
}

AbstractScene* GameEnd::Update()
{
	Anim++;
	if (Scene == 0 || Scene == 3) MainPos += 4;
	if (Scene == 1 || Scene == 2)BlockPos += 4;

	if (Anim % 5 == 0 && Scene == 0) {
		// 描画輝度をセット
		Bright += 10;
		if (255 <= Bright) 
		{
			Bright = 255;
			Scene++;
		}
		SetDrawBright(Bright, Bright, Bright);
	}

	if (Scene == 1)
	{
		StrPos -= 4;
		if (StrPos <= -(SCREEN_WIDTH)) 
		{
			Scene++;
			StrPos = SCREEN_WIDTH * 1.5;
		}
	}
	else if (Scene == 2)
	{
		StrPos -= 4;
		if (StrPos <= SCREEN_WIDTH / 2)
		{
			Scene++;
		}
	}

	if (Scene == 3 && PAD_INPUT::OnClick(XINPUT_BUTTON_B) || Fade)
	{
		Fade = true;
		if (Anim % 5 == 0) {
			// 描画輝度をセット
			SetDrawBright(Bright, Bright, Bright);
			Bright -= 10;
		}
		if (Bright <= 0)
		{
			StopSoundMem(GameClearBGM);
			return new Title(BgmSet);
		}
	}
	return this;
}

void GameEnd::Draw()const
{
	DrawGraph(0, 0, GameEndImage, FALSE);

	//アニメーション：文字
	if (Scene == 1)
	{
		DrawRotaGraph(StrPos, 300, 1, 0, String[0], true);
		DrawRotaGraph(StrPos + 700, 300, 1, 0, String[1], true);
	}
	if (2 <= Scene)
	{
		DrawRotaGraph(StrPos, 200, 1, 0, String[2], true);
		DrawRotaGraph(StrPos, 400, 1, 0, String[3], true);
	}

	//アニメーション：主人公
	int Step = Anim / 30 % 4;
	DrawGraph(MainPos, SCREEN_HEIGHT - BLOCK_SIZE - 100 + 2, BootyImg[Step], true);

	//アニメーション：地面
	Step = BlockPos % BLOCK_SIZE;	//地面が動く
	for (int i = 0; i < 10; i++)
	{
		DrawGraph(BLOCK_SIZE * (i - 1) - Step, SCREEN_HEIGHT - BLOCK_SIZE, Block[0], true);
	}
	/*DrawString(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"GameOver",GetColor(255,0,0));*/

}
