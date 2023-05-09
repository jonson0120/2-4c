#pragma once
#include"DxLib.h"
#include"Player.h"
#include"Item.h"
#include"common.h"

class Weapon : public Item
{
private:
	weapons Kind;	//武器種類	
	Passive passive[4];	//付与効果
	bool CanGet;		//拾える範囲にいるか
	bool Getted;		//同フレームにおいて拾われたか

	int image;		//アイテム画像
	int passiveimg[8];	//パッシブ画像
	int numimg[44];		//数字画像
	int icon;		//アイコン画像
	float size;		//画像サイズ比

	float fall;		//落下速度

	Range pos;		//アイテム座標
	int Height, Width;	//アイテムサイズ
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

public :
	Weapon(weapons Weapon, Range position, int Level);
	void SetPassive(int Level);

	void Update(Player* player);
	void Draw(Range pos)const;

	void SetItem();

	bool GetGet() { return Getted; }
	ItemType GetType() { return Equip; }

	weapons GetweaponType() { return Kind; }
	int GetDis(Range Player);

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
};

