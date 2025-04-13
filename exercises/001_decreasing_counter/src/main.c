#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/sync.h"

#define OLED_ADDR 0x3C
#define DEFAULT_SCALE 1

volatile int counter = 0, timer_count = 9;
volatile char countdown[13], btn_b[15];
volatile bool second_elapsed = false;

ssd1306_t display;
struct repeating_timer timer;

void update_screen()
{
  uint32_t interrupt_status = save_and_disable_interrupts();
  snprintf((char *)countdown, sizeof(countdown), "COUNTDOWN: %d", timer_count);
  snprintf((char *)btn_b, sizeof(btn_b), "BUTTON B: %d", counter);
  restore_interrupts(interrupt_status);

  ssd1306_clear(&display);
  ssd1306_draw_string(&display, 10, 20, DEFAULT_SCALE, (char *)countdown);
  ssd1306_draw_string(&display, 10, 30, DEFAULT_SCALE, (char *)btn_b);
  ssd1306_show(&display);
}

bool timer_callback(struct repeating_timer *t)
{
  second_elapsed = true;
  return true;
}

inline void toggle_irq(uint8_t gpio, bool on)
{
  gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL, on);
}

void debounce(uint gpio)
{
  toggle_irq(gpio, false);
  busy_wait_us_32(35000);
  if (!gpio_get(gpio))
  {
    uint32_t interrupt_status = save_and_disable_interrupts();
    switch (gpio)
    {
    case 5:
      cancel_repeating_timer(&timer);
      counter = 0;
      timer_count = 9;
      add_repeating_timer_ms(1000, timer_callback, NULL, &timer);
      break;
    case 6:
      counter++;
      break;
    }
    restore_interrupts(interrupt_status);
  }
  toggle_irq(gpio, true);
}

void gpio_irq_callback(uint gpio, uint32_t event_mask)
{
  debounce(gpio);
  return;
}

void setup_button(uint8_t BUTTON)
{
  gpio_init(BUTTON);
  gpio_set_dir(BUTTON, GPIO_IN);
  gpio_pull_up(BUTTON);
  gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, gpio_irq_callback);
  return;
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

int main()
{
  stdio_init_all();
  if (!setup_i2c())
    return 1;
  if (!setup_ssd1306())
    return 1;

  setup_button(5);
  setup_button(6);

  add_repeating_timer_ms(1000, timer_callback, NULL, &timer);

  while (true)
  {
    toggle_irq(6, true);
    while (timer_count >= 0)
    {
      update_screen();
      if (second_elapsed)
      {
        second_elapsed = false;

        uint32_t interrupt_status = save_and_disable_interrupts();
        timer_count--;
        restore_interrupts(interrupt_status);
      }
    }
    toggle_irq(6, false);
    __wfi();
  }
}
