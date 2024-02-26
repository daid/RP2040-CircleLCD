#pragma once

#include <stdint.h>

static constexpr unsigned int PWM_CHAN_A = 1;
static constexpr unsigned int PWM_CHAN_B = 1;


unsigned int pwm_gpio_to_slice_num(unsigned int pin);
void pwm_set_wrap(unsigned int slice, uint16_t wrap);
void pwm_set_chan_level(unsigned int slice, unsigned int channel, uint16_t level);
void pwm_set_clkdiv(unsigned int slice, float divider);
void pwm_set_enabled(unsigned int slice, bool enabled);
