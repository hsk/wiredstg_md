// EnemyFans.c : 敵／ファン
#include "Game.h"
// 敵を生成する
void EnemyFansGenerate(void) {
    GENERATOR *iy = &enemyGenerator;
    #if 0
    iy->kind = 0;
    iy->state = 0;
    return;
    #endif

    // 初期化の開始
    if (iy->state == 0) {
        // 生成数の設定
        iy->length = (random()&1)*2+4;
        // タイマの設定
        iy->timer = 1;
        // パラメータの設定
        iy->param1 = (random()&0x80)+0x20;
        iy->state++;// 初期化の完了
    }
    // タイマの更新
    if(--iy->timer)return;
    iy->timer = 0x04;
    // 生成の開始
    ENEMY* ix = EnemyGetEmpty();
    if(ix==0)return;
    ix->kind = iy->kind;// 敵の生成
    ix->state = 0;
    ix->y = intToFix16(iy->param1);
    ix->hp = 0x01;
    
    if(--iy->length)return;// 生成数の更新
    iy->kind = 0;
    iy->state = 0;
}
static void EnemyFansFrontMove(ENEMY *);
static void EnemyFansBackMove(ENEMY *);
// 敵を更新する
void EnemyFansUpdate(ENEMY* ix) {
    // 初期化の開始
    if (ix->state==0){
        // ショットの設定
        ix->animation = 0;// アニメーションの設定
        ix->timer = 4;// タイマの設定
        ix->state++;// 初期化の完了
        ix->x = (ix->kind == ENEMY_TYPE_FANS_FRONT) ? 0 : FIX16(319);
    }
    // 移動
    if (ix->kind == ENEMY_TYPE_FANS_FRONT) EnemyFansFrontMove(ix);
    else                                   EnemyFansBackMove(ix);
    ix->x-=FIX16(1);
    if (fix16ToInt(ix->x) == 0) {
        BulletGenerate(ix->x,ix->y);
        ix->x = intToFix16((random()&0x1f)+0x30);
    }
    if (--ix->timer == 0) {
        u8 a = ix->animation + 1;
        if (a >= 3) a = 0;
        ix->animation = a;
        ix->timer = 4;
    }
}
static void f1(ENEMY* ix) {
    ix->x += FIX16(4);
    if (ix->x>=FIX16(320)) {
        ix->kind=0;
    } else {
        if (ix->x<FIX16(0xd0)) return;
        ix->state = (ship.y<=ix->y) ? 3 : 2;
    }
}
static void f2(ENEMY* ix) {
    ix->x -= FIX16(4);
    if (ix->x<FIX16(0)) ix->kind=0;
    else {
        ix->y += FIX16(4);
        if (ix->y>=FIX16(0xc0)) ix->kind=0;
        else if (ship.y < ix->y) ix->state = 4;
    }
}
static void f3(ENEMY* ix) {
    ix->x -= FIX16(4);
    if (ix->x < FIX16(0)) ix->kind=0;
    else {
        ix->y -= FIX16(4);
        if (ix->y < FIX16(0)) ix->kind=0;
        else if (ship.y > ix->y) ix->state = 4;
    }
}
static void f4(ENEMY* ix) {
    ix->x -= FIX16(4);
    if (ix->x < FIX16(0)) ix->kind=0;
}
// 敵が移動する／→
static void EnemyFansFrontMove(ENEMY* ix) {
    u8 a = ix->state;
    if (a == 1)      f1(ix); // 移動（←）
    else if (a == 2) f2(ix); // 移動（↓）
    else if (a == 3) f3(ix); // 移動（↑）
    else             f4(ix); // 移動（→）
}
static void b1(ENEMY* ix) {
    ix->x-=FIX16(4);
    if (ix->x >= FIX16(0x30)) return;
    ix->state = ship.y>ix->y?2:3;
}
static void b2(ENEMY* ix) {
    ix->x += FIX16(4);
    if (ix->x>=FIX16(320)) ix->kind=0;
    else {
        ix->y += FIX16(4);
        if (ix->y>=FIX16(0xc0)) ix->kind=0;
        else if (ship.y < ix->y) ix->state = 4;
    }
}
static void b3(ENEMY* ix) {
    ix->x += FIX16(4);
    if (ix->x >= FIX16(320)) ix->kind=0;
    else {
        ix->y -= FIX16(4);
        if (ix->y < FIX16(0)) ix->kind=0;
        else if (ship.y > ix->y) ix->state = 4;
    }
}
static void b4(ENEMY* ix) {
    ix->x += FIX16(4);
    if (ix->x >= FIX16(320)) ix->kind=0;
}
// 敵が移動する／←
static void EnemyFansBackMove(ENEMY* ix) {
    u8 a = ix->state;
    if (a == 1)      b1(ix); // 移動（←）
    else if (a == 2) b2(ix); // 移動（↓）
    else if (a == 3) b3(ix); // 移動（↑）
    else             b4(ix); // 移動（→）
}
void EnemyFansRender(ENEMY* ix) {
    EnemyPutPattern16x16(ix, fan_sprite);
}
