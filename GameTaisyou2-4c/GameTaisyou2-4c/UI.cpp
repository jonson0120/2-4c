#include"UI.h"
#include"DxLib.h"
#include"PadInput.h"
#include"Player.h"


UI::UI()
{
	stat = { 0,0,0 };
	PotionCount = 0;
	PotionMax = 0;

	Weapon = 0;

	DaggerImage = LoadGraph("images/Dagger.png");
	MaceImage = LoadGraph("images/mace.png");
	SpearImage = LoadGraph("images/spear.png");
	KatanaImage = LoadGraph("images/katana.png");
	NoneImage = LoadGraph("images/Empty.png");
	
	LoadDivGraph("images/Gauge.png", 3, 3, 1, 34, 34, Gauge);

	LoadDivGraph("images/number.png", 44, 11, 4, 10, 16, Number);

	PotionImage1 = LoadGraph("images/potion2.png");
	LoadDivGraph("images/UiButton.png", 2, 2, 1, 21, 21, ButtonImg);

	Damage = 0;

	PlayerHP = 0;
	MaxHP = 0;

	potionflag = false;
	
}

void UI::Update(Player* player)
{
	stat = player->GetStat();
	PlayerHP = stat.Hp;
	MaxHP = stat.MaxHp;

	PotionCount = player->GetPotion();
	PotionMax = player->GetPotionMax();

	UseCount = player->UseP();

	switch (player->GetEquip())
	{
	case weapons::dagger:
		Weapon = 0;
		break;
	case weapons::mace:
		Weapon = 1;
		break;
	case weapons::spear:
		Weapon = 2;
		break;
	case weapons::katana:
		Weapon = 3;
		break;
	case weapons::NONE:
		Weapon = 4;
		break;
	default:
		break;
	}

	switch (player->GetSecond())
	{
	case weapons::dagger:
		Second = 0;
		break;
	case weapons::mace:
		Second = 1;
		break;
	case weapons::spear:
		Second = 2;
		break;
	case weapons::katana:
		Second = 3;
		break;
	case weapons::NONE:
		Second = 4;
		break;
	default:
		break;
	}
}

void UI::Draw() const
{

	/*DrawFormatString(0, 200, GetColor(255, 255, 255), "%d", PotionCount);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "%.1f", PlayerHP);
	DrawFormatString(0, 400, GetColor(255, 255, 255), "%d", Damage);*/

	//ポーション使用時の●
	for (int i = 0; i < PotionMax; i++)
	{
		DrawCircle(165 + (i * 55), 75, 5, GetColor(255, 0, 0), TRUE);
	}

	//ポーション画像の表示

	for (int i = 0; i < PotionCount; i++)
	{
		DrawRotaGraph(165 + (i * 55), 80, 0.3, 0, PotionImage1, TRUE);
	}

	DrawRotaGraph(140, 100, 1.2, 0, Gauge[0], true);
	DrawCircleGauge(140, 100, 100 * (UseCount / 60.f), Gauge[2], 0, 1.1, false, false);
	DrawRotaGraph(140, 100, 0.9, 0, ButtonImg[0], TRUE);

	//武器
	DrawCircle(75, 50, 50, GetColor(35, 59, 108), TRUE);
	switch (Weapon)
	{
	case 0:
		DrawRotaGraph(75, 50, 0.5, 0, DaggerImage, TRUE);
		break;
	case 1:
		DrawRotaGraph(75, 50, 0.2, 0, MaceImage, TRUE);
		break;
	case 2:
		DrawRotaGraph(75, 50, 0.2, 0, SpearImage, TRUE);
		break;
	case 3:
		DrawRotaGraph(75, 50, 0.25, 0, KatanaImage, TRUE);
		break;
	case 4:
		DrawRotaGraph(50, 50, 1, 0, NoneImage, TRUE);
		break;
	}

	DrawCircle(25, 75, 25, GetColor(35, 59, 108), TRUE);
	switch (Second)
	{
	case 0:
		DrawRotaGraph(25, 75, 0.25, 0, DaggerImage, TRUE);
		break;
	case 1:
		DrawRotaGraph(25, 75, 0.1, 0, MaceImage, TRUE);
		break;
	case 2:
		DrawRotaGraph(25, 75, 0.1, 0, SpearImage, TRUE);
		break;
	case 3:
		DrawRotaGraph(25, 75, 0.12, 0, KatanaImage, TRUE);
		break;
	case 4:
		DrawRotaGraph(25, 75, 0.5, 0, NoneImage, TRUE);
		break;
	}

	DrawRotaGraph(10, 55, 1, 0, ButtonImg[1], TRUE);

	//HP赤
	DrawBox(135, 10, 535, 50, GetColor(255, 0, 0), TRUE);

	//HP緑
	DrawBox(135, 10, 135 + (PlayerHP / MaxHP) * 400, 50, GetColor(0, 255, 0), TRUE);

	//HPバーの枠(白)
	DrawBox(135, 10, 135 + (PlayerHP / MaxHP) * 400, 50, GetColor(255, 255, 255), FALSE);

	//HP表示
	//100の位
	if (100 < MaxHP)
	{
		if (100 <= PlayerHP)DrawRotaGraph(150, 30, 1.5, 0, Number[(int)PlayerHP / 100 % 10], TRUE);
		DrawRotaGraph(218, 30, 1.5, 0, Number[(int)PlayerHP / 100 % 10], TRUE);
	}

	//10の位
	if(10 <= PlayerHP)DrawRotaGraph(167, 30, 1.5, 0, Number[(int)PlayerHP / 10 % 10], TRUE);
	DrawRotaGraph(235, 30, 1.5, 0, Number[(int)MaxHP / 10 % 10], TRUE);

	//1の位
	DrawRotaGraph(184, 30, 1.5, 0, Number[(int)PlayerHP % 10], TRUE);
	DrawRotaGraph(252, 30, 1.5, 0, Number[(int)MaxHP % 10], TRUE);

	DrawRotaGraph(201, 30, 1.5, 0, Number[10], TRUE);
}


