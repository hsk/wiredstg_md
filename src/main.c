#include "bios.h"
#include "App.h"
#define MAX_SPRITE       32
#define MAX_SPRITE_ANIM  64

u16 input,old_input,trigger;
int main() {
    SystemInitialize();
    AppInitialize();
    while (TRUE) {
        input = JOY_readJoypad(JOY_1);
        trigger = input & (~old_input);
        old_input = input;
        AppUpdate();
        SystemTransferSprite();
    }
    return 0;
}
