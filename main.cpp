#include "LCD/LCD_1in28.h"
#include "accel/QMI8658.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdio_usb.h"
#include <algorithm>


void DEV_GPIO_Mode(uint16_t Pin, uint16_t Mode)
{
    gpio_init(Pin);
    if (Mode == 0)
    {
        gpio_set_dir(Pin, GPIO_IN);
    }
    else
    {
        gpio_set_dir(Pin, GPIO_OUT);
    }
}

struct Point { int x; int y; };

static uint16_t buffer[240*240];

void drawPoly(Point* points, size_t point_count)
{
    int nodes, nodeX[point_count];

    //  Loop through the rows of the image.
    for (int pixelY=0; pixelY<240; pixelY++) {
        //  Build a list of nodes.
        nodes=0;
        int j = point_count-1;
        for (size_t i=0; i<point_count; i++) {
            if ((points[i].y < pixelY && points[j].y >= pixelY) || (points[j].y < pixelY && points[i].y >= pixelY)) {
                nodeX[nodes++] = points[i].x+(pixelY-points[i].y)*(points[j].x-points[i].x)/(points[j].y-points[i].y);
            }
            j=i;
        }

        //  Sort the nodes, via a simple “Bubble” sort.
        for(int i=0; i<nodes-1; ) {
            if (nodeX[i]>nodeX[i+1]) {
                std::swap(nodeX[i], nodeX[i+1]);
                if (i) i--;
            } else {
                i++;
            }
        }

        //  Fill the pixels between node pairs.
        for(int i=0; i<nodes; i+=2) {
            if (nodeX[i]>=240) break;
            if (nodeX[i+1]> 0 ) {
                if (nodeX[i]< 0 ) nodeX[i  ]=0;
                if (nodeX[i+1]> 240) nodeX[i+1]=240;
                for (int pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX++)
                    buffer[pixelX + pixelY*240] = 0xFFFF;
            }
        }
    }
}

int main(int argc, char** argv)
{
    stdio_init_all();
    for(int n=0; n<1000 && !stdio_usb_connected(); n++) {
        sleep_ms(1);
    }

    auto slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
    pwm_set_clkdiv(slice_num, 50);
    pwm_set_enabled(slice_num, true);

    pwm_set_chan_level(slice_num, PWM_CHAN_B, 10);

    DEV_GPIO_Mode(LCD_RST_PIN, 1);
    DEV_GPIO_Mode(LCD_DC_PIN, 1);
    DEV_GPIO_Mode(LCD_CS_PIN, 1);
    DEV_GPIO_Mode(LCD_BL_PIN, 1);

    DEV_Digital_Write(LCD_CS_PIN, 1);
    DEV_Digital_Write(LCD_DC_PIN, 0);
    DEV_Digital_Write(LCD_BL_PIN, 1);

    spi_init(LCD_SPI_PORT, 27000 * 1000);
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);

    i2c_init(SENSOR_I2C_PORT, 100 * 1000);
    gpio_set_function(SENSOR_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SENSOR_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SENSOR_SDA_PIN);
    gpio_pull_up(SENSOR_SCL_PIN);

    QMI8658_init();

    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(BLACK);
    for(int n=0; n<240*240; n++)
        buffer[n] = n;
    Point points[4] = {{50, 50}, {0, 120}, {120, 239}, {239, 120}};
    drawPoly(points, 4);
    LCD_1IN28_Display(buffer);
    return 0;
}