#pragma once
#include"AbstractScene.h"
#include"PadInput.h"

class Title : public AbstractScene {
private:
	enum class TITLE_MENU {
		START,
		Debug
	};

	TITLE_MENU Menu_Number;
	int interval;
	int select;
	int JoyPadY;

	int background_image;
	int select_image;
	int mini_select_image;
	int Tutorial_font;
	int Title_font;
	int Start_font;
	int Debug_font;

	bool Tutorial_flg;

	int click_sound;
public:
	Title();

	~Title() {};

	//•`‰æˆÈŠO‚ÌXV‚ğÀ‘•‚·‚é
	virtual AbstractScene* Update()override;

	//•`‰æ‚ÉŠÖ‚·‚é‚±‚ë‚ğÀ‘•‚·‚é
	virtual void Draw()const override;
};