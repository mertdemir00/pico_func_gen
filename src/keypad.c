#include "keypad.h"
#include "hardware/gpio.h"
#include "log.h"
#include "pico/stdlib.h"

#define GET_KEY(row_i, col_i) ((enum Keys)(row_i * 3 + col_i))

//COLUMNS OUTPUT, ROWS INPUT
#define ROW1_IO 22
#define ROW2_IO 21
#define ROW3_IO 20
#define ROW4_IO 19
#define COL1_IO 18
#define COL2_IO 17
#define COL3_IO 16

static int row_io[] = 
{
    ROW1_IO,
    ROW2_IO,
    ROW3_IO,
    ROW4_IO,
};

static int col_io[] = 
{
    COL1_IO,
    COL2_IO,
    COL3_IO
};

static const char key_names[][10] =
{
    "KEY_1",
    "KEY_2",
    "KEY_3",
    "KEY_4",
    "KEY_5",
    "KEY_6",
    "KEY_7",
    "KEY_8",
    "KEY_9",
    "KEY_AST",
    "KEY_0",
    "KEY_POUND"
};

static void generate_event(enum Keys key, bool isRising);
static bool prev_states[12];

void (*key_event)(enum Keys key, bool isRising) = NULL;

void keys_init(void)
{
    int i;
    for(i = 0; i < 4; i++){
        gpio_init(row_io[i]);
        gpio_pull_down(row_io[i]);
    }
    for(i = 0; i < 3; i++){
        gpio_init(col_io[i]);
        gpio_set_dir(col_io[i], GPIO_OUT);
        gpio_put(col_io[i], 1);
    }
}
//CALL IN A LOOP
void key_process(void)
{
    bool states[10] = {0};
    int i,j;
    for(i = 0; i < 3; i++){
        gpio_put(col_io[i], 1);
        gpio_put(col_io[(i + 1)%3], 0);
        gpio_put(col_io[(i + 2)%3], 0);
        sleep_ms(1);
        for(j = 0; j < 4; j++){
            states[GET_KEY(j,i)] = gpio_get(row_io[j]);
        }
    }
    for(i = 0; i < 12; i++){
        if(states[i] != prev_states[i]){
            generate_event(i, states[i]);
        }
        prev_states[i] = states[i];
    }
}

static void generate_event(enum Keys key, bool isRising)
{
    if(isRising){
        LOG("%s pressed.\n", key_names[key]);
    }
    else{
        LOG("%s released.\n", key_names[key]);
    }
    if(key_event)
    {
        key_event(key, isRising);
    }
}