// Ship.h : 自機
// 自機
#define SHIP_TYPE          0x00
#define SHIP_STATE         0x01
#define SHIP_POSITION_X    0x02
#define SHIP_POSITION_Y    0x03
#define SHIP_HP            0x04
#define SHIP_SPEED         0x05
#define SHIP_ANIMATION     0x06
#define SHIP_TIMER         0x07
#define SHIP_SHOT_L        0x08
#define SHIP_SHOT_H        0x09
#define SHIP_SIZE          0x0a

typedef struct {
    u8 kind;
    u8 state;
    s16 x;
    s16 y;
    u8 hp;
    s16 speed;
    u8 animation;
    u8 timer;
    u8 shot_l;
    u8 shot_h;     
} SHIP;

// 種類
#define SHIP_TYPE_NULL     0x00
#define SHIP_TYPE_VICVIPER 0x01
// 状態
#define SHIP_STATE_NULL    0x00
// 外部関数宣言
void ShipInitialize(void);
void ShipUpdate(void);
void ShipRender(void);
// 外部変数宣言
extern SHIP ship;
extern u16 shipInd;
