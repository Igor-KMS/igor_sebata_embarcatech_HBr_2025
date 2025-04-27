#include "pico/stdlib.h"
#include "hal_led.h"

int main()
{
    stdio_init_all();

    if (hal_led_init())
        return -1;

    while (true)
    {
        hal_led_blink_delay(500);
    }
}
