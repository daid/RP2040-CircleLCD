#pragma once

#include <stdint.h>

struct i2c_inst_t {};

extern struct i2c_inst_t* i2c1;

unsigned int i2c_init (i2c_inst_t *i2c, unsigned int baudrate);
int i2c_write_blocking (i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop);
int i2c_read_blocking (i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop);
