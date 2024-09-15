// Star.c : 星
#include "Game.h"
// 星
typedef struct STAR {
  s16 x;
  s16 speed;
} STAR;
#define STAR_N          0x15
// 変数の定義
static STAR star[STAR_N];// 星
// 星を初期化する
void StarInitialize(void) {
    // 星の初期化
    STAR *ix = star;
    for (u8 b=STAR_N;b;b--,ix++) {
        ix->x = FIX16(random() % 320);
        ix->speed = FIX16((random() & 3)+1);
    }
}
// 星を更新する
void StarUpdate(void) {
    STAR *ix = star;
    for(u8 b=STAR_N;b;b--,ix++) {
        ix->x += ix->speed;
        if(ix->x >= FIX16(320)) {
            ix->x-=FIX16(320)-ix->speed;
            ix->speed = FIX16((random()&3)+1);
        }
    }
}    
// 星を描画する
void StarRender(void) {
    // パターンネームの描画
    STAR* ix=star;
    for(u16 y=FIX16(20),b=STAR_N;b;b--,ix++) {
        SystemSetSprite(shipInd, star_sprite, 0, ix->x, y);
        y += FIX16(8); 
    }
}
