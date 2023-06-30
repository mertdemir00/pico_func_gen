#include <string.h>
#include "bsp/board.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "tusb.h"
#include "usbtmc_app.h"
#include "ad9833.h"
#include "log.h"
#include "st7735s.h"
#include "fonts.h"
#include "gfx.h"
#include "keypad.h"
#include "cursor.h"

void cdc_task(void);

uint8_t freq_digits[7];

void key_event_main(enum Keys key, bool isRising)
{
  if(!isRising) return;
  static bool cursor_state; 
  if(key == KEY_AST){
    if(cursor_state){
      cursor_move_right();
      draw_cursor();
    }else{
      draw_cursor();
      cursor_state = true;
    }
    return;
  }
  else if(key == KEY_POUND){
    int i;
    unsigned long freq = 0;
    for(i = 0; i < 7; i++){
      freq = freq * 10;
      freq += freq_digits[i];
    }
    set_freq(freq);
    cursor_state = false;
    clear_cursor();
    return;
  }
  else{
    if(!cursor_state) return;
    freq_digits[cursor_get_pos()] = (key == KEY_0)?0:(key + 1);
    setColor(31,63,31);
    char digit[] = "0";
    digit[0] = digit[0] + freq_digits[cursor_get_pos()];
    drawText(72 + cursor_get_pos()*12, 33, digit);
    flushBuffer();
  }

}
void draw_screen()
{
  setColor(0,0,0);
  fillScreen();
  setColor(31,63,31);
  setbgColor(0,0,0);
  setFont(ter_u24b);
  drawText(0,33, "FREQ: 0000000");
  flushBuffer();
}
int main() 
{
  stdio_init_all();
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  board_init();
  tud_init(BOARD_TUD_RHPORT);// init device stack on configured roothub port
  ad9833_init();
  ST7735S_Init();
  draw_screen();
  keys_init();
  key_event = key_event_main; // register key event
  while (true) {
    tud_task(); // tinyusb device task
    cdc_task();
    key_process();
    usbtmc_app_task_iter();
  }
}
void cdc_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < 1000) return; // not enough time
  start_ms += 1000;
  led_state = 1 - led_state; // toggle
}
//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  (void) itf;
  (void) rts;
  (void) dtr;
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  (void) itf;
}

// Invoked when device is mounted
void tud_mount_cb(void)
{

}

// Invoked when device is unmounted
void tud_umount_cb(void)
{

}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}