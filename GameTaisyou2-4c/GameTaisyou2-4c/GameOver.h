#pragma once
#include "AbstractScene.h"
class GameOver :public AbstractScene
{
private:
	int GameOverImage;
	int GameOverBGM;
public:
	~GameOver() {};
	GameOver();

	//�`��ȊO�̍X�V���������
	virtual AbstractScene* Update() override;

	//�`��Ɋւ��邱�Ƃ��������
	virtual void Draw() const override;
};

