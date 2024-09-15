// EnemyRugal.c : 敵／ルグル
// よってくるやつ
#include "Game.h"
// 敵を生成する
void EnemyRugalGenerate(void) {
    GENERATOR* iy = &enemyGenerator;
    #if 0
    iy->kind = 0;
    iy->state = 0;
    return;
    #endif
    // 初期化の開始
    if (iy->state == 0) {
        iy->length = 3;// 生成数の設定
        iy->timer = 1;// タイマの設定
        iy->state++;// 初期化の完了
    }
    if (--iy->timer) return;// タイマの更新
    iy->timer = 0x10;
    ENEMY* ix = EnemyGetEmpty();
    if(ix==0)return;
    ix->kind = iy->kind;// 敵の生成
    ix->state = 0;
    ix->x = (iy->kind == ENEMY_TYPE_RUGAL_BACK) ? FIX16(319) : 0;
    
    char c = ((random()&7)<<3);
    ix->y = intToFix16((c<<1)+c+0x20);
    ix->hp = 0x01;
    // 生成数の更新
    if(--iy->length) return;
    // 生成の完了
    iy->kind = 0;
    iy->state = 0;
}
// 敵を更新する
void EnemyRugalUpdate(ENEMY* ix) {
    // 初期化の開始
    if (ix->state==0){
        ix->param0 = 0;// パラメータの設定
        ix->param1 = (ix->kind - ENEMY_TYPE_RUGAL_FRONT)*3+1;
        ix->shot = (random()&0x3f)+0x40;// ショットの設定
        ix->animation = ix->param1;// アニメーションの設定
        ix->timer = 0x40;// タイマの設定
        ix->state++;// 初期化の完了
    }
    // 移動
    for(;;) {
        if (ix->kind == ENEMY_TYPE_RUGAL_FRONT) {
            ix->x += FIX16(1);
            if(ix->x>=FIX16(320)) break;
        } else {
            ix->x-=FIX16(1);
            if(ix->x<=FIX16(0)) break;
        }
        ix->y+=intToFix16((s8)ix->param0);
        if(ix->y>=FIX16(0xc0)||ix->y<=FIX16(0)) break;
        // 方向転換
        if(--ix->timer == 0) {
            s8 a = fix16ToInt(ship.y - ix->y);
            if (a < 0) {
                if (a < -8) a = -1; else a = 0;
            } else {
                if (a > 8) a = 1; else a = 0;
            }
            ix->param0 = a;
            ix->animation = a + ix->param1;
            ix->timer = (random()&0x3f)+0x10;
        }
        // ショットの更新
        if (--ix->shot == 0) {
            BulletGenerate(ix->x,ix->y);
            ix->shot = (random()&0x3f)+0x40;
        }
        return;
    }
    ix->kind = 0;// 敵の削除
}
void EnemyRugalRender(ENEMY* ix) {
    EnemyPutPattern16x16(ix, rugal_sprite);
}
