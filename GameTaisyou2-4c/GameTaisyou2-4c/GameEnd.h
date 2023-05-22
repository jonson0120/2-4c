#pragma once
#include "AbstractScene.h"
class GameEnd :public AbstractScene
{
private:
	int GameEndImage;
	int GameClearBGM;

	//アニメーション管理
	int Anim = 0;			//アニメーション経過時間
	int Scene = 0;			//場面
	int Bright = 0;			//フェードアウト(0で完全に暗転)

	int MainPos;			//主人公の立ち位置
	int BlockPos;			//ブロックの位置
	int StrPos;				//文字の位置

	bool Fade = false;				//シーン終了時にフェードアウト

	//画像
	int BootyImg[4];	//荷車を引く主人公
	int Block[2];		//地面
	int String[4];		//文字

	int BgmSet[7];
public:
	~GameEnd() {};
	GameEnd(int BgmSet[7]);

	//�`��ȊO�̍X�V���������
	virtual AbstractScene* Update() override;

	//�`��Ɋւ��邱�Ƃ��������
	virtual void Draw() const override;
};
