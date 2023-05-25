﻿#include "DxLib.h"
#include "Fps.h"
#include "common.h"
#include"SceneManager.h"
#include"GameMainScene.h"
#include"PadInput.h"
#include"TitleScene.h"

#define FLAME 60

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    SetMainWindowText("TOMB OF BEAST");  //タイトル設定
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_BIT_16);

    ChangeWindowMode(TRUE);		//ウィンドウモードで起動

    if (DxLib_Init() == -1) return -1;	//DXライブラリの初期化処理

    SetDrawScreen(DX_SCREEN_BACK);  //描画先画面を裏にする

    SetFontSize(20);  //文字サイズを設定

    SetWindowIconID(333);

    //後で使うよ

    SceneManager sceneMng((AbstractScene*)new Title());
  

    Fps fps;

    //ゲームループ
    while (ProcessMessage() == 0 && sceneMng.Update() != nullptr)
    {
        ClearDrawScreen();  //画面の初期化

        sceneMng.Draw();

        fps.Update();	//更新
        //fps.Draw();		//描画

        ScreenFlip();  //裏画面の培養を表画面に反映

        PAD_INPUT::UpdateKey();

            fps.Wait();		//待機
    }

    DxLib_End();  //DXライブラリ使用の終了処理

    return 0;
}