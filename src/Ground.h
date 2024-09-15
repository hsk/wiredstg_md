// Ground.h : 地面
// ジェネレータ
typedef struct GROUND_GENERATOR {
  u8 upper_state;
  u8 upper_height;
  u8 upper_length;
  u8 lower_state;
  u8 lower_height;
  u8 lower_length;
  u8 count;
} GROUND_GENERATOR;
// ジェネレータの状態
#define GROUND_GENERATOR_STATE_NULL     0x00
#define GROUND_GENERATOR_STATE_FLAT     0x01
#define GROUND_GENERATOR_STATE_UP       0x02
#define GROUND_GENERATOR_STATE_DOWN     0x03
// 外部関数宣言
void GroundInitialize(void);
void GroundUpdate(void);
void GroundRender(void);
// 外部変数宣言
extern u8 ground[0x0600];
extern GROUND_GENERATOR groundGenerator;
