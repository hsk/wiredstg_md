#include "bios.h"
#include "Math.h"
#include "App.h"
#include "Ship.h"
#include "Enemy.h"
#include "Bullet.h"
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
