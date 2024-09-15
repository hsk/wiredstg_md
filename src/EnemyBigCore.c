// EnemyBigCore.c : 敵／ビッグコア
#include "Game.h"
u16 bigcoreInd;
// 定数の定義
// パターンネーム
static u8 const enemyBigCoreCorePatternName[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0
    0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 1
    0x24, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 2
    0x24, 0x25, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, // 3
    0x24, 0x25, 0x26, 0x27, 0x00, 0x00, 0x00, 0x00, // 4
    0x24, 0x25, 0x26, 0x26, 0x00, 0x00, 0x00, 0x00, // 5
    0x24, 0x25, 0x26, 0x26, 0x27, 0x00, 0x00, 0x00, // 6
    0x24, 0x25, 0x26, 0x26, 0x26, 0x00, 0x00, 0x00, // 7
    0x24, 0x25, 0x26, 0x26, 0x26, 0x27, 0x00, 0x00, // 8
    0x24, 0x25, 0x26, 0x26, 0x26, 0x26, 0x00, 0x00, // 9 
    0x24, 0x25, 0x26, 0x26, 0x26, 0x26, 0x27, 0x00, // 10
    0x24, 0x25, 0x26, 0x26, 0x26, 0x26, 0x26, 0x00, // 11
};
static u8 const enemyBigCoreBodyPatternName[] = {
    0x00, 0x00, 0x60, 0x00, 0x00, 0x61, 0x62, 0x63, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x65, 0x66, 0x67, 0x68, 0xa0, 0x69, 0x6a, 0x6b, 0x00, 0x00, 0x00,
    0x6c, 0x6d, 0x6e, 0x6f, 0xa0, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x00,
    0x76, 0x77, 0x78, 0xa0, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7e, 0x7f,
    0xa1, 0xa2, 0xa3, 0xa0, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x86, 0x87, 0x88, 0xa0, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8e, 0x8f,
    0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x00,
    0x94, 0x95, 0x96, 0x97, 0x98, 0xa0, 0x99, 0x9a, 0x9b, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x90, 0x00, 0x00, 0x91, 0x92, 0x93, 0x00, 0x00, 0x00, 0x00,
};
// 敵を生成する
void EnemyBigCoreGenerate(void) {
    // 敵の生成／コア
    ENEMY* ix = EnemyGetEmpty();
    ix->kind = ENEMY_TYPE_BIGCORE_CORE;
    ix->state = 0;
    ix->x = FIX16(0xa0);
    ix->y = FIX16(0xe0);
    ix->hp = 16;
    ix->param0 = 16;
    // 敵の生成／ボディ
    ENEMY* body = EnemyGetEmpty();
    *(ENEMY**)&body->param0=ix;
    body->kind = ENEMY_TYPE_BIGCORE_BODY;
    body->state = 0;
    body->hp = 0xff;
    enemyGenerator.kind = 0;// 生成の完了
    enemyGenerator.state = 0;
}

static void up1(ENEMY* ix) {
    if (ix->y>=FIX16(0x61)) ix->y -= FIX16(4);
    else if (ix->x>=FIX16(0x39)) ix->x -= FIX16(4);
    else ix->state = 2;
}
static void up2(ENEMY* ix) {
    EnemyBeamGenerate((fix16ToInt(ix->x)<<8)|fix16ToInt(ix->y));
    ix->timer = 0x20;
    ix->state = 3;
}
static void up3(ENEMY* ix) {
    if (--ix->timer == 0) {
        u8 c = (random()&0x18) + 0x18;
        u8 sy = fix16ToInt(ship.y);
        u8 ey = fix16ToInt(ix->y);
        if ((ey > 0x88 || ey >= 0x38) && (ey >= sy && (ey==sy || ey >= 0x64))) {
            ey -= c;
            if (ey < 0x28) ey = 0x28;
            ix->timer = ey;
            ix->state = 4;
        } else {
            ey += c;
            if (ey > 0x98) ey = 0x98;
            ix->timer = ey;
            ix->state = 5;
        }
    }
}
static void up4(ENEMY* ix) {
    ix->y -= FIX16(4);
    if (ix->y < intToFix16(ix->timer)){
        ix->y = intToFix16(ix->timer);
        ix->state = 2;
    }
}
static void up5(ENEMY* ix) {
    ix->y += FIX16(4);
    if (ix->y>=intToFix16(ix->timer)){
        ix->y = intToFix16(ix->timer);
        ix->state = 2;
    }
}
// 敵を更新する／コア
void EnemyBigCoreUpdateCore(ENEMY* ix) {
    if (ix->state==0){
        ix->state++;// 初期化の完了
    }
    u8 a = ix->state;
    if     (--a == 0) up1(ix);// 登場
    else if(--a == 0) up2(ix);// ビームを撃つ
    else if(--a == 0) up3(ix);// 待機
    else if(--a == 0) up4(ix);// 移動↑
    else              up5(ix);// 移動↓

    if (ix->hp != ix->param0) {// ＨＰの監視
        ix->param0 = ix->hp;
    }
}
// 敵を更新する／ボディ
void EnemyBigCoreUpdateBody(ENEMY* ix) {
    if (ix->state==0) {// 初期化の開始
        ix->animation = 45;// アニメーションの設定
        ix->state++;// 初期化の完了
    }
    ix->hp = 0xff; // ＨＰの再設定
    //ENEMY* iy = (ENEMY*)((ix[ENEMY_PARAM_1]<<8)|ix->param0);// コアの取得
    ENEMY* iy = *(ENEMY**)&ix->param0;// コアの取得
    if (iy->kind==ENEMY_TYPE_BIGCORE_CORE) {// コアの監視
        ix->x = iy->x;
        ix->y = iy->y;
    } else {
        if (--ix->animation == 0)// アニメーションの更新
            ix->kind = 0;// 敵の削除

        ENEMY *iy = EnemyGetEmpty();
        if (iy==0) return;
        iy->kind = ENEMY_TYPE_BOMB;
        iy->state = 1;
        iy->x = intToFix16((random() & 63)+(random() & 32));
        iy->y = ix->y + intToFix16((random() & 63) -32);

    }
}
u16 get_core_pos(ENEMY* ix) {
    u16 y = fix16ToInt(ix->y)&0xf8;
    return (y<<2) + y + (fix16ToInt(ix->x)>>3) - 6;
}
// 敵を描画する／コア
void EnemyBigCoreRender(ENEMY* ix) {
    // 位置の取得
    u16 bc = get_core_pos(ix);
    // パターンを置く
    if(ix->y>=FIX16(0xc0)) return;
    u8* iy = enemyCollision + bc + 5;
    s8 hp = ix->hp-5; if (hp < 1) hp = 1;
    u8* hl = (void*)(enemyBigCoreCorePatternName+((hp<<3)&0x78));
    u16 x = 5;
    for (u8 b=0x07;b;iy++,--b){
        u8 a = *hl++;
        VDP_setTileMapXY(BG_B, bigcoreInd+a, x, 4);
        if (a) *iy = ix->index;
        x++;
    }
}

// 敵を描画する／ボディ
s16 get_body_pos(ENEMY*ix){
    u16 y = (fix16ToInt(ix->y)-0x20)&0xf8;
    return (y<<2) +y+ (fix16ToInt(ix->x)>>3) - 6;
}
void EnemyBigCoreBodyRender(ENEMY* ix) {
    // パターンを置く
    s8 b1 = 0x1b - (fix16ToInt(ix->y)>>3);
    if (b1<0) return;
    if (++b1 > 9) b1 = 9;
    // 位置の取得
    s16 bc = get_body_pos(ix);
    {
        u8* iy = enemyCollision + bc;
        u8* hl = (void*)enemyBigCoreBodyPatternName;
        for(;b1;b1--){
            for(u8 b = 12;b;iy++,--b) {
                if (*hl++) *iy = ix->index;
            }
            iy += 40-12;
        }
    }
    VDP_setHorizontalScroll(BG_B,fix16ToInt(ix->x)-48);
    VDP_setVerticalScroll(BG_B,32-fix16ToInt(ix->y));
}
