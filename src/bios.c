#include "bios.h"
#include "pal.h"
#include "gfx.h"
#include "Enemy.h"
typedef struct VSPR {
  s16 x;
  s16 y;
  const FSPR* frame;
  u16 attribute;
} VSPR;
VSPR* vsprs_pos;
VSPR vsprs[80];
u8 vdpPos=0;
char str[32];
volatile u8 flg;
static void vint() {
  if(flg)flg--;
}
void SystemInitialize(void) {
    SYS_disableInts();
    VDP_setPlaneSize(64, 64, TRUE);
    VDP_clearPlane(BG_A, 0);
    VDP_clearPlane(BG_B, 0);

    PAL_setColors(0, title_pal, 64, CPU);
    VDP_loadFontData(font,16*5,CPU);
    curTileInd = TILE_USER_INDEX;
    VDP_loadTileData(title_tiles, TILE_USER_INDEX,sizeof(title_tiles)/(8*4), TRUE);
    curTileInd += sizeof(title_tiles)/(8*4);
    VDP_setTileMapDataRectEx(BG_A, title_map,TILE_USER_INDEX, 4, 25, 24, 3, 24, CPU);
    bigcoreInd = curTileInd;
    VDP_loadTileData(bigcore_tiles, bigcoreInd,sizeof(bigcore_tiles)/(8*4), TRUE);
    curTileInd += sizeof(bigcore_tiles)/(8*4);
    VDP_setVerticalScroll(BG_B,72);
    VDP_setTileMapDataRectEx(BG_B, bigcore_map,bigcoreInd, 0, 0,12, 9, 12, CPU);
    enemyInd=curTileInd;
    for(u8 i = 0; i < 80; i++) vdpSpriteCache[i].link=i+1;
    SystemClearSprite();
    SYS_setVIntCallback(&vint);
    SYS_enableInts();
}

void SystemClearSprite(void) { // スプライトをクリアする
    vsprs_pos=vsprs;
    vdpPos=1;
}

void SystemSetSprite(u16 attribute, const FSPR* fsprs, u8 frame, s16 x, s16 y) { // スプライトを設定する
    VSPR* spr = vsprs_pos++;
    spr->attribute = attribute;
    spr->x = fix16ToInt(x)+128;
    spr->y = fix16ToInt(y)+128;
    spr->frame = &fsprs[frame];
}

#if 1
static void wait() {
    u16 n=0;
    flg=2;
    while(flg)n++;
    uintToStr(n,str,4);
    VDP_drawText(str,34,0);
}
#else
static void wait() {
    flg=2;
    while(flg);
    SystemUpdateSound();
}
#endif
void doDmaFast(void* from, u16 to, u16 len, s16 step) {
    vu16 *pw;
    u32 fromAddr;
    vu32 cmdbuf[1];
    u16* cmdbufp;

    fromAddr = (u32) from;

    VDP_setAutoInc(step);

    pw = (vu16*) VDP_CTRL_PORT;

    // Setup DMA length (in word here)
    *pw = 0x9300 + (len & 0xff);
    *pw = 0x9400 + ((len >> 8) & 0xff);

    // Setup DMA address
    fromAddr >>= 1;
    *pw = 0x9500 + (fromAddr & 0xff);
    fromAddr >>= 8;
    *pw = 0x9600 + (fromAddr & 0xff);
    fromAddr >>= 8;
    *pw = 0x9700 + (fromAddr & 0x7f);
    cmdbuf[0] = VDP_DMA_VRAM_ADDR((u32)to);
    cmdbufp = (u16*) cmdbuf;
    *pw = *cmdbufp++;
    *pw = *cmdbufp;
}
u8 updateSprite;
void SystemTransferSprite(void) {
    if (updateSprite) {
        vdpSpriteCache[vdpPos-1].link=vdpPos;
        vdpPos=0;
        for (VSPR* vs = vsprs;vs<vsprs_pos;vs++) {
            const FSPR* fs = vs->frame;
            FrameSPR* sp = fs->spr;
            for (u8 j=0;j<fs->numSpr;j++,sp++) {
                VDPSprite* v = &vdpSpriteCache[vdpPos++];
                v->x=vs->x+sp->offsetX;
                v->y=vs->y+sp->offsetY;
                v->size = sp->size;
                v->attribut = vs->attribute+sp->attribut;
            }
        }
        vsprs_pos=vsprs;
        if (vdpPos==0) {
            vdpPos=1;
            vdpSpriteCache[0].y=0;
        }
        vdpSpriteCache[vdpPos-1].link=0;
    }
    wait();
    if (updateSprite) {
        //VDP_updateSprites(vdpPos,DMA);
        //DMA_transfer(DMA, DMA_VRAM, vdpSpriteCache, VDP_SPRITE_TABLE, (sizeof(VDPSprite) * vdpPos) / 2, 2);
        doDmaFast(vdpSpriteCache, VDP_SPRITE_TABLE, (sizeof(VDPSprite)/ 2 * vdpPos) , 2);
    }
    JOY_update();
    trigger = ~input;
    input = JOY_readJoypad(JOY_1);
}
u8 log_pos = 0;
void log() {
    VDP_drawText(str,0,log_pos++);
}
