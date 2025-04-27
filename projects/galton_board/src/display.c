#include "display.h"
#include "ball.h"
#include "hardware/i2c.h"
#include <stdio.h>

ssd1306_t display;

bool setup_ssd1306()
{
  display.width = D_WIDTH;
  display.height = D_HEIGHT;
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

inline void show(){
  ssd1306_show(&display);
}

void display_bins()
{
  ssd1306_draw_line(&display, D_WIDTH - 35, 0, D_WIDTH, 0);
  ssd1306_draw_line(&display, D_WIDTH - 35, D_HEIGHT-1, D_WIDTH, D_HEIGHT-1);

  for (int i = 0; i < 6; i++)
  {
    ssd1306_draw_line(&display, D_WIDTH - bins[i].y, bins[i].x_h, D_WIDTH, bins[i].x_h);
  }
}

void display_balls(int radius)
{
  const int r_squared = radius * radius;

  for (int i = 0; i < ball_next_idx; i++)
  {
    for (int y = -radius; y <= radius; y++)
    {
      for (int x = -radius; x <= radius; x++)
      {
        if (x * x + y * y <= r_squared + 1)
        {
          ssd1306_draw_pixel(&display, balls[i].x + x, balls[i].y + y);
        }
      }
    }
  }
}

void display_pins()
{
  const int r_squared = DEFAULT_RADIUS * DEFAULT_RADIUS;

  for (int i = 0; i < pin_next_idx; i++)
  {
    for (int y = -DEFAULT_RADIUS; y <= DEFAULT_RADIUS; y++)
    {
      for (int x = -DEFAULT_RADIUS; x <= DEFAULT_RADIUS; x++)
      {
        if (x * x + y * y <= r_squared)
        {
          ssd1306_draw_pixel(&display, pins[i].x + x, pins[i].y + y);
        }
      }
    }
  }
}

bool init_display()
{
  if (!setup_i2c() || !setup_ssd1306())
  {
    printf("Failed to initialize display!");
    return false;
  }
  ssd1306_clear(&display);
  ssd1306_show(&display);
  return true;
}
