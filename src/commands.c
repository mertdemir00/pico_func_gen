#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "log.h"
#include "ad9833.h"
#include "st7735s.h"
#include "fonts.h"
#include "gfx.h"
#include "cursor.h"

uint8_t out_buffer[BUF_SIZE]; // A few packets long should be enough.
static size_t buffer_len;

struct Command{
    const char *first_word;
    void (*command_handler)(const uint8_t *args);
};

static void idn_command(const uint8_t *args);
static void freq_command(const uint8_t* args);
static void cursor_command(const uint8_t* args);

static struct Command commands[] =
{
    {"*idn?", idn_command},
    {"freq", freq_command},
    {"cursor", cursor_command}
};

static size_t compare_until_space(const char *cmd_first_word, const uint8_t *cmd_data)
{
    size_t len_until_space;
    for(len_until_space = 0; len_until_space < strlen((const char*)cmd_data); len_until_space++){
        if(cmd_data[len_until_space] == ' ') break;
    }
    if(!strncasecmp(cmd_first_word, (const char *)cmd_data, strlen(cmd_first_word))){
        return len_until_space;
    }
    else{
        return 0;
    }
}

size_t handle_command(const uint8_t *cmd_data)
{
    LOG("COMMAND RECEIVED:%s\n", cmd_data);
    const int total_commands = sizeof(commands)/sizeof(commands[0]);
    
    int i;
    int len_until_space;
    for(i = 0; i < total_commands; i++){
        if((len_until_space = compare_until_space(commands[i].first_word, cmd_data)) != 0)
        {
            commands[i].command_handler(cmd_data + len_until_space + 1);
            return buffer_len;
        }
    }
    return 0;
}

static void setup_response(const char *data)
{
    buffer_len = strlen(data);
    strncpy((char *)out_buffer, data, buffer_len);
}

static void idn_command(const uint8_t *args)
{
    LOG("%s called\n",__func__);
    const char idn_str[] = "Fungen,MK0,1,V0.0.0.0\r\n";
    setup_response(idn_str);
}

static void freq_command(const uint8_t* args)
{
    LOG("%s called\n",__func__);
    unsigned long freq = strtoul((const char*)args, NULL, 10);
    if(freq >= 10000000) return;
    set_freq(freq);
    char freqtxt[15];
    sniprintf(freqtxt,14, "FREQ: %07d",freq);
    setColor(0,0,0);
    filledRect(80,33,160,56);
    setColor(31,63,31);
    drawText(0,33, freqtxt);
    flushBuffer();
}

static void cursor_command(const uint8_t* args)
{
    LOG("%s called\n",__func__);
    unsigned long cursor = strtoul((const char*)args, NULL, 10);
    cursor_set_pos(cursor);
    draw_cursor();
}