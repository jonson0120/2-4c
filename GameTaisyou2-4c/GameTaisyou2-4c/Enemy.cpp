#include"Enemy.h"
#include"DxLib.h"
#include"common.h"

Enemy::Enemy() {
	image = 0;

	enex = 2000; // 176 一番左下にするための座標
	eney = 1000; //1423 一番左下にするための座標

	Width = 64;
	Height = 64;

	direction = 0;

	speed = 6;
	fall = 16;
	jump = 0;

	LoadDivGraph("images/Enemy.png", 5, 16, 16, 32, 32, EImages);
}

void Enemy::Update() {

	//落下とジャンプ
	float fallinit = 16;
	eney += fall;
	while (!MapData[(eney + Height / 2) / 160][(enex - Width / 2) / 160])
	{
		eney--;
		jump = 0;
	}

	//エネミーの移動処理
	enex += speed;
	if (direction == 0)
	{
		direction = GetRand(60) + 60;
		speed *= -1;
	}
	else
	{
		direction--;
	}

	if (!MapData[eney / 160][(enex + Width / 2) / 160]) 
	{
		while (!MapData[eney / 160][(enex + Width / 2) / 160])enex--;
		speed *= -1;
	}

	if (!MapData[eney / 160][(enex - Width / 2) / 160])
	{
		while (!MapData[eney / 160][(enex - Width / 2) / 160])enex++;
		speed *= -1;
	}
}

void Enemy::Draw(int x,int y) const {
	//敵の表示
	DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
		enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2),EImages[0],TRUE);

}

void Enemy::SetMapData(int MapData[MAP_HEIGHT][MAP_WIDTH]) {

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			this->MapData[i][j] = MapData[i][j];
		}
	}
}