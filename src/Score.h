#define MAX_SCORE  0x99999999
#define SCORE_INIT 0x00000000
#define HISCORE_INIT 0x00001000
void ScoreInitialize(void);
void ScoreAdd(void);
void ScoreRender(void);
void ScoreToStr(u32 score,char* score_str);
extern u32 hiscore;
extern char scoreString[10];
