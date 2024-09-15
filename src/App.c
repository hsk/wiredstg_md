// App.c : アプリケーション
#include "Game.h"
// 変数の定義
char appState;// 状態
void AppInitialize(void) {
    // アプリケーションの初期化
    appState = APP_STATE_GAME_INITIALIZE;// 状態の初期化
}
// アプリケーションを更新する
void AppUpdate(void) {
    char a = appState;// 状態の取得
    if (a == APP_STATE_GAME_INITIALIZE)      GameInitialize();  // ゲームの初期化
    else /*if (a == APP_STATE_GAME_UPDATE)*/ GameUpdate();      // ゲームの更新
}
