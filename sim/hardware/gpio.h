#pragma once

#include <stdint.h>


static constexpr unsigned int GPIO_IN = 0;
static constexpr unsigned int GPIO_OUT = 1;

static constexpr unsigned int GPIO_FUNC_SPI = 1;

void gpio_init(unsigned int pin);
void gpio_set_dir(unsigned int pin, unsigned int direction);
void gpio_put(unsigned int pin, unsigned int value);
void gpio_set_function(unsigned int pin, unsigned int function);