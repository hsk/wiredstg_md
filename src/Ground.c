// Ground.c : 地面
#include "Game.h"
// 変数の定義
u8 ground[0x0600];// 地面
u8 ground_flip=0;
GROUND_GENERATOR groundGenerator;// ジェネレータ
static u8 groundGeneratorRow[0x18];
// 地面を初期化する
void GroundInitialize(void) {
    memset(ground,0,0x600);// 地面の初期化
    memset(groundGeneratorRow,0,0x018);// 生成する列の初期化
    // ジェネレータの初期化
    groundGenerator.upper_state = 0;
    groundGenerator.lower_state = 0;
    groundGenerator.upper_height = 0;
    groundGenerator.lower_height = 0;
    groundGenerator.upper_length = 1;
    groundGenerator.lower_length = 1;
    gameScroll = 0;
    gameScroll2 = 0;
    ground_flip = 0;
    VDP_clearTileMapRect(BG_A, 0, 0, 512/8, 28);
}
static void genup(void) {
    // ↑の生成
    u8 a = groundGenerator.upper_state;
    if (a == 0)                                  {// 地面なし／↑
        GROUND_GENERATOR* ix = &groundGenerator;
        if (--ix->upper_length) return;
        groundGeneratorRow[1]=0x4a;
        ix->upper_height++;
        ix->upper_length=(random()&0x1f) + 0x11;
        ix->upper_state = GROUND_GENERATOR_STATE_FLAT;
    } else if (a == GROUND_GENERATOR_STATE_FLAT) {// 平地／↑
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 1;
        for(u8 b= ix->upper_height-1;b;b--) *hl++ = 0x4c;
        *hl = 0x48;
        if(--ix->upper_length) return;
        ix->upper_length = (random()&7)+1;
        ix->upper_state = GROUND_GENERATOR_STATE_UP;
    } else if (a == GROUND_GENERATOR_STATE_UP)   {// 上る／↑
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 1;
        for(u8 b = ix->upper_height;b;b--) *hl++ = 0x4c;
        *hl = 0x4a;
        ix->upper_height++;
        if(--ix->upper_length) return;
        ix->upper_length = ix->upper_height - 1;
        ix->upper_state = GROUND_GENERATOR_STATE_DOWN;
    } else                                       {// 下る／↑
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 1;
        for(u8 b = ix->upper_height-1;b;b--) *hl++ = 0x4c;
        *hl = 0x4b;
        ix->upper_height--;
        if(--ix->upper_length) return;
        ix->upper_length = (random()&0x1f)+0x11;
        ix->upper_state = GROUND_GENERATOR_STATE_FLAT;
    }
}
static void gendown(void) {
    u8 a = groundGenerator.lower_state;
    if (a == 0)                                  { // 地面なし／↓
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 0x17;
        if (--ix->lower_length) return;
        *hl=0x4b;
        ix->lower_height++;
        ix->lower_length=(random()&0x1f) + 0x11;
        ix->lower_state = GROUND_GENERATOR_STATE_FLAT;
    } else if (a == GROUND_GENERATOR_STATE_FLAT) { // 平地／↓
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 0x17;
        for(u8 b= ix->lower_height-1;b;b--) *hl-- = 0x4c;
        *hl = 0x49;
        if(--ix->lower_length) return;
        ix->lower_length = (random()&7)+1;
        ix->lower_state = GROUND_GENERATOR_STATE_UP;
    } else if (a == GROUND_GENERATOR_STATE_UP)   { // 上る／↓
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 0x17;
        for(u8 b = ix->lower_height;b;b--) *hl-- = 0x4c;
        *hl = 0x4b;
        ix->lower_height++;
        if(--ix->lower_length) return;
        ix->lower_length = ix->lower_height - 1;
        ix->lower_state = GROUND_GENERATOR_STATE_DOWN;
    } else                                       { // 下る／↓
        GROUND_GENERATOR* ix = &groundGenerator;
        u8* hl = groundGeneratorRow + 0x17;
        for(u8 b = ix->lower_height-1;b;b--) *hl-- = 0x4c;
        *hl = 0x4a;
        ix->lower_height--;
        if(--ix->lower_length) return;
        ix->lower_length = (random()&0x1f)+0x8;
        ix->lower_state = GROUND_GENERATOR_STATE_FLAT;
    }
}
// 地面を更新する
void GroundUpdate(void) {
    // スクロールの開始
    if (gameScroll) return;
    for(u8* hl=groundGeneratorRow+1,*ix=ground+64+gameScroll3,b=0x17;b;ix+=64,hl++,b--)
        *ix=*hl;
    memset(groundGeneratorRow,0,0x018);// 生成する列の初期化
    genup();
    gendown();
}
// 地面を描画する
void GroundRender(void) {
    if (!gameScroll) {
        u8 *p = groundGeneratorRow+1;
        for(u8 y=1;y<24;y++,p++)
            VDP_setTileMapXY(BG_A, TILE_FONT_INDEX+ *p - 6, gameScroll3, y+ground_flip);
    }
    VDP_setHorizontalScroll(BG_A,gameScroll2>>1);
}
