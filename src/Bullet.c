// Bullet.c : 弾
#include "Game.h"
// 変数の定義
BULLET bullet[BULLET_N];// 弾
u8 bulletN;
static u8 bulletTimer;// タイマ
// 弾を初期化する
void BulletInitialize(void) {
    // 弾の初期化
    memset(bullet,0,sizeof(bullet));
    bulletN = 7;
    bulletTimer = 0;// タイマの初期化
}
static BULLET* get_bullet(void) {
    // 空の取得
    BULLET*ix = bullet;
    for(u8 b = bulletN;b;b--,ix++)
        if (ix->state==0) return ix;
    return 0;
}
u16 gen1(s16 x,s16 y,BULLET* ix) {
    // 生成の開始
    ix->x = x;
    ix->y = y;
    s8 h1 = fix16ToInt(ship.x-x)>>1;
    s8 l1 = fix16ToInt(ship.y-y)>>1;
    return SystemGetAtan2(h1,l1)<<2;
}
void gen2(u16 a,BULLET* ix) {
    ix->speed_y = sinFix16(a);
    ix->speed_x = cosFix16(a);
    if (a < 0x40*4) {
        ix->state = 1;
    } else if (a < 0x80*4) {
        ix->state = 2;
    } else if (a < 0xc0*4) {
        ix->state = 3;
    } else {
        ix->state = 4;
    }
}
void gen9(BULLET* ix) {
    ix->speed_x = ix->speed_x*2;
    ix->speed_y = ix->speed_y*2;
}
// 弾を生成する
void BulletGenerate(s16 x,s16 y) {
    if (ship.kind != SHIP_TYPE_VICVIPER) return;// 自機の存在
    // 距離の取得
    s16 a = ship.x-x;
    if (FIX16(-16) <= a && a < FIX16(0x10)) return;
    a = ship.y - y;
    if (FIX16(-16) <= a && a < FIX16(0x10)) return;
    BULLET* ix = get_bullet();// 空の取得
    if (ix==NULL) return;
    gen2(gen1(x,y,ix),ix);
    gen9(ix);
}
// 弾を更新する
void BulletUpdate(void) {
    BULLET *ix = bullet;
    for(u8 b = bulletN;b;ix++,b--) {// 弾の走査
        u8 a = ix->state;
        if (a == 0) continue;
        // 弾の移動
        if (--a == 0) {
            ix->x += ix->speed_x;
            if (ix->x >= FIX16(320)) {
                ix->state = 0;// 弾の削除
                continue;
            }
            ix->y += ix->speed_y;
            if (ix->y >= FIX16(320)) {
                ix->state = 0;// 弾の削除
                continue;
            }
            continue;
        }
        if (--a == 0) {
            ix->x += ix->speed_x;
            if (ix->x < 0) {
                ix->state = 0;// 弾の削除
                continue;
            }
            ix->y += ix->speed_y;
            if (ix->y >= FIX16(320)) {
                ix->state = 0;// 弾の削除
                continue;
            }
            continue;
        }
        if (--a == 0) {
            ix->x += ix->speed_x;
            if (ix->x < 0) {
                ix->state = 0;// 弾の削除
                continue;
            }
            ix->y += ix->speed_y;
            if (ix->y < 0) {
                ix->state = 0;// 弾の削除
                continue;
            }
            continue;
        }
        {
            ix->x += ix->speed_x;
            if (ix->x >= FIX16(320)) {
                ix->state = 0;// 弾の削除
                continue;
            }
            ix->y += ix->speed_y;
            if (ix->y < 0) {
                ix->state = 0;// 弾の削除
                continue;
            }
            continue;
        }
    }
    bulletTimer++;// タイマの更新
}
// 弾を描画する
void BulletRender(void) {
    // スプライトの描画
    BULLET *ix = bullet;
    for (u8 b = bulletN;b;ix++,b--) {
        if (ix->state==0) continue;
        SystemSetSprite(shipInd, ship_sprite, 8, ix->x, ix->y);
    }
}
