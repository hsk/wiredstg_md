// Shot.c : ショット
#include "Game.h"
// 変数の定義
SHOT shot[SHOT_N];// ショット
// ショットを初期化する
void ShotInitialize(void) {
    memset(shot,0,sizeof(shot)); // ショットの初期化
}
// ショットを生成する
void ShotGenerate(void) {
    // 空きを探す
    SHOT* ix = shot;
    for(u8 b = SHOT_N;b;b--,ix++) {
        if (ix->state) continue;
        // ショットの生成
        ix->x = ship.x - FIX16(0x0c);
        ix->y = ship.y;
        ix->state++;
        SE0(shotSe);// ＳＥの再生
        break;
    }
}
// ショットを更新する
void ShotUpdate(void) {
    // ショットの走査
    SHOT *ix = shot;
    for(u8 b=SHOT_N;b;ix++,b--) {
        if (ix->state==0) continue;
        // 移動
        ix->x-=FIX16(8); 
        if (ix->x < FIX16(0)) {
            ix->state = 0;
            continue;
        }
    }
}
// ショットを描画する
void ShotRender(void) {
    SHOT* ix = shot;
    for(u8 b=SHOT_N;b;ix++,b--) {
        if (ix->state==0) continue;
        SystemSetSprite(shipInd, ship_sprite, 7, ix->x, ix->y);
    }
}
