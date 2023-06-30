#include "st7735s.h"
#include "gfx.h"
#include "cursor.h"

#define CURSOR_START 0
#define CURSOR_END 6

int cursor_pos = 0;

const int cursor_wid_px = 12;
const int cursor_start_x = cursor_wid_px * 6;
const int cursor_start_y = 60;

int cursor_get_pos()
{
    return cursor_pos;
}

void cursor_set_pos(int pos)
{
    if(pos >= CURSOR_END){
        cursor_pos = CURSOR_END;
    }
    else if(pos <= CURSOR_START){
        cursor_pos = CURSOR_START;
    }
    else{
        cursor_pos = pos;
    }
}

void cursor_move_right()
{
    if(cursor_pos >= CURSOR_END)
    {
        cursor_pos = CURSOR_START;
        return;
    }
    cursor_pos++;
}

void draw_cursor()
{
    clear_cursor();
    setColor(31,63,31);
    drawLine(cursor_start_x + cursor_wid_px*cursor_pos,
     cursor_start_y,
      cursor_start_x + cursor_wid_px*(cursor_pos + 1),
       cursor_start_y);
    flushBuffer();
}

void clear_cursor()
{
    setColor(0,0,0);
    drawLine(cursor_start_x,
     cursor_start_y,
      cursor_start_x + cursor_wid_px*(CURSOR_END - CURSOR_START + 1),
       cursor_start_y);
    flushBuffer();
}

