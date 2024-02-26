#pragma once

#include <stdint.h>

struct spi_inst_t {
};

extern struct spi_inst_t* spi1;

void spi_init(spi_inst_t* inst, unsigned int frequency);
void spi_write_blocking(spi_inst_t* inst, const uint8_t* src, size_t len);
