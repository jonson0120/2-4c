#include"TitleScene.h"
#include"HowToMap.h"
#include"GameMainScene.h"

Title::Title(int BgmSet[7]) {
	SetDrawBright(255, 255, 255);
	Menu_Number = TITLE_MENU::START;
	interval = 0;
	select = 0;
	JoyPadY = 0;

	Tutorial_flg = false;

	select_image = LoadGraph("images/select.png");
	mini_select_image = LoadGraph("images/select.png");
	Tutorial_font = LoadGraph("images/Tutorial.png");
	Title_font= LoadGraph("images/TITLE.png");
	Start_font= LoadGraph("images/START.png");
	Debug_font= LoadGraph("images/null.png");
	End_font = LoadGraph("images/End.png");

	click_sound = LoadSoundMem("sound/click.mp3");
	CursorSE = LoadSoundMem("sound/Cursor.mp3");

	this->BgmSet[TITLE] = BgmSet[TITLE];
	this->BgmSet[HOWTO] = BgmSet[HOWTO];
	this->BgmSet[DUNGEON] = BgmSet[DUNGEON];
	this->BgmSet[BOSS] = BgmSet[BOSS];
	this->BgmSet[SAFEZONE] = BgmSet[SAFEZONE];
	this->BgmSet[GAMECLEAR] = BgmSet[GAMECLEAR];
	this->BgmSet[GAMEOVER] = BgmSet[GAMEOVER];

	TitleBGM = BgmSet[TITLE];
	PlaySoundMem(TitleBGM, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(255 * 70 / 100, TitleBGM);

}

AbstractScene* Title::Update() {
	if (interval < 30)interval++;
	JoyPadY = PAD_INPUT::GetPadThumbLY();

	if (JoyPadY > MARGIN && interval >= 30) {
		PlaySoundMem(CursorSE, DX_PLAYTYPE_BACK);
		select--;
		interval = 0;
	}
	else if (JoyPadY < -MARGIN && interval >= 30) {
		PlaySoundMem(CursorSE, DX_PLAYTYPE_BACK);
		select++;
		interval = 0;
	}
	
	if (JoyPadY < MARGIN && -MARGIN < JoyPadY && !PAD_INPUT::OnPressed(XINPUT_BUTTON_B))interval = 30;

	if (select == 0) Menu_Number = TITLE_MENU::START;
	if (select == 1) Menu_Number = TITLE_MENU::END;

	if (select < 0)select = 1;
	if (select > 1)select = 0;

	if (Tutorial_flg == false) {
		if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && interval >= 30) {
			PlaySoundMem(click_sound, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			if (TITLE_MENU::START == Menu_Number) Tutorial_flg = true;
			if (TITLE_MENU::END == Menu_Number)return nullptr;
			interval = 0;
		}
	}
	if (Tutorial_flg == true) {
		if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && interval >= 30) {
			PlaySoundMem(click_sound, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			if (TITLE_MENU::START == Menu_Number) return new GameMainScene(BgmSet); //�Q�[�����X�^�[�g����
			if (TITLE_MENU::END == Menu_Number)return new HowToMap(BgmSet); //�`���[�g���A�����X�^�[�g����
			interval = 0;
		}

	}
	
	return this;
}

void Title::Draw() const{
	
	/*DrawFormatString(540, 100, 0xffffff, "TITLE");
	DrawFormatString(540, 360, 0xffffff, "START");
	DrawFormatString(540, 460, 0xffffff, "Debug");*/
	DrawRotaGraph(SCREEN_WIDTH / 2, 200, 1, 0, Title_font, TRUE);
	if (Tutorial_flg == false) {
		DrawGraph(440, 360, Start_font, TRUE);
		DrawGraph(475, 460, End_font, TRUE);
		DrawGraph(400, 360 + select * 100, select_image, TRUE);
	}
	else {
		DrawRotaGraph(630, 400, 2.0, 0, Tutorial_font, FALSE);
		DrawRotaGraph(550, 430 + select * 32, 0.5, 0, mini_select_image, TRUE);
	}
}