// bios.h : BIOS コール
#include "genesis.h"
#define MAX_SPRITE       32
#define MAX_SPRITE_ANIM  64

typedef struct
{
    s8 offsetY;          // respect VDP sprite field order, may help
    u8 offsetYFlip;
    u8 size;
    s8 offsetX;
    u8 offsetXFlip;
    u16 attribut;
}  FrameSPR;
typedef struct FSPR {
  u8 numSpr;
  u8 no;
  u16 tileno;
  FrameSPR* spr;
} FSPR;

extern u16 input,trigger;
extern u8 sprite[128];
extern Sprite* sprites[MAX_SPRITE];
extern u16 animVramIndexes[MAX_SPRITE_ANIM];
extern char str[32];
extern u8 updateSprite;
void SystemInitialize(void);
void SystemClearSprite(void);
void SystemTransferSprite(void);
void SystemSetSprite(u16 attribute, const FSPR* fsprs, u8 frame, s16 y, s16 x);
