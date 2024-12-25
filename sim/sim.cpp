#include <stdio.h>
#include <stdint.h>
#include <SDL.h>


static unsigned int gpio_pin_state[32];

class LCDSim
{
public:
    enum class State {
        ImageData,
        XCoords,
        YCoords,
        Unknown,
    } state;
    uint8_t data_buffer[32];
    size_t data_index;
    int start_x, end_x;
    int start_y, end_y;
    int cur_x, cur_y;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool running = true;
    uint16_t buffer[240*240];
    int xmin[240];

    LCDSim()
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_CreateWindowAndRenderer(240, 240, 0, &window, &renderer);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 240, 240);
        for(int y=0; y<240; y++)
            xmin[y] = 120 - std::sqrt(120*120-((y-120)*(y-120)));
    }
    
    ~LCDSim()
    {
        while(running) {
            update();
        }
    }

    void update()
    {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT: running = false; break;
            }
        }
        SDL_RenderClear(renderer);
        uint16_t* pixels;
        int pitch;
        SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);
        memcpy(pixels, buffer, sizeof(buffer));
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    void cmd(uint8_t cmd)
    {
        switch(cmd) {
        case 0x2A: state = State::XCoords; break;
        case 0x2B: state = State::YCoords; break;
        case 0x2C: state = State::ImageData; break;
        default:
            printf("Unknown LCD command: %02x\n", cmd);
            state = State::Unknown;
        }
        data_index = 0;
    }

    void data(uint8_t data)
    {
        data_buffer[data_index] = data;
        if (data_index < sizeof(data_buffer) - 1) data_index += 1;
        switch(state) {
        case State::XCoords:
            if (data_index == 4) {
                start_x = data_buffer[1];
                end_x = data_buffer[3];
                cur_x = start_x;
            }
            break;
        case State::YCoords:
            if (data_index == 4) {
                start_y = data_buffer[1];
                end_y = data_buffer[3];
                cur_y = start_y;
            }
            break;
        case State::ImageData:
            if (data_index == 2) {
                if (cur_y >= 0 && cur_y < 240 && cur_x >= xmin[cur_y] && cur_x < 240 - xmin[cur_y])
                    buffer[cur_x + cur_y * 240] = (data_buffer[1] << 8) | data_buffer[0];
                cur_x += 1;
                if (cur_x == end_x) {
                    cur_x = start_x;
                    cur_y += 1;
                    if (cur_y == end_y)
                        cur_y = start_y;
                }
                data_index = 0;
            }
            break;
        case State::Unknown:
            break;
        }
    }
};

static LCDSim lcd_sim;

#include "pico/time.h"

void sleep_ms(uint32_t delay_ms) {}

#include "hardware/gpio.h"

void gpio_init(unsigned int pin) {}
void gpio_set_dir(unsigned int pin, unsigned int direction) {}
void gpio_put(unsigned int pin, unsigned int value) { if (pin < 32) gpio_pin_state[pin] = value; }
void gpio_set_function(unsigned int pin, unsigned int function) {}
void gpio_pull_up(unsigned int pin) {}

#include "hardware/pwm.h"

unsigned int pwm_gpio_to_slice_num(unsigned int pin) { return pin; }
void pwm_set_wrap(unsigned int slice, uint16_t wrap) {}
void pwm_set_chan_level(unsigned int slice, unsigned int channel, uint16_t level) {}
void pwm_set_clkdiv(unsigned int slice, float divider) {}
void pwm_set_enabled(unsigned int slice, bool enabled) {}

#include "hardware/spi.h"

struct spi_inst_t* spi1;

void spi_init(spi_inst_t* inst, unsigned int frequency) {}
void spi_write_blocking(spi_inst_t* inst, const uint8_t* src, size_t len) {
    while(len--)
        if (!gpio_pin_state[8]) lcd_sim.cmd(*src++); else lcd_sim.data(*src++);
}

#include "hardware/i2c.h"

struct i2c_inst_t* i2c1;

unsigned int i2c_init (i2c_inst_t *i2c, unsigned int baudrate) { return 0; }
int i2c_write_blocking (i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop) { return 0; }
int i2c_read_blocking (i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop) {
    memset(dst, 0, len);
    return 0;
}
