#pragma once
#include "AbstractScene.h"
class GameOver :public AbstractScene
{
private:
	int GameOverImage;

public:
	~GameOver() {};
	GameOver();

	//•`‰æˆÈŠO‚ÌXV‚ğÀ‘•‚·‚é
	virtual AbstractScene* Update() override;

	//•`‰æ‚ÉŠÖ‚·‚é‚±‚Æ‚ğÀ‘•‚·‚é
	virtual void Draw() const override;
};

