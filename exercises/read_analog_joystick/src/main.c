#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

#define JOYSTICK_X 27
#define JOYSTICK_Y 26

#define OLED_ADDR 0x3C
#define DEFAULT_SCALE 1

ssd1306_t display;

bool setup_i2c()
{
    if (!i2c_init(i2c1, 100 * 1000))
        return false;
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
    return true;
}

bool setup_ssd1306()
{
    display.width = 128;
    display.height = 64;
    display.external_vcc = false;
    if (!ssd1306_init(&display, display.width, display.height, OLED_ADDR, i2c1))
        return false;
    ssd1306_contrast(&display, 100);
    return true;
}

int main()
{
    stdio_init_all();
    if (!setup_i2c())
        return 1;
    if (!setup_ssd1306())
        return 1;

    adc_init();
    adc_gpio_init(JOYSTICK_Y);
    adc_gpio_init(JOYSTICK_X);

    adc_set_round_robin(0b0011); // set round robin on channels 1 and 0 (gpio 27 and 26, resp.)

    char jy[25] = "JY = 0 (0.0V)";
    char jx[25] = "JX = 0 (0.0V)";

    while (true)
    {
        uint16_t y = adc_read();
        uint16_t x = adc_read();

        float x_volt = x * 3.3f / (1 << 12);
        float y_volt = y * 3.3f / (1 << 12);
   
        snprintf(jx, sizeof(jx), "JX = %d (%3.2fV)", x, x_volt);
        snprintf(jy, sizeof(jy), "JY = %d (%3.2fV)", y, y_volt);

        ssd1306_clear(&display);
        ssd1306_draw_string(&display, 0, 20, DEFAULT_SCALE, (char *)jx);
        ssd1306_draw_string(&display, 0, 30, DEFAULT_SCALE, (char *)jy);
        ssd1306_show(&display);

        sleep_ms(20);
    }
}
