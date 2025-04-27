#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "ball.h"
#include <time.h>
#include "math.h"
#include "collision.h"

struct repeating_timer tick_timer;

bool timer_callback(struct repeating_timer *t)
{
    ssd1306_clear(&display);
    display_pins();
    display_bins();
    display_balls(DEFAULT_RADIUS);
    show();
    return true;
}

int main()
{
    stdio_init_all();
    srand(time(NULL));

    if (!init_display())
        return 1;

    populate_pins(5);
    populate_bins();
    spawn_ball();

    add_repeating_timer_ms(33, timer_callback, NULL, &tick_timer); // 1/30 -> 30 ticks per second

    while (true)
    {
        for (int i = 0; i < ball_next_idx; i++)
        {
            handle_descend(&balls[i]);
            detect_collision(&balls[i]);
        }
    }
    return 0;
}
