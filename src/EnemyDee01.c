// EnemyDee01.c : 敵／ディー０１
#include "Game.h"
// 敵を生成する
void EnemyDee01Generate(void) {
    GENERATOR* iy = &enemyGenerator;
    do {
        #if 0
        iy->kind = 0;
        iy->state = 0;
        return;
        #endif

        // 初期化の開始
        if (iy->state == 0) {
            // 地面のチェック
            if (iy->kind==ENEMY_TYPE_DEE01_UPPER){
                if (groundGenerator.upper_state!=GROUND_GENERATOR_STATE_FLAT) break;
                if (groundGenerator.upper_length<8) break;
            } else {
                if (groundGenerator.lower_state!=GROUND_GENERATOR_STATE_FLAT) break;
                if (groundGenerator.lower_length<8) break;
            }
            // 生成数の設定
            u8 a = (random()&3);
            if(a==0)a++;
            iy->length = a;// 生成数の設定
            iy->timer = 2;// タイマの設定
            iy->state++;// 初期化の完了
        }
        if (gameScroll!=0) return;// スクロールの監視
        if (--iy->timer) return;// タイマの更新
        iy->timer = 0x2;
        // 生成の開始
        ENEMY* ix = EnemyGetEmpty();
        if (ix==0) return;
        // 敵の生成
        ix->kind = iy->kind;
        ix->state = 0;
        ix->x = FIX16(0);
        ix->y = (ix->kind == ENEMY_TYPE_DEE01_UPPER) ? FIX16(0x18) : FIX16(0xb0);
        ix->hp = 1;
        if(--iy->length)return;// 生成数の更新
    } while(0);
    iy->kind = 0;
    iy->state = 0;
}
static void up1(ENEMY* ix) {
    u8 a;
    s8 h = (fix16ToInt(ship.x)>>1)-(fix16ToInt(ix->x)>>1);
    if (!(h&0x80)) {
        a = (fix16ToInt(ship.y)>>1)-(fix16ToInt(ix->y)>>1);
        if(!(a&0x80)) {
            a = SystemGetAtan2(h,a);
        } else {
            a = 0;
        }
    } else {
        a = 0x40;
    }

    u8 c = 0;
    if (a >= 0x10) {
        c++;
        if (a >= 0x30) c++;
    }
    ix->animation = c;
}
static void up2(ENEMY* ix) {
    u8 h = (fix16ToInt(ship.x)>>1)-(fix16ToInt(ix->x)>>1);
    u8 a;
    if (!(h&0x80)) {
        a = (fix16ToInt(ship.y)>>1)-(fix16ToInt(ix->y)>>1);
        if(a&0x80) {
            a = SystemGetAtan2(h,a);
        } else {
            a = 0;
        }
    } else {
        a = 0xc0;
    }
    if (a) a--;
    u8 c = 0;
    if (a) {
        if (a < 0xf0) {
            c++;
            if (a <0xd0) c++;
        }
    }
    ix->animation = c + 3;
}

// 敵を更新する
void EnemyDee01Update(ENEMY* ix) {
    // 初期化の開始
    if (ix->state==0) {
        ix->param0 = 0;// パラメータの設定
        ix->shot = (random()&0x3f)+0x40;// ショットの設定
        ix->state++;// 初期化の完了
    }
    // 移動（→）
    if ((gameScroll&1)==0) {
        ix->x+=FIX16(1);
        if (ix->x>=FIX16(320)) {
            ix->kind = 0;// 敵の削除
            return;
        }
    }
    // 向きの取得
    if (ix->kind==ENEMY_TYPE_DEE01_UPPER) {
        ix->animation=0;
        up1(ix);
    } else {
        ix->animation=3;
        up2(ix);
    }
    // ショットの更新
    if (ship.x >= ix->x) {
        if (--ix->shot) return;
        BulletGenerate(ix->x,ix->y);
        ix->shot=(random()&0x3f)+0x30;
    }
}
void EnemyDee01Render(ENEMY* ix) {
    EnemyPutPattern16x16(ix, dee01_sprite);
}
