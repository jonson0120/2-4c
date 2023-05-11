#pragma once
#pragma once
#include "AbstractScene.h"
#include"common.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
#include "Enemy2.h"
#include"UI.h"
#include"TreasureBox.h"


class GameMainScene : public AbstractScene
{
private:
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ
	int MaplimitX;						//マップ制限
	int MaplimitY;						//マップ制限

	int CheckData[MAP_HEIGHT][MAP_WIDTH];	//空間チェック用マップデータ
	int MapImg[4];			//マップ画像
	int MapExitX; //マップの出口のX座標
	int MapExitY; //マップの出口のY座標

	int Level;		//階層
	bool SafeZone;	//今いるのは安全地帯?
	bool UpGrade = false;	//アップグレードメニューを開く?
	bool Pause = false;		//ポーズメニューを開く？

	int time;
	Player player;				//プレイヤー
	Enemy** enemy;				//敵
	LocNum Damage[ENEMY_MAX + 1];	//ダメージ(受けた位置・ダメージ量・表示時間) 0:プレイヤー 1以降:敵

	Item** item;				//アイテム
	//Enemy2 enemy2;
	UI ui;
	TreasureBox** treasurebox;

	int CameraX;
	int CameraY;

	int hit = 0;

	int Bright; //輝度
	int Bright_minus; //輝度を引くためのもの

	bool Exit_flg; //出口についてるかを判定する
	int AnimTimer; //アニメーション用のタイマー
	bool Anim_flg; //アニメーション用のフラグ
	bool MakeMap_flg; //マップを作るか判断する
	bool MoveStop_flg; //プレイヤーの行動制御フラグ
	bool Pressed_flg; //出口に着いた時にボタン長押し入力を受け入れるか判定する

	int DoorIcon[4];	//出口などに表示するアイコン

	int x, y;

	int count;

	int hierarchy_font[44]; //階層用フォント画像変数
	int Chara[28];		//文字


	int DoorSE;
	int NextMapSE;
	int AttackSE;
	int TreasureSE;
	int DamageSE;

	int DungeonBGM;

public:
	GameMainScene();
	//デストラクト
	virtual ~GameMainScene()
	{
		//delete player;//newを作ったらdeleteを作る
	};
	//描画以外の更新を実装する
	virtual AbstractScene* Update()override;

	void SortEnemy();
	void SortItem();

	//描画に関するころを実装する
	virtual void Draw()const override;		//通常の描画
	void DrawDamage(LocNum LocDmg, int num) const;	//ダメージ描画(ダメージ内容・配列番号)

	void MakeMap();
	int CheckSpace(int y, int x, int* cnt);
	void NextMap();
	void ExitCheck();

	//敵を出現させる
	void MakeEnemy();

	void SearchEnemy();
};