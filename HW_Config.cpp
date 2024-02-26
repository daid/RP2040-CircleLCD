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

void DEV_I2C_Write_Byte(i2c_inst_t* I2C_PORT, uint8_t addr, uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, addr, data, 2, false);
}

void DEV_I2C_Read_nByte(i2c_inst_t* I2C_PORT, uint8_t addr, uint8_t reg, uint8_t* buf, size_t len)
{
    i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buf, len, false);
}

void DEV_Digital_Write(uint16_t Pin, uint8_t Value)
{
    gpio_put(Pin, Value);
}

void DEV_Delay_ms(uint32_t xms)
{
    sleep_ms(xms);
}
