#pragma once
#include <stdbool.h>

enum Keys{
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_AST,
    KEY_0,
    KEY_POUND
};

void key_process(void);
void keys_init(void);
extern void (*key_event)(enum Keys key, bool isRising);