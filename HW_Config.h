#pragma once

#include "hardware/spi.h"
#include "hardware/i2c.h"

#define LCD_DC_PIN      (8)
#define LCD_CS_PIN      (9)
#define LCD_CLK_PIN     (10)
#define LCD_MOSI_PIN    (11)
#define LCD_RST_PIN     (12)
#define LCD_BL_PIN      (25)

#define LCD_SPI_PORT    (spi1)

#define SENSOR_I2C_PORT (nullptr)

void DEV_SPI_WriteByte(spi_inst_t* SPI_PORT,uint8_t Value);
void DEV_SPI_Write_nByte(spi_inst_t* SPI_PORT,uint8_t *pData, uint32_t Len);

void DEV_I2C_Write_Byte(i2c_inst_t* I2C_PORT, uint8_t addr, uint8_t reg, uint8_t value);
void DEV_I2C_Read_nByte(i2c_inst_t* I2C_PORT, uint8_t addr, uint8_t reg, void* buf, size_t len);

void DEV_Digital_Write(uint16_t Pin, uint8_t Value);
void DEV_Delay_ms(uint32_t xms);
