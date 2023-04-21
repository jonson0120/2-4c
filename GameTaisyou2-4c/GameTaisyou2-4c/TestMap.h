#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
#include "Enemy2.h"
#include"UI.h"
#include"TreasureBox.h"


class TestMap : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v�f�[�^
	int CheckData[MAP_HEIGHT][MAP_WIDTH];	//��ԃ`�F�b�N�p�}�b�v�f�[�^
	int MapImg[4];			//�}�b�v�摜
	int MapExitX; //�}�b�v�̏o����X���W
	int MapExitY; //�}�b�v�̏o����Y���W

	int Art[7];		//�G�A�C�R���Ȃ�
	int info[3];	//�ē���
	int MapType;	//�}�b�v�n�`�^�C�v 0:���R 1:�i�� 2:�K�i

	int time;
	Player player;
	Enemy** enemy;
	Item** item;
	//Enemy2 enemy2;
	UI ui;

	int CameraX;
	int CameraY;

	int hit = 0;

	int Bright; //�P�x
	int Bright_minus; //�P�x���������߂̂���

	bool Exit_flg; //�o���ɂ��Ă邩�𔻒肷��
	int AnimTimer; //�A�j���[�V�����p�̃^�C�}�[
	bool Anim_flg; //�A�j���[�V�����p�̃t���O
	bool MakeMap_flg; //�}�b�v����邩���f����
	bool MoveStop_flg; //�v���C���[�̍s������t���O
	bool Pressed_flg; //�o���ɒ��������Ƀ{�^�����������͂��󂯓���邩���肷��

	int x, y;

	int count;
public:
	TestMap();
	//�f�X�g���N�g
	virtual ~TestMap()
	{
		//delete player;//new���������delete�����
	};
	//�`��ȊO�̍X�V����������
	virtual AbstractScene* Update()override;

	void SortEnemy();
	void SortItem();

	//�`��Ɋւ��邱�����������
	virtual void Draw()const override;

	void MakeMap();
	int CheckSpace(int y, int x, int* cnt);
	void NextMap();
	void ExitCheck();

	void SearchEnemy();
};