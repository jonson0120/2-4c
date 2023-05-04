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
	LoadDivGraph("images/alphabet.png", 28, 7, 4, 10, 12, Chara);
	LoadDivGraph("images/sign.png", 4, 4, 1, 11, 11, Sign);

	LoadDivGraph("images/passive.png", 7, 1, 7, 83, 17, passiveimg);

	ItemImg[0] = LoadGraph("images/potion2.png");
	ItemImg[1] = LoadGraph("images/shard.png");

	LoadDivGraph("images/UiButton.png", 2, 2, 1, 21, 21, ButtonImg);

	LoadDivGraph("images/str.png", 6, 6, 1, 128, 128, UpGradeImg);
	LoadDivGraph("images/strui.png", 4, 1, 4, 112, 24, UpGradeTxt);

	Damage = 0;

	PlayerHP = 0;
	MaxHP = 0; 
	Shard = 0;

	cost[HP] = 5;
	cost[ATK] = 5;
	cost[HEAL] = 20;

	nowstat[HP] = 0;
	nowstat[ATK] = 0;
	nowstat[HEAL] = 0;

	potionflag = false;
	
}

void UI::Update(Player* player)
{
	stat = player->GetStat();
	PlayerHP = stat.Hp;
	MaxHP = stat.MaxHp;

	Shard = player->GetShard();

	PotionCount = player->GetPotion();
	PotionMax = player->GetPotionMax();

	UseCount = player->UseP();

	nowstat[HP] = stat.MaxHp;
	nowstat[ATK] = stat.Atk;
	nowstat[HEAL] = stat.PotionMax;

	for (int i = 0; i < 4; i++)
	{
		passive[i] = player->GetPassive(i);
	}

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

bool UI::UpGradeUI(Player* player) 
{
	int JoyPadX = PAD_INPUT::GetPadThumbLX();

	if (JoyPadX > MARGIN && WaitTime <= 0) {
		if (3 < ++MenuNum)MenuNum = 0;
		WaitTime = 20;
	}
	if (JoyPadX < -MARGIN && WaitTime <= 0) {
		if (--MenuNum < 0)MenuNum = 3;
		WaitTime = 20;
	}

	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B)) 
	{
		switch (MenuNum)
		{
		case 0:
			if (cost[HP] <= player->GetShard())
			{
				player->StrHP(5);
				player->UseShard(cost[HP]);
				cost[HP] += 5;
			}
			break;

		case 1:
			if (cost[ATK] <= player->GetShard())
			{
				player->StrAtk();
				player->UseShard(cost[ATK]);
				cost[ATK] += 5;
			}
			break;

		case 2:
			if (cost[HEAL] <= player->GetShard() && player->GetPotionMax() < 5)
			{
				player->StrHeal();
				player->UseShard(cost[HEAL]);
				cost[HEAL] += 20;
			}
			break;

		case 3:
			MenuNum = 0;
			return false;
			break;

		default:
			break;
		}
	}

	if (--WaitTime < 0 || (-MARGIN < JoyPadX && JoyPadX < MARGIN))WaitTime = 0;
	return true;
}

//アップグレード画面
void UI::UpGradeDraw() const
{
	int Dis = 140;

	//カーソル
	DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * MenuNum, SCREEN_HEIGHT / 2, 1, 0, UpGradeImg[4], TRUE);

	//強化費用表示
	for (int i = 0; i < 4; i++)
	{
		DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i, SCREEN_HEIGHT / 2, 1, 0, UpGradeImg[i], TRUE);
		if (i == MenuNum)
		{
			DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i, SCREEN_HEIGHT / 2 - 120, 1, 0, UpGradeTxt[i], TRUE);
			if (i != 3)
			{
				//強化費用表示
				if (i == 2 && 5 <= nowstat[2]) {}
				//3桁の場合
				else if (cost[i] >= 100)
				{
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 + 90, 2, 0, Number[cost[i] / 100 % 10], TRUE);
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 + 90, 2, 0, Number[cost[i] / 10 % 10], TRUE);
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 60, SCREEN_HEIGHT / 2 + 90, 2, 0, Number[cost[i] % 10], TRUE);
				}
				//2桁の場合
				else if (cost[i] >= 10)
				{
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 + 90, 2, 0, Number[cost[i] / 10 % 10], TRUE);
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 + 90, 2, 0, Number[cost[i] % 10], TRUE);
				}
				//1桁の場合
				else
				{
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 + 90, 2, 0, Number[cost[i] % 10], TRUE);
				}

				//現在ステータスの表示
				//3桁の場合
				if (!(i == 2 && 5 <= nowstat[2]))
				{
					if (nowstat[i] >= 100)
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 64, SCREEN_HEIGHT / 2 - 70, 1, 0, Number[nowstat[i] / 100 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 52, SCREEN_HEIGHT / 2 - 70, 1, 0, Number[nowstat[i] / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 40, SCREEN_HEIGHT / 2 - 70, 1, 0, Number[nowstat[i] % 10], TRUE);
					}
					//2桁の場合
					else if (nowstat[i] >= 10)
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 52, SCREEN_HEIGHT / 2 - 70, 1, 0, Number[nowstat[i] / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 40, SCREEN_HEIGHT / 2 - 70, 1, 0, Number[nowstat[i] % 10], TRUE);
					}
					//1桁の場合
					else
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 40, SCREEN_HEIGHT / 2 - 70, 1, 0, Number[nowstat[i] % 10], TRUE);
					}

					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 20, SCREEN_HEIGHT / 2 - 70, 2, 0, Sign[3], TRUE);
				}

				//強化後ステータスの表示
				switch (i)
				{
				case 0:	//体力
					//3桁の場合
					if (nowstat[i] + 5 >= 100)
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 5) / 100 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 5) / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 60, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 5) % 10], TRUE);
					}
					//2桁の場合
					else
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 5) / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 5) % 10], TRUE);
					}
					break;

				case 1:	//攻撃力
				//3桁の場合
					if (nowstat[i] + 1 >= 100)
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) / 100 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 60, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) % 10], TRUE);
					}
					//2桁の場合
					else if (nowstat[i] + 1 >= 10)
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) % 10], TRUE);
					}
					//1桁の場合
					else
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) % 10], TRUE);
					}
					break;

				case 2:	//ポーション
				//最大強化の場合
					if (5 <= nowstat[i])
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i]) % 10], TRUE);
					}
					else if (nowstat[i] + 1 >= 10)
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) / 10 % 10], TRUE);
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 35, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) % 10], TRUE);
					}
					else
					{
						DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i + 10, SCREEN_HEIGHT / 2 - 80, 2, 0, Number[(nowstat[i] + 1) % 10], TRUE);
					}
					break;

				default:
					break;
				}

				//シャード、記号
				if (!(i == 2 && 5 <= nowstat[2])) {
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 10, SCREEN_HEIGHT / 2 + 95, 1, 0, Sign[0], TRUE);
					DrawRotaGraph(SCREEN_WIDTH / 2 - Dis * 1.5 + Dis * i - 30, SCREEN_HEIGHT / 2 + 90, 1, 0, ItemImg[1], TRUE);
				}
			}
		}
	}

}

Pause UI::PauseUI()
{
	int JoyPadY = PAD_INPUT::GetPadThumbLY();
	static bool GoTitle = false;

	Title = GoTitle;

	if (JoyPadY > MARGIN && WaitTime <= 0) {
		if (1 < ++MenuNum)MenuNum = 0;
		WaitTime = 20;
	}
	if (JoyPadY < -MARGIN && WaitTime <= 0) {
		if (--MenuNum < 0)MenuNum = 1;
		WaitTime = 20;
	}

	if (PAD_INPUT::OnClick(XINPUT_BUTTON_B))
	{
		switch (MenuNum)
		{
		case 0:
			if (!GoTitle)return Pause::RETURN;
			else GoTitle = false;
			break;

		case 1:
			if (!GoTitle) {
				MenuNum = 0;
				GoTitle = true;
			}
			else {
				GoTitle = false;
				return Pause::TITLE;
			}
			break;

		default:
			break;
		}
	}

	if (--WaitTime < 0 || (-MARGIN < JoyPadY && JoyPadY < MARGIN))WaitTime = 0;
	return Pause::NONE;
}

void UI::PauseDraw() const
{
	int Dis = 100;

	//カーソル
	DrawExtendGraph(SCREEN_WIDTH / 2 - Dis * 2, SCREEN_HEIGHT / 2 - Dis + (Dis * 2 * MenuNum) ,
					SCREEN_WIDTH / 2 + Dis * 2, SCREEN_HEIGHT / 2 + (Dis * 2 * MenuNum), UpGradeImg[5], true);
	
	if (!Title) 
	{
		//PAUSE
		DrawRotaGraph(SCREEN_WIDTH / 2 - 120, 150, 5, 0, Chara[15], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 - 60, 150, 5, 0, Chara[0], true);
		DrawRotaGraph(SCREEN_WIDTH / 2, 150, 5, 0, Chara[20], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 60, 150, 5, 0, Chara[18], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 120, 150, 5, 0, Chara[4], true);

		//RETURN
		DrawRotaGraph(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - Dis / 2 , 5, 0, Chara[17], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[4], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[19], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[20], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 90, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[17], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[13], true);

		//TITLE
		DrawRotaGraph(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[19], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[8], true);
		DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[19], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[11], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 120, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[4], true);
	}
	else 
	{
		//REALLY?
		DrawRotaGraph(SCREEN_WIDTH / 2 - 180, 150, 5, 0, Chara[17], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 - 120, 150, 5, 0, Chara[4], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 - 60, 150, 5, 0, Chara[0], true);
		DrawRotaGraph(SCREEN_WIDTH / 2, 150, 5, 0, Chara[11], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 60, 150, 5, 0, Chara[11], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 120, 150, 5, 0, Chara[24], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 180, 150, 5, 0, Chara[27], true);

		//NO
		DrawRotaGraph(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[13], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - Dis / 2, 5, 0, Chara[14], true);

		//YES
		DrawRotaGraph(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[24], true);
		DrawRotaGraph(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[4], true);
		DrawRotaGraph(SCREEN_WIDTH / 2 + 60, SCREEN_HEIGHT / 2 + (Dis * 1.5), 5, 0, Chara[18], true);

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

	//ポーション画像の表示---------------

	for (int i = 0; i < PotionCount; i++)
	{
		DrawRotaGraph(165 + (i * 55), 80, 0.3, 0, ItemImg[0], TRUE);
	}

	DrawRotaGraph(140, 100, 1.2, 0, Gauge[0], true);
	DrawCircleGauge(140, 100, 100 * (UseCount / 60.f), Gauge[2], 0, 1.1, false, false);
	DrawRotaGraph(140, 100, 0.9, 0, ButtonImg[0], TRUE);
	//-----------------------------------

	//武器-------------------------------------------------
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

	DrawRotaGraph(15, 50, 1.5, 0, ButtonImg[1], TRUE);

	//パッシブ
	for (int i = 0; i < 4; i++)
	{
		if (passive[i].Kinds != NONE)
		{
			DrawRotaGraph2(15, 125 + (27 * i), 0, 9, 1.5, 0, passiveimg[passive[i].Kinds], TRUE);
			DrawRotaGraph(180, 125 + (27 * i), 1.5, 0, Number[passive[i].Effect], true);
		}
	}
	//-----------------------------------------------------

	//HP赤
	DrawBox(135, 10, 535, 50, GetColor(255, 0, 0), TRUE);

	//HP緑
	DrawBox(135, 10, 135 + (PlayerHP / MaxHP) * 400, 50, GetColor(0, 255, 0), TRUE);

	//HPバーの枠(白)
	DrawBox(135, 10, 135 + (PlayerHP / MaxHP) * 400, 50, GetColor(255, 255, 255), FALSE);

	//HP表示
	int danger = 0;
	if (PlayerHP / MaxHP <= 0.21)danger = 33;
	else if (PlayerHP / MaxHP <= 0.41)danger = 22;

	//100の位
	if (100 <= MaxHP)
	{
		if (100 <= PlayerHP)DrawRotaGraph(150, 30, 1.5, 0, Number[(int)PlayerHP / 100 % 10 + danger], TRUE);
		DrawRotaGraph(218, 30, 1.5, 0, Number[(int)PlayerHP / 100 % 10], TRUE);
	}

	//10の位
	if(10 <= PlayerHP)DrawRotaGraph(167, 30, 1.5, 0, Number[(int)PlayerHP / 10 % 10 + danger], TRUE);
	DrawRotaGraph(235, 30, 1.5, 0, Number[(int)MaxHP / 10 % 10 + danger], TRUE);

	//1の位
	DrawRotaGraph(184, 30, 1.5, 0, Number[(int)PlayerHP % 10 + danger], TRUE);
	DrawRotaGraph(252, 30, 1.5, 0, Number[(int)MaxHP % 10 + danger], TRUE);

	//スラッシュ
	DrawRotaGraph(201, 30, 1.5, 0, Number[10 + danger], TRUE);


	//シャード所持数
	//シャード
	DrawRotaGraph(225, 125, 1, 0, ItemImg[1], TRUE);

	//×印
	DrawRotaGraph(245, 130, 1, 0, Sign[0], TRUE);

	//100の位
	if (100 <= Shard) DrawRotaGraph(262, 125, 1.5, 0, Number[Shard / 100 % 10], TRUE);

	//10の位
	if (100 <= Shard)DrawRotaGraph(279, 125, 1.5, 0, Number[Shard / 10 % 10], TRUE);
	else if (10 <= Shard)DrawRotaGraph(262, 125, 1.5, 0, Number[Shard / 10 % 10], TRUE);

	//1の位
	if (100 <= Shard)DrawRotaGraph(296, 125, 1.5, 0, Number[Shard % 10], TRUE);
	else if (10 <= Shard)DrawRotaGraph(279, 125, 1.5, 0, Number[Shard % 10], TRUE);
	else DrawRotaGraph(262, 125, 1.5, 0, Number[Shard % 10], TRUE);

}


