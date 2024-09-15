// Shot.h : ショット
// ショット
typedef struct SHOT {
  u8 kind;
  u8 state;
  s16 x;
  s16 y;
} SHOT;
#define SHOT_N            0x04
// 種類
#define SHOT_TYPE_NULL    0x00
// 状態
#define SHOT_STATE_NULL   0x00
// 外部関数宣言
void ShotInitialize(void);
void ShotGenerate(void);
void ShotUpdate(void);
void ShotRender(void);
// 外部変数宣言
extern SHOT shot[SHOT_N];
