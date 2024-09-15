// EnemyBomb.c : 敵／爆発
#include "Game.h"
// 敵を生成する
void EnemyBombGenerate(void) {
}
// 敵を更新する
void EnemyBombUpdate(ENEMY* ix) {
    // 初期化の開始
    if(ix->state==0){
        ix->state++;// 初期化の完了
        ix->animation = 0;// アニメーションの設定
        return;
    }
    if(ix->state==1) {
        ix->animation = 0;// アニメーションの設定
        ix->timer = 1;// タイマの設定
        ix->state++;
    }
    // アニメーションの更新
    if(--ix->timer) return;
    if (ix->animation < 2) {
        ix->animation++;
        ix->timer = 2;
        return;
    }
    ix->kind = 0;// 敵の削除
}
// 敵を描画する
void EnemyBombRender(ENEMY* ix) {
    SystemSetSprite(enemyInd, boom_sprite, ix->animation, ix->x, ix->y);
}
