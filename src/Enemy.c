// Enemy.c : 敵
#include "Game.h"
// 定数の定義
// ジェネレータ
static void EnemyNullGenerate(void);
typedef void(*FP)(void);
typedef void(*FP1)(ENEMY*);

static FP const enemyGenerateProc[] = {
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyGarunGenerate,
    EnemyGarunGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    EnemyNullGenerate,
    0x0000,
    0x0000,
};
static char const enemyGenerateType[] = {
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_FRONT,
    ENEMY_TYPE_FANS_BACK,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_FRONT,
    ENEMY_TYPE_RUGAL_BACK,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_FRONT,
    ENEMY_TYPE_GARUN_BACK,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_UPPER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DEE01_LOWER,
    ENEMY_TYPE_DUCKER_UPPER,
    ENEMY_TYPE_DUCKER_UPPER,
    ENEMY_TYPE_DUCKER_LOWER,
    ENEMY_TYPE_DUCKER_LOWER,
};
// 更新
static void EnemyNullUpdate(ENEMY* ix);
static FP1 const enemyUpdateProc[] = {
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyGarunUpdate,
    EnemyGarunUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
    EnemyNullUpdate,
};
// 描画
static void EnemyNullRender(ENEMY*);

u16 enemyInd;

static FP1 const enemyRenderProc[] = {
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyGarunRender,
    EnemyGarunRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
    EnemyNullRender,
};
// 変数の定義
ENEMY enemy[ENEMY_N];// 敵
char enemyN;
GENERATOR enemyGenerator;// ジェネレータ
// 敵を初期化する
void EnemyInitialize(void) {

    memset(enemy,0,sizeof(ENEMY) * ENEMY_N);// 敵の初期化
    ENEMY *hl = enemy;
    for(char b=ENEMY_N,a=1;b;b--,hl++,a++) hl->index=a;
    enemyN = 7;
    // ジェネレータの初期化
    memset(&enemyGenerator,0,sizeof(GENERATOR));
    enemyGenerator.phase=ENEMY_PHASE_NORMAL;
    VDP_loadTileData(enemy_tiles, enemyInd,sizeof(enemy_tiles)/(8*4), CPU);
    curTileInd = enemyInd+sizeof(enemy_tiles)/(8*4);
}
// 敵を更新する
void EnemyUpdate(void) {
    enemyGenerateProc[enemyGenerator.kind]();
    ENEMY *ix=enemy;
    for(char b=enemyN;b;b--,ix++) {// 敵の更新
        enemyUpdateProc[ix->kind](ix);
    }
}
// 敵を描画する
void EnemyRender(void) {
    // 種類別の描画
    ENEMY *ix=enemy;
    for(char b=enemyN;b;b--,ix++) {
        enemyRenderProc[ix->kind](ix);
    }
}
// ENEMY_TYPE_NULL を生成する
static void EnemyNullGenerate(void) {
    u8 a = enemyGenerator.phase; 
    // 通常戦の開始
    if (a==ENEMY_PHASE_NORMAL) {
        // 通常戦の初期化
        if (enemyGenerator.state==0) {
            enemyGenerator.timer=8;
            enemyGenerator.state++;
        }
        // タイマの更新
        if(--enemyGenerator.timer) return;
        // 敵の生成
        enemyGenerator.state = 0;
        enemyGenerator.kind = enemyGenerateType[random()&0x1f];
        return;
    }
}
// ENEMY_TYPE_NULL を更新する
static void EnemyNullUpdate(ENEMY* ix) {
}
// ENEMY_TYPE_NULL を描画する
static void EnemyNullRender(ENEMY* ix) {
}
// 空の敵を取得する
ENEMY* EnemyGetEmpty(void) {
    ENEMY *ix=enemy;
    for(char b=enemyN;b;b--,ix++) if (!ix->kind) return ix;
    return NULL;
}
