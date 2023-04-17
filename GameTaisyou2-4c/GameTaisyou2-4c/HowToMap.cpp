#include"DxLib.h"
#include"HowToMap.h"
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
	
	LoadDivGraph("images/HowToBlock.png", 8, 4, 2, 160, 160, MapImg);

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

	//enemy2.Update(&player);
	CameraX = player.GetX();
	CameraY = player.GetY();

	if (player.GetY() / BLOCK_SIZE == MAP_HEIGHT - 3 && player.GetX() / BLOCK_SIZE == MAP_WIDTH_T - 10 && !Getted && PAD_INPUT::OnClick(XINPUT_BUTTON_Y))
	{
		player.SetDagger();
		Getted = true;
	}

	if (player.GetY() / BLOCK_SIZE == MAP_HEIGHT - 3 && player.GetX() / BLOCK_SIZE == MAP_WIDTH_T - 2)
	{
		NextMap();
	}

	if (CheckHitKey(KEY_INPUT_Q)) player.SetX(BLOCK_SIZE * 40);

	if (GoMain_flg)return new GameMainScene();

	return this;
}

void HowToMap::Draw() const
{

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH_T; j++)
		{
			if (MapData[i][j] < 4)DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY() + 150, MapImg[MapData[i][j]], TRUE);
		}
	}

	if (!Getted)DrawRotaGraph(BLOCK_SIZE * (MAP_WIDTH_T - 10 + 0.5) - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * (MAP_HEIGHT - 2 + 0.5) - player.GetY() + SCREEN_HEIGHT / 2 + 45,  1, 0, Dagger[0], true);
	else DrawRotaGraph(BLOCK_SIZE * (MAP_WIDTH_T - 10 + 0.5) - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * (MAP_HEIGHT - 2 + 0.5) - player.GetY() + SCREEN_HEIGHT / 2 + 45, 1, 0, Dagger[1], true);

	if (player.GetY() / BLOCK_SIZE == MAP_HEIGHT - 3 && player.GetX() / BLOCK_SIZE == MAP_WIDTH_T - 10 && !Getted)
	{
		DrawRotaGraph(BLOCK_SIZE * (MAP_WIDTH_T - 10 + 0.5) - player.GetX() + SCREEN_WIDTH / 2, BLOCK_SIZE * (MAP_HEIGHT - 2 + 0.5) - player.GetY() + SCREEN_HEIGHT / 2, 1, 0, Icon, true);
	}

	player.Draw();
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
				if (j == MAP_WIDTH_T - 1 || j <= 4)
				{
					MapData[i][j] = 0;
				}
				else MapData[i][j] = 1;

				if (MAP_HEIGHT - 2 <= i || i == 0)
				{
					MapData[i][j] = 0;
				}
			}
		}
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
		if (Bright <= 0)GoMain_flg = true;
	}
}