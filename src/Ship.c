// Ship.c : 自機
#include "Game.h"
u16 shipInd;
// 変数の定義
SHIP ship;// 自機
// 自機を初期化する
void ShipInitialize(void) {
    // 自機の初期化
    ship.kind = SHIP_TYPE_VICVIPER;
    ship.state = SHIP_STATE_NULL;
    ship.x = FIX16(0xc0);
    ship.y = FIX16(0x60);
    ship.hp = 0x01;
    ship.speed = FIX16(0x02);
    ship.animation  = 0x00;
    ship.timer = 0x00;
    ship.shot_l = 0x00;
    ship.shot_h = 0x00;
    VDP_loadTileData(ship_tiles, shipInd=curTileInd,sizeof(ship_tiles)/(8*4), CPU);
    curTileInd += sizeof(ship_tiles)/(8*4);
}
// 自機を更新する
void ShipUpdate(void) {
    // ↑↓の移動
    if (input & BUTTON_UP) {
        ship.y -= ship.speed;
        if (ship.y<FIX16(0x10))ship.y=FIX16(0x10);
        ship.animation = 2;
    } else if (input & BUTTON_DOWN) {
        ship.y += ship.speed;
        if (ship.y>FIX16(0xb7))ship.y=FIX16(0xb7);
        ship.animation = 1;
    } else {
        ship.animation = 0;
    }
    // ←→の移動
    if (input & BUTTON_LEFT) {
        ship.x -= ship.speed;
        if (ship.x<FIX16(0xc))ship.x=FIX16(0xc);
    } else if (input & BUTTON_RIGHT) {
        ship.x += ship.speed;
        if (ship.x>FIX16(64+0xf6))ship.x=FIX16(64+0xf6);
    }
    // ショット
    if (ship.shot_l==0 && (input & BUTTON_A)) {
        ShotGenerate();
        ship.shot_l=2;
        ship.shot_h++;
    } else if (input & BUTTON_A) {
        ship.shot_l--;
    } else {
        ship.shot_l=0;
    }
}
// 自機を描画する
void ShipRender(void) {
    // スプライトの描画
    if (ship.kind==0 || ship.animation >= 9)return;
    // 描画の開始
    SystemSetSprite(shipInd, ship_sprite, ship.animation, ship.x, ship.y);
}
