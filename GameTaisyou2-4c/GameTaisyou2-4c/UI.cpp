#include"UI.h"
#include"DxLib.h"
#include"PadInput.h"

UI::UI()
{
	DaggerImage = LoadGraph("images/Dagger.png");

	PotionImage1 = LoadGraph("images/potion2.png");
	PotionImage2 = LoadGraph("images/potion2.png");
	PotionImage3 = LoadGraph("images/potion2.png");

	Damage = 0;

	PlayerHP = 60;
	MaxHP=510;

	flag = false;

	PlayerHP = 110+((MaxHP - 110) / 100 * PlayerHP);
	
	//ポーション使用回数
	PotionCount = 0;
}

void UI::Update() 
{
	
	if ( 0< PotionCount)
	{
		flag = true;
	}

	if (PAD_INPUT::OnClick(XINPUT_BUTTON_DPAD_LEFT) && flag==true)
	{

		PotionCount=0;

	}

	//Xボタンを押すと回復(3回まで使用可能)
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_X) &&   PotionCount<3)
	{

		PotionCount++;
		
		PlayerHP = PlayerHP +((MaxHP-110)/100*20);
	}

	//HPは100以上回復しない
	if (MaxHP <PlayerHP)
	{
		PlayerHP = MaxHP;
	}

	//HPは0以下にはならない
	if (PlayerHP <= 110)
	{
		PlayerHP = 110;
	}


	//@
	if (PAD_INPUT::OnClick(XINPUT_BUTTON_DPAD_RIGHT))
	{
		Damage = GetRand(100);
		PlayerHP = PlayerHP - ((MaxHP - 110) / 100 * Damage);
	}
}

void UI::Draw() const
{

	DrawFormatString(0, 200, GetColor(255, 255, 255), "%d", PotionCount);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "%.1f", PlayerHP);
	DrawFormatString(0, 400, GetColor(255, 255, 255), "%d", Damage);

	//ポーション使用時の●
	DrawCircle(140, 75, 5, GetColor(255, 0, 0), TRUE);
	DrawCircle(195, 75, 5, GetColor(255, 0, 0), TRUE);
	DrawCircle(250, 75, 5, GetColor(255, 0, 0), TRUE);

	//ポーション画像の表示
	{

		if (PotionCount < 1)
		{
			DrawRotaGraph(250, 80, 0.3, 0, PotionImage1, TRUE);
		}
		if (PotionCount < 2)
		{
			DrawRotaGraph(195, 80, 0.3, 0, PotionImage2, TRUE);
		}
		if (PotionCount < 3)
		{
			DrawRotaGraph(140, 80, 0.3, 0, PotionImage3, TRUE);
		}

	}
	

	//武器
	DrawCircle(50, 50, 50, GetColor(255, 255, 255), TRUE);
	DrawRotaGraph(50, 50, 0.5, 0, DaggerImage, TRUE);

	//HP赤
	DrawBox(110,10,510,50,GetColor(255,0,0),TRUE);

	//HP緑
	DrawBox(110, 10, PlayerHP, 50, GetColor(0, 255, 0), TRUE);

	//HPバーの枠(白)
	DrawBox(110, 10, MaxHP, 50, GetColor(255,255, 255), FALSE);

}

