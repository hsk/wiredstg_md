// Game.c : ゲーム
#include "Game.h"
// 状態
#define GAME_STATE_NULL 0x00
#define GAME_STATE_PLAY 0x10
// 変数の定義
static char gameState;// 状態
static char gamePause;// 一時停止
// ゲームを初期化する
void GameInitialize(void) {
    // ゲームの初期化
    curTileInd = TILE_USER_INDEX;
    EnemyInitialize(); // 敵の初期化
    ShipInitialize(); // 自機の初期化
    ShotInitialize(); // ショットの初期化
    gamePause = 0;// 一時停止の初期化
    VDP_drawText("         ",11,11);
    VDP_setWindowVPos(FALSE,1);
    VDP_setWindowAddress(VDP_getBGAAddress());
    // 状態の設定
    gameState = GAME_STATE_PLAY;
    appState = APP_STATE_GAME_UPDATE;
    updateSprite = 1;
}
static void GamePlay(void);
// ゲームを更新する
void GameUpdate(void) {
    char a = gameState & 0xf0;// 状態別の処理
    // ESC キーで一時停止
    if (a == GAME_STATE_PLAY && (trigger & BUTTON_START)) {
        updateSprite = 1 - updateSprite;
        gamePause = 1 - gamePause;
    }
    if (gamePause) return;
    random();// 乱数の更新
    GamePlay();
}
// ゲームをプレイする
static void GamePlay(void) {
    if (!(gameState&0xf)) {// 初期化の開始
        gameState++;// 初期化の完了
    }
    ShipUpdate(); // 自機の更新
    ShotUpdate(); // ショットの更新
    EnemyUpdate(); // 敵の更新
    ShipRender(); // 自機の描画
    ShotRender(); // ショットの描画
    EnemyRender(); // 敵の描画
}
