#include"Enemy.h"
#include"DxLib.h"
#include"common.h"

Enemy::Enemy() {
	image = 0;

	enex = 2000; // 176 一番左下にするための座標
	eney = 1423; //1423 一番左下にするための座標

	Width = 32;
	Height = 32;

	speed = 0;
	fall = 16;
	jump = 0;
}

void Enemy::Update() {

}

void Enemy::Draw(int x,int y) const {
	//敵の表示
	DrawBoxAA(enex - (Width / 2) - x + (SCREEN_WIDTH / 2) , eney - (Height / 2) - y + (SCREEN_HEIGHT / 2),
			  enex + (Width / 2) - x + (SCREEN_WIDTH / 2) , eney + (Height / 2) - y + (SCREEN_HEIGHT / 2), 0x00ff00, TRUE);
}