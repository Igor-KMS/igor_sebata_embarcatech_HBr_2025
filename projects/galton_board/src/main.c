#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "ball.h"
#include "collision.h"
#include "gui.h"
#include "histogram.h"
#include "hardware/adc.h"
#include "random.h"

struct repeating_timer tick_timer;
struct repeating_timer display_timer;
unsigned int max_ball_spawn = 0;

volatile int end_flag = 0;

void end_sequence()
{
    ssd1306_clear(&display);

    draw_text_sideways(55, 5, "BIN");
    draw_text_sideways(20, 5, "QTY");

    for (int i = 0; i < MAX_BIN_QTY; i++)
    {
        char bin_str[4];
        snprintf(bin_str, sizeof(bin_str), "%d", i + 1);

        char count_str[6];
        snprintf(count_str, sizeof(count_str), "%d", bins[i].count);

        uint32_t row_y = 5 + (i + 1) * 9;

        draw_text_sideways(55, row_y, bin_str);
        draw_text_sideways(15, row_y, count_str);
    }

    show();
}

bool display_callback(struct repeating_timer *t)
{
    static int polled = 0;
    static int qtd = 0;
    if (++polled > 2 && qtd < max_ball_spawn)
    {
        spawn_ball();
        qtd++;
        polled = 0;
    }
    if (qtd >= max_ball_spawn && ++polled > 80)
    {
        end_flag = 1;
        end_sequence();
        return false;
    }

    ssd1306_clear(&display);
    display_pins();
    display_bins();
    display_balls(DEFAULT_RADIUS);
    display_ball_qty(qtd);
    update_histogram();
    show();
    return true;
}

bool timer_callback(struct repeating_timer *t)
{
    for (int i = 0; i < ball_next_idx; i++)
    {
        if (balls[i].visible)
        {
            handle_descend(&balls[i]);
            detect_collision(&balls[i]);
            detect_oob(&balls[i]);
        }
    }
    if (end_flag)
        return false;
    return true;
}

void init_joystick()
{
    adc_init();
    adc_gpio_init(26); // JY
    adc_gpio_init(27); // JX

    gpio_init(22); // JSW
    gpio_pull_up(22);
    gpio_set_dir(22, GPIO_IN);
}

int main()
{
    stdio_init_all();

    if (!init_display())
        return 1;

    init_joystick();

    while (1)
    {
        ssd1306_clear(&display);
        if (current_page_index + 1 > MAX_PAGES)
            break;

        draw_page(&pages[current_page_index]);
        show();

        sleep_ms(100);
        handle_input();
    }

    rng = pages[1].saved_index;
    bias = pages[2].saved_index;
    max_ball_spawn = 100 + (100 * pages[3].saved_index);

    populate_pins(6);
    populate_bins();

    add_repeating_timer_ms(20, timer_callback, NULL, &tick_timer);
    add_repeating_timer_ms(33, display_callback, NULL, &display_timer);

    while (true)
    {
        tight_loop_contents();
    }
    return 0;
}
