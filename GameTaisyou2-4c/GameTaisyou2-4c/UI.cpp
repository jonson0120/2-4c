#include"UI.h"
#include"DxLib.h"
#include"PadInput.h"
#include"Player.h"


UI::UI()
{
	stat = { 0,0,0 };
	PotionCount = 0;

	Weapon = 0;

	DaggerImage = LoadGraph("images/Dagger.png");
	MaceImage = LoadGraph("images/mace.png");
	SpearImage=LoadGraph("images/spear.png");
	KatanaImage=LoadGraph("images/katana.png");
	

	PotionImage1 = LoadGraph("images/potion2.png");
	PotionImage2 = LoadGraph("images/potion2.png");
	PotionImage3 = LoadGraph("images/potion2.png");

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
	for (int i = 0; i < PotionCount; i++) 
	{
		DrawCircle(140 + (i * 55), 75, 5, GetColor(255, 0, 0), TRUE);
	}

	//ポーション画像の表示

	for (int i = 0; i < PotionCount; i++)
	{
		DrawRotaGraph(140 + (i * 55), 80, 0.3, 0, PotionImage1, TRUE);
	}

	//武器
	DrawCircle(50, 50, 50, GetColor(35, 59, 108), TRUE);
	switch (Weapon)
	{
	case 0:
		DrawRotaGraph(50, 50, 0.5, 0, DaggerImage, TRUE);
		break;
	case 1:
		DrawRotaGraph(50, 50, 0.2, 0, MaceImage, TRUE);
		break;
	case 2:
		DrawRotaGraph(50, 50, 0.2, 0, SpearImage, TRUE);
		break;
	case 3:
		DrawRotaGraph(50, 50, 0.25, 0, KatanaImage, TRUE);
		break;

	}

	//HP赤
	DrawBox(110, 10, 510, 50, GetColor(255, 0, 0), TRUE);

	//HP緑
	DrawBox(110, 10, 110 + (PlayerHP / MaxHP) * 400, 50, GetColor(0, 255, 0), TRUE);

	//HPバーの枠(白)
	DrawBox(110, 10, 110 + (PlayerHP / MaxHP) * 400, 50, GetColor(255, 255, 255), FALSE);

}


