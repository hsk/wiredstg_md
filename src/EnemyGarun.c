// EnemyGarun.c : 敵／ガルン
#include "Game.h"
// 定数の定義
// カーブ
static u8 const enemyGarunCurve[] = {
    0x04, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02,
    0x02, 0x01, 0xff, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd,
    0xfc, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfe,
    0xfe, 0xff, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03,
    //0x04, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01,
    //0x00, 0x00, 0xff, 0xff, 0xfe, 0xfe, 0xfd, 0xfd,
    //0xfc, 0xfc, 0xfd, 0xfd, 0xfe, 0xfe, 0xff, 0xff,
    //0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03,
};
// アニメーション
static u8 const enemyGarunAnimation[] = {
    0, 1, 2, 1,
    3, 4, 5, 4,
};
// 敵を生成する
void EnemyGarunGenerate(void) {
    GENERATOR* iy = &enemyGenerator;
    #if 0
    iy->kind = 0;
    iy->state = 0;
    return;
    #endif
    // 初期化の開始
    if (iy->state == 0) {
        iy->length = 4;// 生成数の設定
        iy->timer = 1;// タイマの設定
        iy->state++;// 初期化の完了
    }
    if (--iy->timer) return;// タイマの更新
    iy->timer = 0x10;
    ENEMY* ix = EnemyGetEmpty();
    if(ix==0)return;
    ix->kind = iy->kind;// 敵の生成
    ix->state = 0;
    ix->x = (ix->kind==ENEMY_TYPE_GARUN_BACK) ? FIX16(319) : FIX16(0);
    ix->y = FIX16(((random()&3)<<5)+0x30);
    ix->hp = 0x01;
    if(--iy->length)return;// 生成数の更新
    iy->kind = 0;
    iy->state = 0;
}
// 敵を更新する
void EnemyGarunUpdate(ENEMY* ix) {
    // 初期化の開始
    if (ix->state==0){
        ix->param0 = 0;// パラメータの設定
        ix->timer = 0;// タイマの設定
        ix->state++;// 初期化の完了
    }
    for(;;) {
        // 移動（←→）
        if (ix->kind == ENEMY_TYPE_GARUN_FRONT) {
            ix->x+=FIX16(2);
            if (ix->x>FIX16(319)) break;
        } else {
            ix->x-=FIX16(2);
            if (ix->x < FIX16(0)) break;
        }
        // 移動（↑↓）
        ix->param0++;
        ix->y += intToFix16((s8)enemyGarunCurve[ix->param0&0x1f]);
        if (ix->y>=FIX16(0xc0)) break;
        // アニメーションの更新
        ix->timer++;
        ix->animation=enemyGarunAnimation[
            ((ix->kind-ENEMY_TYPE_GARUN_FRONT)<<2)+
            ((ix->timer & 0x0c)>>2)];
        return;
    }
    ix->kind = 0;// 敵の削除
}
void EnemyGarunRender(ENEMY* ix) {
    SystemSetSprite(enemyInd, garun_sprite, ix->animation, ix->x, ix->y);
}
