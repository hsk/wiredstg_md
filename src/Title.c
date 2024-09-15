// Title.c : タイトル
#include "bios.h"
#include "App.h"
#include "Title.h"
#include "string.h"
#include "gfx.h"
// 変数の定義
static u8 titleState;    // 状態
static u8 titleAnimation;// アニメーション
static u8 titleTimer;    // タイマ
// タイトルを初期化する
void TitleInitialize(void) {
    titleState = 0;// 状態の設定
    appState = APP_STATE_TITLE_UPDATE;
    VDP_setVerticalScroll(BG_B,72);
    VDP_clearTileMapRect(BG_A, 0, 0, 512/8, 28);
    VDP_setVerticalScroll(BG_A,0);
    VDP_setHorizontalScroll(BG_A,0);
    updateSprite = 1;
}
static void updateAnim(void) {
    // アニメーションの更新
    if (titleAnimation<0x60) {
        titleAnimation++;
        u8 x = titleAnimation&31;
        u8 y = titleAnimation/32;
        if (4 <= x && x < 4+24 && y < 4) {
            VDP_setTileMapXY(BG_A,TILE_USER_INDEX+title_map[x-4+y*24],x+4,y+7);
        }
    }
}
static void drawSpace(void) {
    // ハイスコアの描画
    if (titleAnimation<0x60)return;

    if (titleTimer&0x10) VDP_drawText("PRESS START BUTTON",7+4,18);
    else                 VDP_drawText("                  ",7+4,18);
}
// タイトルを更新する
void TitleUpdate(void) {
    if (titleState==0) {// 初期化の開始
        titleAnimation = 0; // アニメーションの初期化
        titleTimer = 0;     // タイマの初期化
        titleState++;       // 初期化の完了
    }
    random();// 乱数の更新
    if (titleState==1) {// START キーの監視
        titleTimer++;// タイマの更新
        if ((input & BUTTON_START) || (input & BUTTON_A)) {// START キーの押下
            titleState++;
            while(titleAnimation<0x60)updateAnim();
        }
    } else {
        titleTimer += 8;// タイマの更新
        if (titleTimer >= 0xf8) {// ジングルの監視
            appState = APP_STATE_GAME_INITIALIZE;// ゲームの開始
        }
    }
    updateAnim();
    drawSpace();
}
