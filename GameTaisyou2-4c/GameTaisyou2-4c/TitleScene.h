#pragma once
#include"AbstractScene.h"
#include"PadInput.h"

class Title : public AbstractScene {
private:
	enum class TITLE_MENU {
		START,
		END
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
	int End_font;

	int CursorSE;
	int TitleBGM;

	bool Tutorial_flg;

	int click_sound;

	int BgmSet[7];	//読み込まれたBGMデータ

public:
	Title(int BgmSet[7]);

	~Title() {};

	//�`��ȊO�̍X�V���������
	virtual AbstractScene* Update()override;

	//�`��Ɋւ��邱����������
	virtual void Draw()const override;
};