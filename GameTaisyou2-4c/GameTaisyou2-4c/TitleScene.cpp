#include"TitleScene.h"
#include"GameMainScene.h"
#include"TestMap.h"

Title::Title() {
	Menu_Number = TITLE_MENU::START;
	interval = 0;
	select = 0;
	JoyPadY = 0;

	select_image = LoadGraph("images/select.png");
}

AbstractScene* Title::Update() {
	if (interval < 30)interval++;
	JoyPadY = PAD_INPUT::GetPadThumbLY();

	if (JoyPadY > MARGIN && interval >= 30) {
		select--;
		interval = 0;
	}
	if (JoyPadY < -MARGIN && interval >= 30) {
		select++;
		interval = 0;
	}

	if (select == 0) Menu_Number = TITLE_MENU::START;
	if (select == 1) Menu_Number = TITLE_MENU::Debug;

	if (select < 0)select = 1;
	if (select > 1)select = 0;

	if (PAD_INPUT::OnPressed(XINPUT_BUTTON_B) && interval >= 30) {
		if (TITLE_MENU::START == Menu_Number)return new GameMainScene();
		if (TITLE_MENU::Debug == Menu_Number)return new TestMap();
		interval = 0;
	}

	return this;
}

void Title::Draw() const{
	
	DrawFormatString(540, 100, 0xffffff, "TITLE");
	DrawFormatString(540, 360, 0xffffff, "START");
	DrawFormatString(540, 460, 0xffffff, "Debug");
	DrawGraph(400, 310 + select * 100, select_image, TRUE);

	DrawFormatString(0, 500, 0xff0000, "%d", select);
	DrawFormatString(0, 550, 0xff0000, "%d", interval);
	DrawFormatString(0, 600, 0xff0000, "%d", JoyPadY);;
}