// Math.c : 数学ライブラリ
#include "bios.h"

u8 SystemGetAtan2(s8 h,s8 l) { // atan2 の値を取得する
    static u8 const trigonometricAtanAngleTable[] = {
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x40, 0x20, 0x12, 0x0d, 0x09, 0x08, 0x06, 0x05,
        0x40, 0x2d, 0x20, 0x17, 0x12, 0x0f, 0x0d, 0x0b,
        0x40, 0x32, 0x28, 0x20, 0x1a, 0x16, 0x12, 0x10,
        0x40, 0x36, 0x2d, 0x25, 0x20, 0x1b, 0x17, 0x15,
        0x40, 0x37, 0x30, 0x29, 0x24, 0x20, 0x1c, 0x19,
        0x40, 0x39, 0x32, 0x2d, 0x28, 0x23, 0x20, 0x1c,
        0x40, 0x3a, 0x34, 0x2f, 0x2a, 0x26, 0x23, 0x20,
    };
    static u8 const trigonometricAtanOffsetTable[] = {
        0x00, 0x80, 0x00, 0x80,
    };
    // 値の取得
    u8 c = 0;
    if (h < 0) {
        c |= 1;
        h = -h;
    }
    if (l < 0) {
        c |= 2;
        l = -l;
    }
    // 両方8以下にする
    if (h > l) {
        for(;h>=8;) {
            h = h >> 1;
            l = l >> 1;
        }
    } else {
        for(;l>=8;) {
            h = h >> 1;
            l = l >> 1;
        }
    }
    u8 a = trigonometricAtanAngleTable[(l<<3)+h];
    return trigonometricAtanOffsetTable[c] + (((c + 1) & 2) ? -a : a);
}
