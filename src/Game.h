#include "bios.h"
#include "Math.h"
#include "App.h"
#include "Ground.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Star.h"
#include "Shot.h"
#include "gfx.h"
#include "string.h"
// Game.h : ゲーム
// スプライト
#define GAME_SPRITE_SHIP_0      0x00
#define GAME_SPRITE_SHIP_1      0x60
// 外部関数宣言
void GameInitialize(void);
void GameUpdate(void);
// 外部変数宣言
extern u8 gameScroll;
extern u16 gameScroll2;
extern u8 gameScroll3;

