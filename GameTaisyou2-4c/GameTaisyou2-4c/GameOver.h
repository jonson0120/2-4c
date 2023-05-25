#pragma once
#include "AbstractScene.h"
class GameOver :public AbstractScene
{
private:
	int GameOverImage;
	int GameOverBGM;

	int BgmSet[7];
public:
	~GameOver() {};
	GameOver(int BgmSet[7]);

	//�`��ȊO�̍X�V���������
	virtual AbstractScene* Update() override;

	//�`��Ɋւ��邱�Ƃ��������
	virtual void Draw() const override;
};

