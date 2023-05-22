#pragma once
#include "AbstractScene.h"
class GameEnd :public AbstractScene
{
private:
	int GameEndImage;
	int GameClearBGM;

	int BgmSet[7];
public:
	~GameEnd() {};
	GameEnd(int BgmSet[7]);

	//�`��ȊO�̍X�V���������
	virtual AbstractScene* Update() override;

	//�`��Ɋւ��邱�Ƃ��������
	virtual void Draw() const override;
};
