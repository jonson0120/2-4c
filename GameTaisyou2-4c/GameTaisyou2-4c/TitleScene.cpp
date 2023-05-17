#include"TitleScene.h"
#include"HowToMap.h"
#include"GameMainScene.h"
#include"TestMap.h"

Title::Title() {
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

	click_sound = LoadSoundMem("sound/click.mp3");
	CursorSE = LoadSoundMem("sound/Cursor.mp3");

	TitleBGM = LoadSoundMem("sound/TitleBGM.mp3");
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
	if (select == 1) Menu_Number = TITLE_MENU::Debug;

	if (select < 0)select = 1;
	if (select > 1)select = 0;

	if (Tutorial_flg == false) {
		if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && interval >= 30) {
			PlaySoundMem(click_sound, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			if (TITLE_MENU::START == Menu_Number) Tutorial_flg = true;
			if (TITLE_MENU::Debug == Menu_Number)return new TestMap();
			interval = 0;
		}
	}
	if (Tutorial_flg == true) {
		if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && interval >= 30) {
			PlaySoundMem(click_sound, DX_PLAYTYPE_BACK);
			StopSoundMem(TitleBGM);
			if (TITLE_MENU::START == Menu_Number) return new GameMainScene(); //�Q�[�����X�^�[�g����
			if (TITLE_MENU::Debug == Menu_Number)return new HowToMap(); //�`���[�g���A�����X�^�[�g����
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
		DrawGraph(440, 460, Debug_font, TRUE);
		DrawGraph(400, 360 + select * 100, select_image, TRUE);
	}
	else {
		DrawRotaGraph(630, 400, 2.0, 0, Tutorial_font, FALSE);
		DrawRotaGraph(550, 430 + select * 32, 0.5, 0, mini_select_image, TRUE);
	}
	DrawFormatString(0, 500, 0xff0000, "%d", select);
	DrawFormatString(0, 550, 0xff0000, "%d", interval);
	DrawFormatString(0, 600, 0xff0000, "%d", JoyPadY);;
}