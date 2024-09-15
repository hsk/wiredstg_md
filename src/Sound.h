extern volatile char* soundRequest[4];// サウンド
extern u8* soundHead[4];
extern volatile u8* soundPlay[4];
extern u8 soundPriority;
void SystemInitializeSound(void);
void SystemUpdateSound(void);
void SystemSuspendSound(void);
void SystemResumeSound(void);

extern char const enemyBigCoreSeHit[];
extern char const enemyBigCoreSeBomb[];
extern char const enemyBombSe[];
extern char const gamePlayBgm0[];
extern char const gamePlayBgm1[];
extern char const gamePlayBgm2[];
extern char const gameOverBgm0[];
extern char const gameOverBgm1[];
extern char const gameOverBgm2[];
extern char const shipSe[];
extern char const shotSe[];
// ジングル
extern char const titleJingle0[];
extern char const titleJingle1[];
extern char const titleJingle2[];

#define BGM(v) {\
            soundRequest[0] = (void*)v##0;\
            soundRequest[1] = (void*)v##1;\
            soundRequest[2] = (void*)v##2;}
#define SOUNDWAIT() (!(soundRequest[0]||soundPlay[0]))
#define SE(v) {soundRequest[3] = (void*)v;soundPriority=0;}
//#define SE0(v) {soundRequest[3] = (void*)v;}
#define SE0(v) {if(soundPriority==1||soundPlay[3]==0){soundRequest[3] = (void*)v;soundPriority=1;}}
