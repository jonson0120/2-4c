#include "DxLib.h"
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
    SetWindowIconID(01);

    if (DxLib_Init() == -1) return -1;	//DXライブラリの初期化処理

    SetDrawScreen(DX_SCREEN_BACK);  //描画先画面を裏にする

    SetFontSize(20);  //文字サイズを設定

    //後で使うよ

    //音声データを読み込む
    int BgmSet[7];
 
    BgmSet[TITLE] = LoadSoundMem("sound/TitleBGM.mp3");
    BgmSet[HOWTO] = LoadSoundMem("sound/Tutorial2.mp3");
    BgmSet[DUNGEON] = LoadSoundMem("sound/Dungeon.mp3");
    BgmSet[BOSS] = LoadSoundMem("sound/BOSS.mp3");
    BgmSet[SAFEZONE] = LoadSoundMem("sound/SafeZone.mp3");
    BgmSet[GAMECLEAR] = LoadSoundMem("sound/GameClear.mp3");
    BgmSet[GAMEOVER] = LoadSoundMem("sound/GameOver.mp3");

    SceneManager sceneMng((AbstractScene*)new Title(BgmSet));

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