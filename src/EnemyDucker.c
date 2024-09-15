// EnemyDucker.c : 敵／ダッカー
#include "Game.h"
// 敵を生成する
void EnemyDuckerGenerate(void) {
    GENERATOR* iy = &enemyGenerator;// ジェネレータの取得
    #if 0
    iy->kind = 0;
    iy->state = 0;
    return;
    #endif
    do {
        // 初期化の開始
        if (iy->state==0) {
            if (ground[64+64-41]==0) break;// 地面のチェック
            iy->length = 0x01;// 生成数の設定
            iy->timer = 0x01;// タイマの設定
            iy->state++;// 初期化の完了
        }
        if (gameScroll!=0) return;// スクロールの監視
        if (--iy->timer) return;// タイマの更新
        iy->timer = 0x20;
        // 生成の開始
        ENEMY* ix = EnemyGetEmpty();
        if (ix==0) return;
        // 敵の生成
        ix->kind = iy->kind;
        ix->state = 0;
        if (random() < 0x40) ix->x = FIX16(0); else ix->x = FIX16(319);
        ix->hp = 1;
        if(--iy->length)return;// 生成数の更新
        // 生成の完了
    } while(0);
    iy->kind = 0;
    iy->state = 0;
}
static void up1(ENEMY* ix) {
    ix->animation = ((ix->timer & 0x04)>>2) + ix->param2;
    if (--ix->timer == 0) {
        ix->timer = (random() & 0x1f) + 0x10;
        ix->state = 3;
    }
}

static void up2(ENEMY* ix) {
    ix->animation = ((ix->timer & 0x04)>>2) + 3 + ix->param2;
    if (--ix->timer == 0) {
        ix->timer = (random() & 0x1f) + 0x10;
        ix->state = 3;
    }
}

static void up3(ENEMY* ix) {
    if (--ix->shot == 0) {
        BulletGenerate(ix->x,ix->y);
        ix->shot = (random()&0x0f)+0x10;
    }
    u8 a = (ship.x > ix->x) ? 2 : 5;
    ix->animation = a + ix->param2;
    if (--ix->timer == 0) {
        ix->timer = (--ix->param1) ? (random()&0x3f)+0x10 : 0;
        ix->state = (ship.x < ix->x) ? 2 : 1;
    }
}

static void up4(ENEMY* ix) {
    u8 c;
    u16 x = (fix16ToInt(ix->x)-4+(511-(gameScroll2>>1)))&511;
    if (ix->kind == ENEMY_TYPE_DUCKER_UPPER) {
        u8* hl = ground + 64 + (x >> 3);
        for(c = 0x10;*hl;hl += 64) c += 8;
        // 微調整
        if (x>=8) {
            hl-=1;
            if (*hl) {// 一つ手前に物がある 
                c += 8-(x&7);
            } else {
                hl -= 0x40;
                if (! *hl) {
                    c -= 8;
                    c += (x&7);
                }
            }
        }
    } else {
        u8 *hl = ground + 64*23 + (x >> 3);
        for(c = 0xb8;*hl; hl-=64) c -= 8;
        // 微調整
        if (x>=8) {
            hl-=1;
            if (*hl) {// 一つ手前に物がある 
                c -= 8-(x&7);
            } else {
                hl += 0x40;
                if (! *hl) {
                    c += 8;
                    c -= (x&7);
                }
            }
        }
    }
    ix->y = intToFix16(c);
}

// 敵を更新する
void EnemyDuckerUpdate(ENEMY* ix) {
    // 初期化の開始
    if (ix->state==0){
        ix->param0 = 0;// パラメータの設定
        ix->param1 = 4;
        ix->param2 = (ix->kind-ENEMY_TYPE_DUCKER_UPPER)*6;
        ix->shot = 0x10;// ショットの設定
        ix->timer = (random()&0x3f)+0x20;// タイマの設定
        ix->state = (ix->x>FIX16(0x80)) ? 2 : 3;// 初期化の完了
    }
    do {
        u8 a = ix->state;
        if (a==1) {// 移動（→）
            ix->x+=FIX16(2);
            if (ix->x>=FIX16(320)) break;
            up1(ix);
        } else if (a==2) { // 移動（←）
            ix->x-=FIX16(2);
            if (ix->x < FIX16(0)) break;
            up2(ix);
        } else { // 待機
            if ((gameScroll&1)==0) {
                ix->x+=FIX16(1);
                if (ix->x>=FIX16(320)) break;
            }
            up3(ix);
        }
        up4(ix);// 高さの取得
        return;
    } while(0);
    ix->kind = 0;
}
void EnemyDuckerRender(ENEMY* ix) {
    EnemyPutPattern16x16(ix, ducker_sprite);
}
