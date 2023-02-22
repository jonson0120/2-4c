#include"DxLib.h"
#include"GameMainScene.h"
#include"KeyManager.h"
#include"AbstractScene.h"
#include"UI.h"

GameMainScene::GameMainScene()
{
	MakeMap();
	player.SetMapData(MapData);
	enemy.SetMapData(MapData);

	LoadDivGraph("images/Block.png", 2, 2, 1, 160, 160, MapImg);

	time = 0;

	CameraX = 0;
	CameraY = 0;

}

AbstractScene* GameMainScene::Update() 
{

	ui.Update();
	player.Update();
	enemy.Update();
	CameraX = player.GetX();
	CameraY = player.GetY();

	switch (player.GetEquip())
	{
	case weapons::dagger:
		if (player.HitDagger(enemy.GetX(), enemy.GetY(), enemy.GetWidth(), enemy.GetHeight()))hit++;

	default:
		break;
	}

	time++;
	return this;
}

void GameMainScene::Draw() const
{

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (MapData[i][j] != 2) DrawGraph(160 * (4 + j) - player.GetX(), 360 + 160 * i - player.GetY(), MapImg[MapData[i][j]], TRUE);
		}
	}
	
	DrawFormatString(0, 500, 0xff0000, "%d", Space);
	ui.Draw();
	player.Draw();
	enemy.Draw(player.GetX(),player.GetY());

}

//�}�b�v����
void GameMainScene::MakeMap()
{
	//�ǁE��ԃp�^�[��
	int parts_max = 0;
	int map_parts[][3][3] = {

		{{1,1,1},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {1,0,1},
		 {1,1,1},},

		{{0,1,0},
		 {1,1,1},
		 {0,1,0},},

		{{0,0,0},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {0,0,0},
		 {1,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,0,0},},

		{{0,1,1},
		 {0,1,1},
		 {0,1,1},},

		{{1,0,1},
		 {1,0,1},
		 {1,0,1},},

		{{1,1,0},
		 {1,1,0},
		 {1,1,0},},

		{{0,1,1},
		 {1,0,1},
		 {1,1,0},},

		{{1,1,0},
		 {1,0,1},
		 {0,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {1,1,0},},

		{{1,1,0},
		 {1,1,1},
		 {1,1,1},},

		{{0,1,1},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,0},
		 {1,0,0},
		 {0,0,1},},

		{{1,0,0},
		 {0,0,1},
		 {0,1,1},},

		{{0,1,1},
		 {0,0,1},
		 {1,0,0},},

		{{0,0,1},
		 {1,0,0},
		 {1,1,0},},

		{{1,1,1},
		 {1,1,1},
		 {1,1,0},},

		{{1,1,0},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,1,1},},

		{{0,1,1},
		 {1,1,1},
		 {1,1,1},},

		{{1,1,1},
		 {1,1,1},
		 {0,0,1},},

		{{1,1,1},
		 {1,1,0},
		 {1,1,0},},

		{{1,0,0},
		 {1,1,1},
		 {1,1,1},},

		{ {0,1,1},
		  {0,1,1},
		  {1,1,1}, },
	};
	parts_max = sizeof(map_parts) / sizeof(*map_parts);

	//�}�b�v�f�[�^�쐬
	do {

		//��ԃ`�F�b�N�Ɏg�p����f�[�^���Z�b�g-------
		Space = 0;
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				CheckData[i][j] = 0;
			}
		}
		//-------------------------------------------
		
		//�ǐ���----------------------------------------------------
		for (int i = 1; i < MAP_HEIGHT - 1; i += 3)
		{
			for (int j = 1; j < MAP_WIDTH - 1; j += 3)
			{
				int parts = GetRand(parts_max - 1);	//�g�p����p�^�[�������߂�

				//�p�^�[���ɉ����ĕǂ����
				MapData[i][j] = map_parts[parts][0][0];
				MapData[i + 1][j] = map_parts[parts][1][0];
				MapData[i + 2][j] = map_parts[parts][2][0];

				MapData[i][j + 1] = map_parts[parts][0][1];
				MapData[i + 1][j + 1] = map_parts[parts][1][1];
				MapData[i + 2][j + 1] = map_parts[parts][2][1];

				MapData[i][j + 2] = map_parts[parts][0][2];
				MapData[i + 1][j + 2] = map_parts[parts][1][2];
				MapData[i + 2][j + 2] = map_parts[parts][2][2];
			}
		}
		//---------------------------------------------------------------

		//�}�b�v�[�̕ǁE�V��-----------------------
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_WIDTH; j++)
			{
				if (j == MAP_WIDTH - 1 || j == 0)
				{
					MapData[i][j] = 0;
				}

				if (i == MAP_HEIGHT - 1 || i == 0)
				{
					MapData[i][j] = 0;
				}
			}
		}
		//------------------------------------------


		//�v���C���[�̏����ʒu����Ԃɂ���
		MapData[player.GetY() / 160][player.GetX() / 160] = 1;

		//��Ԑ��`�F�b�N
		CheckSpace(player.GetY() / 160, player.GetX() / 160, &Space);

		//��Ԑ������ȉ��Ȃ�Đ���
	} while (Space < 70);

	//�Ǘ�������Ԃ𖄂߂�
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (CheckData[i][j] == 0)MapData[i][j] = 2;
		}
	}
}

int GameMainScene::CheckSpace(int y, int x, int* cnt)
{
		
		//�Ώۃu���b�N���O�g�Ȃ珈���𔲂���
		if (x == 0 || x == MAP_WIDTH - 1 || y == MAP_HEIGHT - 1 || y == 0)return 0;
	
		CheckData[y][x] = 1;
		(*cnt)++;
	
		if (MapData[y + 1][x] == 1 && !CheckData[y + 1][x])CheckSpace(y + 1, x, cnt);
		else CheckData[y + 1][x] = 1;

		if (MapData[y - 1][x] == 1 && !CheckData[y - 1][x])CheckSpace(y - 1, x, cnt);
		else CheckData[y - 1][x] = 1;

		if (MapData[y][x + 1] == 1 && !CheckData[y][x + 1])CheckSpace(y, x + 1, cnt);
		else CheckData[y][x + 1] = 1;

		if (MapData[y][x - 1] == 1 && !CheckData[y][x - 1])CheckSpace(y, x - 1, cnt);
		else CheckData[y][x - 1] = 1;
	
		return 0;
}