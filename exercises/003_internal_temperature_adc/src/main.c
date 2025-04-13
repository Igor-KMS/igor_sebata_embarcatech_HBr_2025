#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

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
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while (true)
    {
        uint16_t measure = adc_read();
        double volt = measure * (3.3f / (1 << 12));
        double temp = 27 - (volt - 0.706)/0.001721;

        char string_temp[10] = "00.0 oC";
        snprintf(string_temp, sizeof(string_temp), "%3.1lf oC", temp);

        ssd1306_clear(&display);
        ssd1306_draw_string(&display, 25, 20, DEFAULT_SCALE, "Temperatura:");
        ssd1306_draw_string(&display, 42, 30, DEFAULT_SCALE, (char *)string_temp);
        ssd1306_show(&display);

        sleep_ms(300);
    }
}
