// EnemyBeam.c : 敵／ビーム
#include "Game.h"
u16 beamInd;
// 敵を生成する
void EnemyBeamGenerate(s16 hl) {
    // 敵の生成
    ENEMY *ix = EnemyGetEmpty();
    if (ix==0) return;
    ix->kind = ENEMY_TYPE_BEAM;
    ix->state = 0;
    ix->x = intToFix16((((u16)hl)>>8)+0x18);
    ix->y = intToFix16((((u16)hl)&255)-24);
    ix->hp = 0xff;
}
// 敵を更新する
void EnemyBeamUpdate(ENEMY* ix) {
    // 初期化の開始
    if (ix->state==0) {
        ix->shot = 0;// ショットの設定
        ix->state++;// 初期化の完了
    }
    ix->hp = 0xff;// ＨＰの再設定
    if (ix->shot<4) {// ショットの更新
        ix->shot++;
        return;
    }
    // 移動
    ix->x += FIX16(8);
    if (ix->x >= FIX16(320)) ix->kind = 0;// 敵の削除
}
// 敵を描画する
void EnemyBeamRender(ENEMY* ix) {
    // 位置の取得
    u16 y= fix16ToInt(ix->y)&0xf8;
    *(u16*)&ix->param0 = (y<<2)+y+(fix16ToInt(ix->x)>>3);
    // 長さの取得
    {
        u8 e = (fix16ToInt(ix->x)>>3);
        ix->param2 = 40-e;
        if (4<=ix->param2) ix->param2 = ix->shot;
        s16 a = 40-3-e;
        if (a < 0) {
            a = 0;
        } else if (a >= 4) {
            a = ix->shot;
        }
        ix->param3 = a;
    }
    // パターンを置く／外側
    {
        u16 bc = *(u16*)&ix->param0;
        u8* hl = enemyCollision + bc;
        for(u8 b = ix->param2;b;b--) {
            *hl++=ix->index;
        }
        hl += 0xc0-ix->param2;
        for(u8 b = ix->param2;b;b--) {
            *hl++ = ix->index;
        }
    }
    // パターンを置く／内側
    if (ix->param3) {
        u16 bc = *(u16*)&ix->param0;
        u8 *hl = enemyCollision + 40*2+3 + bc;
        u8 a = ix->index;
        for(u8 b = ix->param3;b;b--){
            *hl++ = a;
        }
        u8 c = 0x40 - ix->param3;
        hl += c;
        for(u8 b = ix->param3;b;b--){
            *hl++ = a;
        }
    }
    SystemSetSprite(beamInd,beam_sprite,0,ix->x, ix->y+FIX16(4));
    if (ix->x<FIX16(320-24)) {
        SystemSetSprite(beamInd,beam_sprite,0,ix->x+FIX16(24),ix->y+FIX16(4+16));
        SystemSetSprite(beamInd,beam_sprite,0,ix->x+FIX16(24),ix->y+FIX16(4+32));
    }
    SystemSetSprite(beamInd,beam_sprite,0,ix->x,ix->y+FIX16(4+48));
}
