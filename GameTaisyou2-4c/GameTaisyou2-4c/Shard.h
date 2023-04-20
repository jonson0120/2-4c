#pragma once
#include "Item.h"
class Shard :
    public Item
{
private:
	bool CanGet;		//拾える範囲にいるか
	bool Getted;		//拾われたか

	int image;		//アイテム画像
	float size;		//画像サイズ比

	float fall;		//落下速度
	float Slide;	//横移動速度

	Range pos;		//アイテム座標
	int Height, Width;	//アイテムサイズ
	int MapData[MAP_HEIGHT][MAP_WIDTH];	//マップデータ

public:
	Shard(Range position);

	void Update(Player* player);
	void Draw(Range pos)const;

	void SetItem();

	ItemType GetType() { return Sh; }
	bool GetGet() { return Getted; }

	int GetDis(Range Player);

	void SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]);
};

