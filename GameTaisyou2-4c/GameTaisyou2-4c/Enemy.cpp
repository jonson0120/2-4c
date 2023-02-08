#include"Enemy.h"
#include"DxLib.h"
#include"common.h"

Enemy::Enemy() {
	image = 0;

	enex = 2000; // 176 一番左下にするための座標
	eney = 1000; //1423 一番左下にするための座標

	Width = 64;
	Height = 64;

	speed = 0;
	fall = 16;
	jump = 0;

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
}

void Enemy::Draw(int x,int y) const {
	//敵の表示
	DrawExtendGraph(enex - (Width / 2) - x + (SCREEN_WIDTH / 2), eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
		enex + (Width / 2) - x + (SCREEN_WIDTH / 2), eney + (Height / 2) - y + (SCREEN_HEIGHT / 2),EnemyImage,TRUE);

	//DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			  //enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
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