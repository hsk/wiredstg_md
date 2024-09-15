// Bullet.h : 弾
// 弾
typedef struct BULLET {
    u8 kind;
    u8 state;
    s16 x;
    s16 y;
    s16 speed_x;
    s16 speed_y;
} BULLET;
#define BULLET_N           0x10
// 種類
#define BULLET_TYPE_NULL   0x00
// 状態
#define BULLET_STATE_NULL  0x00
// 外部関数宣言
void BulletInitialize(void);
void BulletGenerate(s16 x,s16 y);
void BulletUpdate(void);
void BulletRender(void);
// 外部変数宣言
extern BULLET bullet[BULLET_N];
extern u8 bulletN;
