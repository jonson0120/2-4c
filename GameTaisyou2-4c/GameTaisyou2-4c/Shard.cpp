#include "Shard.h"
#include "Player.h"
#include "common.h"
#include "Dxlib.h"
#include <math.h>

Shard::Shard(Range position)
{
	CanGet = false;
	Getted = false;

	SetItem();

	fall = -(GetRand(3) + (0.1 * GetRand(9)));
	Slide = GetRand(3) + (0.1 * GetRand(9));

	if (GetRand(1))Slide *= -1;

	pos = position;
	pos.Y -= Height;

	ShardSE = LoadSoundMem("sound/Shard.mp3");
}

void Shard::Update(Player* player)
{
	//�v���C���[���A�C�e�������ʒu�ɂ��邩����
	int CanGetDistance = 15;	//�v���C���[���A�C�e������鋫�E

	int dis = GetDis({ player->GetX(),player->GetY() });	//�v���C���[�Ԃ̋���

	if (dis <= CanGetDistance)
	{
		PlaySoundMem(ShardSE, DX_PLAYTYPE_BACK);
		player->AddShard();
		Getted = true;
	}

	//�������̈ړ�
	pos.X += Slide;
	while (!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE] ||
		!MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE])
	{
		pos.X++;
	}

	while (!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE] ||
		!MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE])
	{
		pos.X--;
	}

	//�����ƃW�����v
	// 
	//�W�����v���x
	float fallinit = 12;

	if (fall < fallinit)
	{
		fall += (fallinit * 2) / 45;
		if (fall > fallinit)
		{
			fall = fallinit;
		}
	}
	pos.Y += fall;

	//�ʒu����

	if (MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE] ||
		MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE])
	{
		Slide = 0;
	}

	while (!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE] ||
		!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE])
	{
		pos.Y--;
		Slide = 0;
	}

}

void Shard::Draw(Range Player)const
{
	DrawRotaGraph(pos.X - Player.X + (SCREEN_WIDTH / 2), pos.Y - Player.Y + (SCREEN_HEIGHT / 2), size, 0, image, TRUE);
}

void Shard::SetItem()
{
	image = LoadGraph("images/Shard.png");
	size = 1;

	int imgX, imgY;

	GetGraphSize(image, &imgX, &imgY);

	Width = imgX * size;
	Height = imgY * size;

}

int Shard::GetDis(Range position)
{
	float DisX = pow(pos.X - position.X, 2);
	float DisY = pow(pos.Y - position.Y, 2);

	return (int)sqrt((int)(DisX + DisY));
}

void Shard::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}
}
