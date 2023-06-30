#include "ad9833.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "log.h"

spi_inst_t* AD_SPI_INST=spi0;
const int AD_SPI_SCLK=2;
const int AD_SPI_MOSI=3;
const int AD_SPI_CS=1;

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(AD_SPI_CS, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(AD_SPI_CS, 1);
    asm volatile("nop \n nop \n nop");
}

static void write_16bits(uint16_t bits)
{
    cs_select();
    spi_write16_blocking(AD_SPI_INST, &bits, 1);
    cs_deselect();
}

void ad9833_init()
{
    spi_init(AD_SPI_INST, 10 * 1000 * 1000);
    spi_set_format( AD_SPI_INST,   // SPI instance
                16,  // Number of bits per transfer
                1,  // Polarity (CPOL)
                0, // Phase (CPHA)
                SPI_MSB_FIRST);
    gpio_set_function(AD_SPI_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(AD_SPI_MOSI, GPIO_FUNC_SPI);
    gpio_init(AD_SPI_CS);
    gpio_set_dir(AD_SPI_CS, GPIO_OUT);
    gpio_put(AD_SPI_CS, 1);
    write_16bits(0x2100);//Reset
    write_16bits(0x4000); //FREQ0 = 0
    write_16bits(0x4000);
    write_16bits(0x2000); //Un-Reset
}

#define FREQ_UPPER(x) ((x & 0xFFFC000) >> 14)
#define FREQ_LOWER(x) (x & 0x3FFF)
void set_freq(uint32_t freq)
{
    if (freq < 0) return;
    uint32_t reg = ((double)(freq << 8)/390625.0) * (1 << 14);
    LOG("Written: %u \n", reg);
    write_16bits(0x2000);
    write_16bits(0x4000 | FREQ_LOWER(reg));
    write_16bits(0x4000 | FREQ_UPPER(reg));
}