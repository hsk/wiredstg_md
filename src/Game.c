// Game.c : ゲーム
#include "Game.h"
// 状態
#define GAME_STATE_NULL 0x00
#define GAME_STATE_PLAY 0x10
#define GAME_STATE_OVER 0x20
// 変数の定義
static char gameState;// 状態
static char gamePause;// 一時停止
u8 gameScroll;// スクロール
u16 gameScroll2;// スクロール
u8 gameScroll3;
u8 gameScroll4;
// ゲームを初期化する
void GameInitialize(void) {
    // ゲームの初期化
    curTileInd = TILE_USER_INDEX;
    GroundInitialize(); // 地面の初期化
    EnemyInitialize(); // 敵の初期化
    ShipInitialize(); // 自機の初期化
    ShotInitialize(); // ショットの初期化
    BulletInitialize(); // 弾の初期化
    gamePause = 0;// 一時停止の初期化
    gameScroll = 0;// スクロールの初期化
    gameScroll2 = 0;
    VDP_drawText("         ",11,11);
    VDP_setWindowVPos(FALSE,1);
    VDP_setWindowAddress(VDP_getBGAAddress());
    // 状態の設定
    gameState = GAME_STATE_PLAY;
    appState = APP_STATE_GAME_UPDATE;
    updateSprite = 1;
}
static void GamePlay(void);
static void GameOver(void);
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
    if (a == GAME_STATE_PLAY) GamePlay();
    else GameOver();
}
static void GameHitCheck(void);
// ゲームをプレイする
static void GamePlay(void) {
    if (!(gameState&0xf)) {// 初期化の開始
        gameState++;// 初期化の完了
    }
    gameScroll2 = (gameScroll2+1)& 1023;
    gameScroll = gameScroll2&0x0f;// スクロールの更新
    gameScroll3 = 63-(gameScroll2>>4);
    GameHitCheck();
    GroundUpdate(); // 地面の更新
    ShipUpdate(); // 自機の更新
    ShotUpdate(); // ショットの更新
    EnemyUpdate(); // 敵の更新
    BulletUpdate(); // 弾の更新
    GroundRender(); // 地面の描画
    ShipRender(); // 自機の描画
    ShotRender(); // ショットの描画
    EnemyRender(); // 敵の描画
    BulletRender(); // 弾の描画
    if (ship.kind)return;// ゲームオーバーの条件
    gameState = GAME_STATE_OVER; // ゲームオーバー
}
static u8 timer = 0;
// ゲームオーバーになる
static void GameOver(void) {
    u8 a = gameState&0xf;
    if (a == 0) {// 初期化の開始
        ShotRender(); // ショットの描画
        EnemyRender(); // 敵の描画
        BulletRender(); // 弾の描画
        // ゲームオーバーの表示
        SystemSetSprite(shipInd, gameover_sprite, 0, FIX16(32+11*8)    , FIX16(11*8));
        SystemSetSprite(shipInd, gameover_sprite, 1, FIX16(32+11*8+5*8), FIX16(11*8));
        gameState++;// 初期化の完了
        timer = 0;
    } else if (a == 1) {
        updateSprite = 0;
        timer++;
    }
    if(timer < 60*3) return;
    appState = APP_STATE_GAME_INITIALIZE; // ゲーム初期化
}
// ヒットチェックを行う
static void GameHitCheck(void) {
    { // ショットのチェック
        SHOT *ix = shot;
        for(u8 b = SHOT_N;b;ix++,--b) {
            if(ix->state==0)continue;
            u16 bc = fix16ToInt(ix->y)&0xf8;
            u8 a = enemyCollision[(bc<<2)+bc+(fix16ToInt(ix->x)>>3)]; 
            if (a) {
                ENEMY* iy = enemy -1 + a;
                if (--iy->hp==0) {
                    iy->kind = ENEMY_TYPE_BOMB;
                    iy->state = 0;
                }
            } else continue;
            ix->state = 0;
        }
    }
    #if 1
    #if 1
    {// 弾のチェック
        BULLET* ix = bullet;
        for(u8 b=bulletN;b;b--,ix++) {
            if (ix->state==0)continue;
            if (ship.kind!=SHIP_TYPE_VICVIPER) continue;
            s16 a = ship.x-ix->x;
            if (a < FIX16(-4) || FIX16(4) < a) continue;
            a = ship.y-ix->y;
            if (a < FIX16(-4) || FIX16(4) < a) continue;
            ship.kind = SHIP_TYPE_BOMB;
            ship.state = 0;
            ix->state = 0;
        }
    }
    #endif
    // 自機のチェック
    if (ship.kind != SHIP_TYPE_VICVIPER) return;
    u16 tmp1 = fix16ToInt(ship.y)&0xf8;
    if (enemyCollision[(tmp1<<2)+tmp1+(fix16ToInt(ship.x)>>3)]==0) return;
    ship.kind = SHIP_TYPE_BOMB;
    ship.state = 0;
    #endif
}
