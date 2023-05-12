#include "Item.h"
#include "Weapon.h"
#include "Player.h"
#include "common.h"
#include "PadInput.h"
#include "Dxlib.h"
#include <math.h>

Weapon::Weapon(weapons Weapon, Range position, int Level)
{
	this->Kind = Weapon;

	CanGet = false;
	Getted = false;

	SetItem();
	SetPassive(Level);

	LoadDivGraph("images/passive.png", 7, 1, 7, 83, 17, passiveimg);
	passiveimg[7] = LoadGraph("images/passivewin.png");

	LoadDivGraph("images/number.png", 44, 11, 4, 10, 16, numimg);

	fall = -3;

	pos = position;
	pos.Y -= Height;

}

void Weapon::SetPassive(int Level) 
{
	int potential = Level / 4;		//�o������\���̂���X�L����
	if (5 < potential)potential = 5;

	bool getted[6] = { false,false,false,false,false,false };

	passive[0] = { ATTACK,GetRand(1 + Level / 10) + 1 };

	int chance;

	for (int i = 0; i < 3; i++)
	{
		if (i == 0) 
		{
			if (Level < 10)chance = 25;
			else if (Level < 20)chance = 50;
			else if (Level < 30)chance = 65;
			else if (Level < 40)chance = 80;
			else chance = 100;
		}
		if (i == 1)
		{
			if (Level < 10)chance = 15;
			else if (Level < 20)chance = 30;
			else if (Level < 30)chance = 50;
			else if (Level < 40)chance = 65;
			else chance = 75;

			if (potential == 0)chance = 0;
		}
		if (i == 2)
		{
			if (Level < 10)chance = 5;
			else if (Level < 20)chance = 20;
			else if (Level < 30)chance = 40;
			else if (Level < 40)chance = 50;
			else chance = 40;

			if (potential <= 1)chance = 0;
		}

		if (GetRand(99) <= chance - 1)
		{
			Enchant win = static_cast<Enchant>(GetRand(potential) + 1);
			while (getted[win - 1])
			{
				win = static_cast<Enchant>(GetRand(potential) + 1);
			}
			int effect = 0;
			switch (win)
			{
			case ATTACK:
				break;
			case DEFENSE:
				effect = GetRand(1 + Level / 10) + 1;
				break;
			case GREED:
				effect = GetRand(1) + 1;
				break;
			case BARRIER:
				effect = GetRand(1 + Level / 10) + 1;
				break;
			case VAMP:
				effect = GetRand(2) + 1;
				break;
			case REPAIR:
				effect = GetRand(1 + Level / 10) + 1;
				break;
			case DODGE:
				effect = GetRand(1) + 1;
				break;
			case NONE:
				break;
			default:
				break;
			}

			passive[i + 1] = { win,effect };
			getted[win - 1] = true;
		}
		else passive[i + 1] = { NONE,0 };
	}

	for (int i = 1; i < 3; i++)
	{
		for (int j = 1; j + i < 4; j++)
		{
			if (passive[i + j].Kinds < passive[i].Kinds) 
			{
				Passive val = passive[i];
				passive[i] = passive[i + j];
				passive[i + j] = val;
			}
		}
	}
}

void Weapon::Update(Player* player)
{
	Getted = false;

	//�v���C���[���A�C�e�������ʒu�ɂ��邩����
	int CanGetDistance = BLOCK_SIZE / 3;	//�v���C���[���A�C�e������鋫�E

	int dis = GetDis({ player->GetX(),player->GetY() });	//�v���C���[�Ԃ̋���

	if (dis <= CanGetDistance)
	{
		CanGet = true;
	}
	else CanGet = false;

	//����E����؂�ւ�
	if (CanGet && !player->GetAttack() && PAD_INPUT::OnClick(XINPUT_BUTTON_Y))
	{
		weapons old = player->GetEquip();
		Passive oldp[4];

		for (int i = 0; i < 4; i++)
		{
			oldp[i] = player->GetPassive(i);
		}
		player->ChangeEquip(Kind, passive);
			Kind = old;
			for (int i = 0; i < 4; i++)
			{
				passive[i] = oldp[i];
			}

			SetItem();
			Getted = true;
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

	while (!MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE]||
		   !MapData[(pos.Y - Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE])
	{
		pos.Y++;
	}

	while (!MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X - Width / 2) / BLOCK_SIZE] ||
	 	   !MapData[(pos.Y + Height / 2) / BLOCK_SIZE][(pos.X + Width / 2) / BLOCK_SIZE])
	{
		pos.Y--;
	}

}

void Weapon::Draw(Range Player)const
{
	DrawRotaGraph(pos.X - Player.X + (SCREEN_WIDTH / 2), pos.Y - Player.Y + (SCREEN_HEIGHT / 2), size, 0, image, TRUE);

	if (CanGet)
	{
		int x = pos.X - Player.X + (SCREEN_WIDTH / 2);
		int y = pos.Y - Player.Y + (SCREEN_HEIGHT / 2) - Height;
		DrawRotaGraph(x, y, 1, 0, icon, TRUE);
		DrawRotaGraph(x, y - 50, 1, 0, passiveimg[7], TRUE);
		for (int i = 0; i < 4; i++)
		{
			if (passive[i].Kinds != NONE) 
			{
				DrawRotaGraph2(x - 55, y - 77 + (18 * i), 0, 9, 1, 0, passiveimg[passive[i].Kinds], true);
				DrawRotaGraph(x + 40, y - 77 + (18 * i), 1, 0, numimg[passive[i].Effect], true);
			}
		}
	}
}

void Weapon::SetItem()
{
		switch (Kind)
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
	icon = LoadGraph("images/ItemIcon.png");

	int imgX, imgY;

	GetGraphSize(image, &imgX, &imgY);

	Width = imgX * size;
	Height = imgY * size;

}

int Weapon::GetDis(Range position)
{
	float DisX = pow(pos.X - position.X, 2);
	float DisY = pow(pos.Y - position.Y, 2);

	return (int)sqrt((int)(DisX + DisY));
}

void Weapon::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH])
{
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}
}
