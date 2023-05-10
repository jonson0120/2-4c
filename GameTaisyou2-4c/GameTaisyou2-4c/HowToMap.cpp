#include"DxLib.h"
#include"HowToMap.h"
#include"TitleScene.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"
#include"UI.h"

#include<math.h>
#include<stdlib.h>


HowToMap::HowToMap()
{

	MapExitX = 0;
	MapExitY = 0;
	MakeMap();
	player.SetMapData(MapData);

	//enemy2.SetMapData(MapData);
	
	LoadDivGraph("images/HowToBlock.png", 2, 2, 1, 160, 160, MapImg);
	MapImg[2] = 0;
	MapImg[3] = LoadGraph("images/pillor.png");
	MapImg[4] = LoadGraph("images/board.png");

	int weed[3];
	LoadDivGraph("images/weed.png", 3, 3, 1, 160, 160, weed);

	MapImg[5] = weed[0];
	MapImg[6] = weed[1];
	MapImg[7] = weed[2];

	Block= LoadGraph("images/singleblock.png");

	LoadDivGraph("images/methot.png", 10, 5, 2, 192, 160, HowToImg);
	HowToImg[10] = LoadGraph("images/oksign.png");

	LoadDivGraph("images/HowDagger.png", 2, 2, 1, 60, 53, Dagger);
	Icon = LoadGraph("images/ItemIcon.png");
	Getted = false;

	time = 0;

	count = 0;

	CameraX = 0;
	CameraY = 0;

	Bright = 255;
	Bright_minus = 10;
	AnimTimer = 0;

	LoadDivGraph("images/Gauge.png", 2, 2, 1, 34, 34, DoorIcon);
	DoorIcon[2] = LoadGraph("images/DoorIcon.png");

	Exit_flg = false;
	Anim_flg = false;
	GoMain_flg = false;
	GoTitle_flg = false;
	MoveStop_flg = true;
	Pressed_flg = false;
}

AbstractScene* HowToMap::Update()
{
	if (player.GetLife()<=0)
	{
		player.Reset();
	}

	if (MoveStop_flg == true)
	{
		player.Update();
	}

	//フェーズ進行
	if (player.GetX() / BLOCK_SIZE == 16 && Phase == 0)
	{
		Phase++;
	}
	else if (Phase == 1 && PAD_INPUT::OnClick(XINPUT_BUTTON_A))
	{
		PhaseCount = 60;
		Phase++;
	}
	else if (player.GetX() / BLOCK_SIZE == 28 && Phase == 2)
	{
		Phase++;
	}
	else if (Phase == 3 && ((player.GetWall() == 2 && MARGIN < PAD_INPUT::GetPadThumbLY()) || player.GetX() / BLOCK_SIZE == 31))
	{
		if (player.GetX() / BLOCK_SIZE != 31)PhaseCount = 60;
		Phase++;
	}
	else if (player.GetX() / BLOCK_SIZE == 33 && Phase == 4)
	{
		Phase++;
	}
	else if (Phase == 5 && (player.GetWall() == 3 || player.GetX() / BLOCK_SIZE == 39))
	{
		if (player.GetX() / BLOCK_SIZE != 39)PhaseCount = 60;
		Phase++;
	}
	else if (Phase == 6 && Getted)
	{
		Phase++;
	}
	else if (7 <= Phase && Phase <= 9 && player.GetX() / BLOCK_SIZE == 46 && 120 < player.GetX() % BLOCK_SIZE && PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		if (PhaseCount == 0)
		{
			PhaseCount = 60;
			Phase++;
		}
	}

	if (Phase == 10 && PhaseCount == 60)
	{
		MapData[8][47] = 1;
		player.SetMapData(MapData);
	}

	if (--PhaseCount < 0)PhaseCount = 0;
	Anim++;

	//enemy2.Update(&player);
	CameraX = player.GetX();
	CameraY = player.GetY();

	if (player.GetY() / BLOCK_SIZE == MAP_HEIGHT - 3 && player.GetX() / BLOCK_SIZE == MAP_WIDTH_T - 5 && !Getted && PAD_INPUT::OnClick(XINPUT_BUTTON_Y))
	{
		player.SetDagger();
		Getted = true;
	}

	if (player.GetY() / BLOCK_SIZE == MAP_HEIGHT - 3 && (player.GetX() / BLOCK_SIZE == 5 || player.GetX() / BLOCK_SIZE == MAP_WIDTH_T - 2))
	{
		NextMap();
	}

	if (CheckHitKey(KEY_INPUT_Q))
	{
		if (Phase < 7)Phase = 7;
		player.SetX(BLOCK_SIZE * 45);
	}

	if (GoMain_flg)return new GameMainScene();
	if (GoTitle_flg)return new Title();

	return this;
}

void HowToMap::Draw() const
{
	DrawGraph(BLOCK_SIZE * 22 - player.GetX() - 15, 360 + BLOCK_SIZE * 8 - player.GetY() + 110, MapImg[3], TRUE);
	DrawGraph(BLOCK_SIZE * 28 - player.GetX() - 15, 360 + BLOCK_SIZE * 7 - player.GetY() + 110, MapImg[3], TRUE);
	DrawGraph(BLOCK_SIZE * 35 - player.GetX() - 15, 360 + BLOCK_SIZE * 6 - player.GetY() + 110, MapImg[3], TRUE);
	DrawGraph(BLOCK_SIZE * 38 - player.GetX() - 15, 360 + BLOCK_SIZE * 7 - player.GetY() + 110, MapImg[3], TRUE);
	DrawGraph(BLOCK_SIZE * 43 - player.GetX() - 15, 360 + BLOCK_SIZE * 6 - player.GetY() + 110, MapImg[3], TRUE);

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH_T; j++)
		{
			if (ImgData[i][j] <= 1) DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY() + 150, MapImg[ImgData[i][j]], TRUE);
			else if (ImgData[i][j] == 5) DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY() + 150, Block, TRUE);
		}
	}

	int weed = Phase - 7;
	if (weed < 0)weed = 0;
	if (MapData[8][47] == 0)DrawGraph(160 * (4 + 47) - player.GetX(), 360 + 160 * 8 - player.GetY() + 150, MapImg[7 - weed] , TRUE);

	DrawGraph(160 * (4 + 10) - player.GetX(), 360 + 160 * 8 - player.GetY() + 150, MapImg[4], TRUE);

	if (!Getted)DrawRotaGraph(BLOCK_SIZE * (MAP_WIDTH_T - 5 + 0.5) - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * (MAP_HEIGHT - 2 + 0.5) - player.GetY() + SCREEN_HEIGHT / 2 + 45,  1, 0, Dagger[0], true);
	else DrawRotaGraph(BLOCK_SIZE * (MAP_WIDTH_T - 5 + 0.5) - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * (MAP_HEIGHT - 2 + 0.5) - player.GetY() + SCREEN_HEIGHT / 2 + 45, 1, 0, Dagger[1], true);

	if (player.GetY() / BLOCK_SIZE == MAP_HEIGHT - 3 && player.GetX() / BLOCK_SIZE == MAP_WIDTH_T - 5 && !Getted)
	{
		DrawRotaGraph(BLOCK_SIZE * (MAP_WIDTH_T - 5 + 0.5) - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * (MAP_HEIGHT - 2 + 0.5) - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Icon, true);
	}

	if (Phase == 1 || (Phase == 2 && PhaseCount))
	{
		int flame = Anim / 30 % 2;
		DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[0 + flame], TRUE);
		if (Phase == 2)DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[10], TRUE);
	}

	if (Phase == 3 || (Phase == 4 && PhaseCount))
	{
		int flame = Anim / 30 % 3;
		DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[2 + flame], TRUE);
		if (Phase == 4)DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[10], TRUE);
	}

	if (Phase == 5 || (Phase == 6 && PhaseCount))
	{
		int flame = Anim / 30 % 3;
		DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[5 + flame], TRUE);
		if (Phase == 6)DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[10], TRUE);
	}

	if ((7 <= Phase && Phase <= 9) || (Phase == 10 && PhaseCount))
	{
		int flame = Anim / 30 % 2;
		DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[8 + flame], TRUE);
		if (Phase == 10)DrawRotaGraph(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2, 1.0f, 0, HowToImg[10], TRUE);
	}

	player.Draw();

	//操作説明
}

//マップ生成
void HowToMap::MakeMap()
{
		//---------------------------------------------------------------

		//マップ端の壁・天井-----------------------
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH_T; j++)
			{
				ImgData[i][j] = 2;
				if (j == MAP_WIDTH_T - 1 || j <= 4)
				{
					MapData[i][j] = 0;
				}
				else MapData[i][j] = 1;

				if (MAP_HEIGHT - 2 <= i || i == 0)
				{
					MapData[i][j] = 0;
					if (i == MAP_HEIGHT - 2)ImgData[i][j] = 0;
					if (i == MAP_HEIGHT - 1)ImgData[i][j] = 1;
				}

				if (34 <= j && i <= 4) 
				{
					MapData[i][j] = 0;
					ImgData[i][j] = 5;
				}

				if (45 <= j && i <= 6)
				{
					MapData[i][j] = 0;
					ImgData[i][j] = 5;
				}

				if (47 <= j && i <= 7)
				{
					MapData[i][j] = 0;
					ImgData[i][j] = 5;
				}
			}
		}

		MapData[8][18] = 0;

		MapData[8][24] = 0;
		MapData[7][24] = 0;

		MapData[8][31] = 0;
		MapData[7][31] = 0;
		MapData[6][31] = 0;

		MapData[8][34] = 0;
		MapData[7][34] = 0;

		MapData[8][39] = 0;
		MapData[7][39] = 0;
		MapData[6][39] = 0;

		MapData[8][47] = 0;
		//------------------------------------------

}

void HowToMap::NextMap() {
	AnimTimer++;
	MoveStop_flg = false;

	if (0 <= Bright && Anim_flg == false) {
		// フェードアウト処理
		if (AnimTimer % 5 == 0) {
			// 描画輝度をセット
			SetDrawBright(Bright, Bright, Bright);
			Bright -= Bright_minus;
		}
		if (Bright <= 0)
		{
			if (player.GetX() / BLOCK_SIZE == 5) GoTitle_flg = true;
			else GoMain_flg = true;
		}
	}
}