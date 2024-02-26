#include "HW_Config.h"
#include "hardware/gpio.h"
#include "pico/time.h"


void DEV_SPI_WriteByte(spi_inst_t *SPI_PORT,uint8_t Value)
{
    spi_write_blocking(SPI_PORT, &Value, 1);
}

void DEV_SPI_Write_nByte(spi_inst_t *SPI_PORT,uint8_t *pData, uint32_t Len)
{
    spi_write_blocking(SPI_PORT, pData, Len);
}

void DEV_Digital_Write(uint16_t Pin, uint8_t Value)
{
    gpio_put(Pin, Value);
}

void DEV_Delay_ms(uint32_t xms)
{
    sleep_ms(xms);
}
