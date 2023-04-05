#include "Item.h"
#include "Player.h"
#include "common.h"
#include "PadInput.h"
#include "Dxlib.h"
#include <math.h>

Item::Item(int Type, weapons Weapon ,Range position) 
{
	this->Type = Type;
	this->Weapon = Weapon;

	CanGet = false;

	SetItem();

	pos = position;

}

void Item::Update(Player* player)
{
	//プレイヤーがアイテムを取れる位置にいるか判定
	int CanGetDistance = BLOCK_SIZE / 3;	//プレイヤーがアイテムを取れる境界
	int dis = GetDis({ player->GetX(),player->GetY() });	//プレイヤー間の距離
	if (GetDis({ player->GetX(),player->GetY() }) <= CanGetDistance)
	{
		CanGet = true;
	}
	else CanGet = false;

	//武器・武器切り替え
	if (Type == 1 && CanGet && !player->GetAttack() && PAD_INPUT::OnClick(XINPUT_BUTTON_Y))
	{
		weapons old = player->GetEquip();
		player->ChangeEquip(Weapon);
		Weapon = old;
		SetItem();
	}

	//位置調整
	while ((MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE]) ||
		(MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE]))
	{
		pos.Y++;
	}

	while ((!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE]) ||
		(!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE]))
	{
		pos.Y--;
	}
}

void Item::Draw(Range Player)const
{
	DrawRotaGraph(pos.X - Player.X + (SCREEN_WIDTH / 2), pos.Y - Player.Y + (SCREEN_HEIGHT / 2), size, 0, image, TRUE);

	if (CanGet)DrawRotaGraph(pos.X - Player.X + (SCREEN_WIDTH / 2), pos.Y - Player.Y + (SCREEN_HEIGHT / 2) - Height, 1, 0, icon, TRUE);
}

void Item::SetItem() 
{
	if (Type == 1)
	{
		switch (Weapon)
		{
		case weapons::dagger:
			image = LoadGraph("images/Dagger.png");
			size = 0.3;
			break;

		case weapons::mace:
			image = LoadGraph("images/mace2.png");
			size = 0.2;
			break;

		case weapons::spear:
			image = LoadGraph("images/spear.png");
			size = 0.2;
			break;

		case weapons::katana:
			image = LoadGraph("images/katana.png");
			size = 0.2;
			break;

		case weapons::NONE:
			image = LoadGraph("images/Dagger.png");
			size = 0.3;
			break;

		default:
			image = LoadGraph("images/Dagger.png");
			size = 0.3;
			break;
		}
	}
	icon = LoadGraph("images/ItemIcon.png");

	int imgX, imgY;

	GetGraphSize(image, &imgX, &imgY);

	Width = imgX * size;
	Height = imgY * size;

}

int Item::GetDis(Range position)
{
	float DisX = pow(pos.X - position.X, 2);
	float DisY = pow(pos.Y - position.Y, 2);

	return (int)sqrt((int)(DisX + DisY));
}

void Item::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]) 
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}
}
