#include "bios.h"
#include "Score.h"
//#include "App.h"
static inline u8 abcd(u8 d0, u8 d1, u8 flags_in, u8* flags_out) {
	u8 res;
	asm volatile ("move %4,%%cc\n"
		"abcd %3,%0\n"
		"move %%sr,%1\n"
		: "=d" (res), "=d" (*flags_out)
		: "0" (d0), "d" (d1), "d" (flags_in)
		: "cc"
	);
	return res;
}
static inline u32 abcd32(u32 a,u32 b) {
  u8 flg=0;
  u32 res=0;
  for(int i=0;i<4;i++) {
    res += abcd(a & 0xff,b & 0xff,flg,&flg)<<(i*8);
    a >>= 8;
    b >>= 8;
  }
  if (flg&1) return 0x99999900;
  return res;
}
static u8 sbcd_(u8 d0, u8 d1, u8 flags_in, u8* flags_out) {
	u8 res;
	asm volatile ("move %4,%%cc\n"
		"sbcd %3,%0\n"
		"move %%sr,%1\n"
		: "=d" (res), "=d" (*flags_out)
		: "0" (d0), "d" (d1), "d" (flags_in)
		: "cc"
	);
	return res;
}
u32 sbcd32(u32 a,u32 b) {
  u8 flg=0;
  u32 res=0;
  for(int i=0;i<4;i++) {
    res += sbcd_(a & 0xff,b & 0xff,flg,&flg)<<(i*8);
    a >>= 8;
    b >>= 8;
  }
  if (flg) return 0;
  return res;
}
u32 score = SCORE_INIT;
u32 hiscore = HISCORE_INIT;
char scoreString[10];
u8 scoreInvalid=0;
void ScoreInitialize(void) {
  scoreInvalid=1;
  scoreString[8]=0;
  score = SCORE_INIT;
  VDP_drawText("1P",0,0);
  VDP_drawText("HI",12,0);
}
void ScoreAdd(void) {
  score = abcd32(score,0x100);
  if(hiscore<score)hiscore=score;
  scoreInvalid=1;
}
void ScoreToStr(u32 score,char* scoreString) {
  int j=28;
  for(;j>=1;j-=4) {
    if(score>>j) break;
    *scoreString++ = ' ';
  }
  for(;j>=0;j-=4) *scoreString++ = ((score>>j) & 0xf)+'0'; 
}

void ScoreRender(void) {
  if(!scoreInvalid) return;
  ScoreToStr(score,scoreString);
  VDP_drawText(scoreString,3,0);
  ScoreToStr(hiscore,scoreString);
  VDP_drawText(scoreString,20,0);
  scoreInvalid=0;
}
