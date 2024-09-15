// Sound.c : サウンドライブラリ
#include "bios.h"
u8 soundPriority;
// 定数の定義
static u16 const soundToneFrequencyTable[] = {
    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x03f9,0x03c0,0x038a,0x0357,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x021b,
    0x01fc,0x01e0,0x01c5,0x01ac,0x0357,0x0327,0x02fa,0x02cf,0x02a7,0x0281,0x0281,0x025d,0x023b,0x021b,0x0000,0x010e,
    0x00ff,0x00f0,0x00e3,0x00d6,0x01ac,0x0193,0x017d,0x0167,0x0154,0x0140,0x0140,0x012e,0x011d,0x010e,0x0000,0x0086,
    0x007f,0x0078,0x0071,0x006b,0x00d6,0x00ca,0x00be,0x00b4,0x00a9,0x00a0,0x00a0,0x0097,0x008f,0x0086,0x0000,0x0044,
    0x0040,0x003c,0x0039,0x0036,0x006b,0x0065,0x005f,0x005a,0x0055,0x0050,0x0050,0x004c,0x0048,0x0044,0x0000,0x0022,
    0x0020,0x001e,0x001d,0x001b,0x0036,0x0032,0x0030,0x002d,0x002a,0x0028,0x0028,0x0026,0x0024,0x0022,0x0000,0x0011,
    0x0010,0x000f,0x000e,0x000d,0x001b,0x0019,0x0018,0x0016,0x0015,0x0014,0x0014,0x0013,0x0012,0x0011,0x0000,0x0000,
};
static u8 const soundLengthTable[] = {
    1,       // T1 L0 32
    2,       // T1 L1 16
    3,       // T1 L2 16.
    4,       // T1 L3  8
    6,       // T1 L4  8.
    8,       // T1 L5  4
    12,      // T1 L6  4.
    16,      // T1 L7  2
    20,      // T1 L8  2.
    24,      // T1 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    2,       // T2 L0 32
    4,       // T2 L1 16
    6,       // T2 L2 16.
    8,       // T2 L3  8
    12,      // T2 L4  8.
    16,      // T2 L5  4
    24,      // T2 L6  4.
    32,      // T2 L7  2
    48,      // T2 L8  2.
    64,      // T2 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    3,       // T3 L0 32
    6,       // T3 L1 16
    9,       // T3 L2 16.
    12,      // T3 L3  8
    18,      // T3 L4  8.
    24,      // T3 L5  4
    36,      // T3 L6  4.
    48,      // T3 L7  2
    72,      // T3 L8  2.
    96,      // T3 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    4,       // T4 L0 32
    8,       // T4 L1 16
    12,      // T4 L2 16.
    16,      // T4 L3  8
    24,      // T4 L4  8.
    32,      // T4 L5  4
    48,      // T4 L6  4.
    64,      // T4 L7  2
    96,      // T4 L8  2.
    128,     // T4 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    5,       // T5 L0 32
    10,      // T5 L1 16
    15,      // T5 L2 16.
    20,      // T5 L3  8
    30,      // T5 L4  8.
    40,      // T5 L5  4
    60,      // T5 L6  4.
    80,      // T5 L7  2
    120,     // T5 L8  2.
    160,     // T5 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    6,       // T6 L0 32
    12,      // T6 L1 16
    18,      // T6 L2 16.
    24,      // T6 L3  8
    32,      // T6 L4  8.
    48,      // T6 L5  4
    72,      // T6 L6  4.
    96,      // T6 L7  2
    144,     // T6 L8  2.
    192,     // T6 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    7,       // T7 L0 32
    14,      // T7 L1 16
    21,      // T7 L2 16.
    28,      // T7 L3  8
    42,      // T7 L4  8.
    56,      // T7 L5  4
    84,      // T7 L6  4.
    112,     // T7 L7  2
    168,     // T7 L8  2.
    224,     // T7 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    8,       // T8 L0 32
    16,      // T8 L1 16
    24,      // T8 L2 16.
    32,      // T8 L3  8
    48,      // T8 L4  8.
    64,      // T8 L5  4
    96,      // T8 L6  4.
    128,     // T8 L7  2
    192,     // T8 L8  2.
    0,       // T8 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
};

// 変数の定義
volatile u8* soundRequest[4];    // サウンド
u8* soundHead[4];
volatile u8* soundPlay[4];
static u8 soundT[4];
static u8 soundV[4];
static u8 soundO[4];
static u8 soundL[4];
static u16 soundFrequency[4];
static u8 soundRest[4];
static u8 soundUpdate[4];
static void SystemClearSoundChannel(u8 a);
void SystemInitializeSound(void) { // サウンドを初期化する
    // 1. サウンドデータの初期化
        soundRequest[0] = 0;
        soundRequest[1] = 0;
        soundRequest[2] = 0;
        soundRequest[3] = 0;
        soundHead[0] = 0;
        soundHead[1] = 0;
        soundHead[2] = 0;
        soundHead[3] = 0;
        soundPlay[0] = 0;
        soundPlay[1] = 0;
        soundPlay[2] = 0;
        soundPlay[3] = 0;
    // 2. サウンドパラメータの初期化
        for(u8 i=0;i<4;i++) SystemClearSoundChannel(i);
}
static void SystemUpdateSoundChannel(void) { // サウンドを更新する
    u8 a;
    // チャンネルの走査
    for(u16 bc = 0;bc<4;bc++){ // １チャンネルの処理
        if (soundRequest[bc]) {// リクエスト
            soundHead[bc] = (u8*)soundRequest[bc];
            soundPlay[bc] = (u8*)soundRequest[bc];
            soundRequest[bc] = 0;
            SystemClearSoundChannel(bc);
        }
        if (soundPlay[bc]==0) continue;// サウンドデータの存在
        if(--soundRest[bc])continue;// 待機
        u8* hl = (u8*)soundPlay[bc];// 再生ポインタの取得
        for(;;) { // MML の解析
            switch (a=*hl++) {
            case 0x00: // 0x00 : 終端コード
                soundHead[bc]=0;
                soundPlay[bc]=0;
                soundFrequency[bc]=0;
                soundUpdate[bc]=1;
                goto SystemUpdateSoundNext;
            case 0xff: // $ff : 繰り返し
                soundPlay[bc]=hl=soundHead[bc];
                continue;
            case 'T': // 'T' : テンポ（T1 ～ T8）
                soundT[bc] = *hl++ - '1';
                continue;
            case 'V': // 'V' : 音量（V0 ～ V16）
                soundV[bc] = *hl++ - '0';
                a = *hl;
                if (a < '0' || '9' < a) continue;
                soundV[bc] = 10 + a - '0';
                hl++;
                continue;
            case 'O': // 'O' : オクターブ（O1 ～ O8）
                soundO[bc] = *hl++ - '1';
                continue;
            case '>': // 'O' : オクターブ（O1 ～ O8）
                soundO[bc]--;
                continue;
            case '<': // 'O' : オクターブ（O1 ～ O8）
                soundO[bc]++;
                continue;
            case 'L': // 'L' : 音の長さ（L0 ～ L9）
                soundL[bc] = *hl++ - '0';
                continue;
            case 'R': // 'R' : 休符
                soundFrequency[bc]=0;
                break;
            default: // 'A' : 音符
                {
                    u8 ix = (soundO[bc]*16)+((a-'A')*2);
                    if (*hl=='+') {
                        hl++;
                        ix++;
                    } else
                    if (*hl=='-') {
                        hl++;
                        ix--;
                    }
                    soundFrequency[bc]=soundToneFrequencyTable[ix];
                }
                break;
            }
            // 音の長さの設定
            a = *hl;
            if ('0' <= a && a <= '9') {
                a -= '0';
                hl++;
            } else {// else
                a = soundL[bc];
            }
            soundRest[bc]=soundLengthTable[(soundT[bc]<<4)+a];
            soundUpdate[bc]=0x1;
            soundPlay[bc]=hl;// 再生ポインタの保存
            break;// チャンネルの走査の完了
        }
    SystemUpdateSoundNext: // 次のチャンネルへ
    }
}
static void SetSoundChannel(u8 a,u8 b) {// チャンネルの設定
    if (soundUpdate[a]) {
        if (soundFrequency[a]) {
            PSG_setEnvelope(b,16-soundV[a]);
            PSG_setTone(b,soundFrequency[a]);
        } else {
            PSG_setEnvelope(b,15);
            if(soundPlay[a]==0) soundUpdate[a]=0;
        }
    }
}
void SystemUpdateSound(void) { // サウンドを更新する
    SystemUpdateSoundChannel();
    SetSoundChannel(0,0);
    SetSoundChannel(1,1);
    if(!soundUpdate[3])SetSoundChannel(2,2);
    SetSoundChannel(3,2);
}
static void SystemClearSoundChannel(u8 a) { // サウンドのチャンネルをクリアする
    soundT[a] = 0;
    soundV[a] = 0x0f;
    soundO[a] = 0x03;
    soundL[a] = 0x05;
    soundFrequency[a]=0;
    soundRest[a]=0x01;
    soundUpdate[a]=0;
}
#define VOL0 "V13"
#define VOL1 "V12"

// ＳＥ
char const enemyBigCoreSeHit[] = "T1V15L0O7A";
char const enemyBigCoreSeBomb[] =
    "T1V15L0"
    "O4G>D-<E>D-<C>D-GD-ED-"
    "C>D-<D->C<C>D-<D->C<C>D-<D->C<C>D-<D->C";
char const enemyBombSe[] = "T1V15L0O4GFEDC>BAG";


// 操作
char const gamePlayBgm0[] =
    "T2" VOL0 "L1"
    //"O3G5G4O4CF3E5C3"
    //"O4G4O3B-B-6GA-B-O4E-FG"
    //"O4C4O3A4O4C5F3O3ABO4CD"
    //"O4E-3CO3ARO4E-3CF3DO3B-RO4DF3"
    //"O3G5G4O4CF3E5C3"
    //"O4G4O3B-B-6GA-B-O4E-FG"
    //"O4A3CC4F3G3A5F3"
    //"O4D4G8R"
    //"O5E-4C4E-6CDE-3"
    //"O5D4O4B-4G7R3"
    //"O5C4O4A4O5CCD4O4B4O5DD"
    //"O5E-4C4E-FF4D4G9R3"
    "O4F3DR>B-<DF3CE-3R>A<CE-3"
    "DC>BA<F3C5>A4<C4"
    "GFE->B-A-GB-6B-<G4"
    "C3E5F3C>G4G5<"
    "G8RD4"
    "F3A5G3F3C4CA3"
    "GFE->B-A-GB-6B-<G4"
    "C3E5F3C>G4G5<"
    "<G9R3D4F4FE-C4E-4"
    "DD>B4<D4CC>A4<C4>"
    "G7R3B-4<D4"
    "E-3DCE-6C4E-4"
    "\xff";
char const gamePlayBgm1[] =
    "T2" VOL0 "L1"
    //"O5CO4BAGFGABO5CO4BAGFGAB"
    //"O5CO4B-A-GFGA-B-O5CO4B-A-GFGA-B-"
    //"O5CO4BAGFGABO5CO4BAGFGAB"
    //"O5CO4B-A-GFGA-B-O5CDE-DCO4B-A-B-"
    //"O5CO4BAGFGABO5CO4BAGFGAB"
    //"O5CO4B-A-GFGA-B-O5CO4B-A-GFGA-B-"
    //"O5CO4BAGFGABO5CO4BAGFGAF"
    //"O5DCO4BAGABO5CDCO4BAGBO5DG"
    //"V15"
    //"O4C4E-4A-7R3"
    //"O3B-4O4D4E-7R3"
    //"O3AB-O4CF5RO3BO4CDG5R"
    //"O4CDE-A-5RDE-FB-4O5C7R3O4B7"
    "O4B-A-B-<CDE-DC>B-A-GFGA-B-<C>"
    "BAGFGAB<C>BAGFGAB<C>"
    "B-A-GFGA-B-<C>B-A-GFGA-B-<C>"
    "BAGFGAB<C>BAGFGAB<C"
    "GD>BGAB<CDC>BAGAB<CD>"
    "FAGFGAB<C>BAGFGAB<C>"
    "B-A-GFGA-B-<C>B-A-GFGA-B-<C>"
    "BAGFGAB<C>BAGFGAB<C>"
    VOL1
    "B7<C7R3>B-4FE-DA-5RE-DC"
    "G5RDC>B<F5RC>B-A<"
    "E-7R3D4>B-4<"
    "A-7R3E-4C4"
    "\xff";
char const gamePlayBgm2[] =
    "T2" VOL0 "L1"
    //"O2C3O3CCO2C3O3CCO2C3O3CCO2C3O3CC"
    //"O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-"
    //"O2F3O3FFO2F3O3FFO2F3O3FFO2F3O3FF"
    //"O2A-3O3A-A-O2A-3O3A-A-O2A-3O3A-A-O2A-3O3A-A-"
    //"O2C3O3CCO2C3O3CCO2C3O3CCO2C3O3CC"
    //"O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-O2E-3O3E-E-"
    //"O2F3O3FFO2F3O3FFO2F3O3FFO2F3O3FF"
    //"O2G3O3GGO2G3O3GGO2G3O3GGO2G3O3GG"
    //"O2A-O3A-E-A-O2A-O3A-E-A-O2A-O3A-E-A-O2A-O3A-E-A-"
    //"O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-O2E-O3E-O2B-O3E-"
    //"O2FO3FCFO2FO3FCFO2GO3GDGO2GO3GDG"
    //"O2A-O3A-E-A-O2A-O3A-E-A-O2B-O3B-FB-O2B-O3B-FB-"
    //"O2G3O3GGO2G3O3GGO2G3O3GGO2G3O3GG"
    "O4A-A->A-3<A-A->A-3<A-A->A-3<A-A->A-3<"
    "FF>F3<FF>F3<FF>F3<FF>F3"
    "<E-E->E-3<E-E->E-3<E-E->E-3<E-E->E-3"
    "<CC>C3<CC>C3<CC>C3<CC>C3"
    "<GG>G3<GG>G3<GG>G3<GG>G3"
    "<FF>F3<FF>F3<FF>F3<FF>F3"
    "<E-E->E-3<E-E->E-3<E-E->E-3<E-E->E-3"
    "<CC>C3<CC>C3<CC>C3<CC>C3"
    "<GG>G3<GG>G3<GG>G3<GG>G3"
    "<B-FB->B-<B-FB->B-<A-E-A->A-<A-E-A->A-"
    "<GDG>G<GDG>G<FCF>F<FCF>F"
    "<E->B-<E->E-<E->B-<E->E-<E->B-<E->E-<E->B-<E->E-"
    "<A-E-A->A-<A-E-A->A-<A-E-A->A-<A-E-A->A-"
    "\xff";
char const gameOverBgm0[] =
    "T2" VOL0 "L1"
    //"O4GECGECGECGEC"
    //"O5CO4GEO5CO4GEO5CO4GEO5CO4GE"
    //"O5ECO4GEGO5CGECO4GO5CE"
    //"O6CO5GEGECE6"
    "O5E6CEGEG<C>"
    "EC>G<CEGC>GEG<CE"
    ">EG<C>EG<C>EG<C>EG<C"
    ">CEGCEGCEGCEG";
char const gameOverBgm1[] =
    "T2" VOL0 "L1"
    //"O4CO3GEO4CO3GEO4CO3GEO4CO3GE"
    //"O4ECO3GO4ECO3GO4ECO3GO4ECO3G"
    //"O4GECO3GO4CEO5CO4GECEG"
    //"O5ECO4GO5CO4GEG6"
    "O4G6EG<C>G<CE>"
    "GECEG<C>EC>G<CEG"
    ">G<CE>G<CE>G<CE>G<CE"
    ">EG<C>EG<C>EG<C>EG<C";
char const gameOverBgm2[] =
    "T2" VOL0 "L1"
    //"O2C5CCC4O1G3"
    //"O2E5EEE4C3"
    //"O2G5GGG4C3"
    //"O3C5O2CCC4R3"
    "O3C4R3CC<C5"
    ">C3G4GGG5"
    "C3E4EEE5"
    ">G3<C4CCC5";
char const shipSe[] =
    //"T1V15L0O4GO2D-O4EO2D-O4CO2D-O3GO2D-O3EO2D-O3CO2D-O2GD-ED-"
    "T1V15L0O4G>>D-<<E>>D-<<C>>D-<G>D-<E>D-<C>D-GD-ED-";
char const shotSe[] = "T1" VOL0 "L0O6C>F+<C>F+C>F+";
// ジングル
char const titleJingle0[] = "T2" VOL0 "L1"/*"O4BO5DGBG5R7"*/"O5G5BGD>BR7";
char const titleJingle1[] = "T2" VOL0 "L1"/*"O4DGBO5DO4B5R7"*/"O4B5<D>BGDR7";
char const titleJingle2[] = "T2" VOL0 "L1"/*"O3G4O4DO3G5R7"*/"O3G5<D>G4R7";
