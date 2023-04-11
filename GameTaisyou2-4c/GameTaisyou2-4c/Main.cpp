﻿#include "DxLib.h"
#include "common.h"
#include"SceneManager.h"
#include"GameMainScene.h"
#include"PadInput.h"
#include"TitleScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetMainWindowText("ここにゲーム名");  //タイトル設定
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_BIT_16);

    ChangeWindowMode(TRUE);		//ウィンドウモードで起動

    if (DxLib_Init() == -1) return -1;	//DXライブラリの初期化処理

    SetDrawScreen(DX_SCREEN_BACK);  //描画先画面を裏にする

    SetFontSize(20);  //文字サイズを設定

    //後で使うよ

    SceneManager sceneMng((AbstractScene*)new Title());
  

    //ゲームループ
    while (ProcessMessage() == 0 && sceneMng.Update() != nullptr)
    {
        ClearDrawScreen();  //画面の初期化

        sceneMng.Draw();
        ScreenFlip();  //裏画面の培養を表画面に反映

        PAD_INPUT::UpdateKey();
    }

    DxLib_End();  //DXライブラリ使用の終了処理

    return 0;
}