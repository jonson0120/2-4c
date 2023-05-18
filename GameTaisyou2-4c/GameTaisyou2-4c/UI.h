#pragma once
#include "Player.h"
#include"PadInput.h"

enum Price
{
	HP,
	ATK,
	HEAL,
};

class UI
{
private:
	Stat stat;

	float MaxHP;
	float PlayerHP;

	int Shard;

	bool potionflag;

	int Damage;


	int Weapon;	//���ݑ������Ă��镐��
	int Second;	//���ݑ������Ă��Ȃ�����

	int DaggerImage;
	int MaceImage;
	int SpearImage;
	int KatanaImage;
	int NoneImage;
	 
	int Gauge[3];

	int ButtonImg[2];

	int ItemImg[2];

	int Number[44];		//����
	int Chara[28];		//����
	int Sign[4];		//����

	Passive passive[4];	//�\������p�b�V�u
	int passiveimg[7];	//�p�b�V�u�摜

	int PotionCount;
	int PotionMax;

	int UseCount = 0;

	int MenuNum = 0;	//���j���[�F�I�����

	int UpGradeImg[6];	//�������j���[�F�摜
	int UpGradeTxt[4];	//�������j���[�F����

	int WaitTime = 0;		//���͑҂�����

	int cost[3];		//�����F�K�v�R�X�g
	int nowstat[3];		//�����F���݂̒l
	bool Title = false;		//�|�[�Y�F�ŏI�m�F

	int StrengthenSE;
	int CursorSE;
	int CancelSE;

public:
	
	UI();
	void Update(Player* player);

	//�������j���[
	bool UpGradeUI(Player* player);

	Pause PauseUI();

	void Draw()const;
	void UpGradeDraw()const;
	void PauseDraw()const;
};

