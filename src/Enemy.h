// Enemy.h : 敵
// 敵
#define ENEMY_N                 0x10
typedef struct ENEMY {
  u8 kind;
  u8 state;
  s16 x;
  s16 y;

  u8 index;
  u8 hp;
  u8 shot;
  u8 shot_param[3];
  u8 animation;
  u8 timer;

  u8 param0;
  u8 param1;
  u8 param2;
  u8 param3;
} ENEMY;
// ジェネレータ
typedef struct GENERATOR {
    u8 phase;
    u8 kind;
    u8 state;
    u8 length;
    u8 timer;
    u8 param0;
    u8 param1;
    u8 param2;
} GENERATOR;
// 段階
#define ENEMY_PHASE_NULL        0x00
#define ENEMY_PHASE_NORMAL      0x01
// 種類
#define ENEMY_TYPE_NULL         0x00
#define ENEMY_TYPE_BOMB         0x01
#define ENEMY_TYPE_FANS_FRONT   0x02
#define ENEMY_TYPE_FANS_BACK    0x03
#define ENEMY_TYPE_RUGAL_FRONT  0x04
#define ENEMY_TYPE_RUGAL_BACK   0x05
#define ENEMY_TYPE_GARUN_FRONT  0x06
#define ENEMY_TYPE_GARUN_BACK   0x07
#define ENEMY_TYPE_DEE01_UPPER  0x08
#define ENEMY_TYPE_DEE01_LOWER  0x09
#define ENEMY_TYPE_DUCKER_UPPER 0x0a
#define ENEMY_TYPE_DUCKER_LOWER 0x0b
#define ENEMY_TYPE_BIGCORE_CORE 0x0c
#define ENEMY_TYPE_BIGCORE_BODY 0x0d
#define ENEMY_TYPE_BEAM         0x0e
// 状態
#define ENEMY_STATE_NULL        0x00
// 外部関数宣言
void EnemyInitialize(void);
void EnemyUpdate(void);
void EnemyRender(void);
ENEMY* EnemyGetEmpty(void);

void EnemyFansGenerate(void);
void EnemyFansUpdate(ENEMY* ix);
void EnemyFansRender(ENEMY* ix);
void EnemyRugalGenerate(void);
void EnemyRugalUpdate(ENEMY* ix);
void EnemyRugalRender(ENEMY* ix);
void EnemyGarunGenerate(void);
void EnemyGarunUpdate(ENEMY* ix);
void EnemyGarunRender(ENEMY* ix);
void EnemyPutPattern16x16(ENEMY* ix, const FSPR* fsprs);
void EnemyBombGenerate();
void EnemyBombUpdate(ENEMY* ix);
void EnemyBombRender(ENEMY* ix);
// 外部変数宣言
extern u16 enemyInd;
extern u16 bigcoreInd;
extern u16 beamInd;
extern ENEMY enemy[ENEMY_N];
extern char eneyN;
extern GENERATOR enemyGenerator;
extern u8 enemyCollision[0x03c0];
