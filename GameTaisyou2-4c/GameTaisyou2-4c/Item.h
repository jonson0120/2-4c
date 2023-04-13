#pragma once
#include"DxLib.h"
#include"Player.h"
#include"common.h"

class Item
{
private:
	int Type;		//アイテム種類 1:武器
	weapons Weapon;	//武器種類	
	bool CanGet;		//拾える範囲にいるか
	bool Getted;		//同フレームにおいて拾われたか

	int image;		//アイテム画像
	int icon;		//アイコン画像
	float size;		//画像サイズ比

	float fall;		//落下速度

	Range pos;		//アイテム座標
	int Height, Width;	//アイテムサイズ
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

public :
	Item(int Type, weapons Weapon, Range position);

	void Update(Player* player);
	void Draw(Range pos)const;

	void SetItem();

	int GetType() { return Type; }
	bool GetGet() { return Getted; }

	weapons GetweaponType() { return Weapon; }
	int GetDis(Range Player);

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
};

