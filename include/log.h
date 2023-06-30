#pragma once
#include <stdio.h>
#include "tusb.h"
extern char tx_buf[];
#define LOG(x,...) do{snprintf(tx_buf,999,x, ##__VA_ARGS__);\
                  tud_cdc_write(tx_buf, strnlen(tx_buf,999));\
                  tud_cdc_write_flush();}while(0)
