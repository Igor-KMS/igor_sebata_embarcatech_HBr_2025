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

void ssd1306_draw_char_with_font_sideways(ssd1306_t *p, uint32_t y, uint32_t x, uint32_t scale, const uint8_t *font, char c)
{
  if (c < font[3] || c > font[4])
    return;

  uint32_t char_width = font[1];
  uint32_t char_height = font[0];
  uint32_t parts_per_line = (char_height >> 3) + ((char_height & 7) > 0);

  for (uint8_t w = 0; w < char_width; ++w)
  {
    for (uint32_t lp = 0; lp < parts_per_line; ++lp)
    {
      uint32_t pp = (c - font[3]) * char_width * parts_per_line + w * parts_per_line + 5 + lp;
      uint8_t line = font[pp];

      for (int8_t j = 0; j < 8; ++j, line >>= 1)
      {
        if (line & 1)
        {
          uint32_t px = x + ((lp << 3) + j) * scale;
          uint32_t py = y + (char_width - 1 - w) * scale;
          ssd1306_draw_square(p, px, py, scale, scale);
        }
      }
    }
  }
}

inline void show()
{
  ssd1306_show(&display);
}

void set_histogram_line(int x_l, int x_h, int count)
{
  for (int i = 1; i <= count; i++)
  {
    ssd1306_draw_line(&display, D_WIDTH - i, x_l, D_WIDTH - i, x_h);
  }
}

void display_bins()
{
  ssd1306_draw_line(&display, D_WIDTH - 35, 0, D_WIDTH, 0);
  ssd1306_draw_line(&display, D_WIDTH - 35, D_HEIGHT - 1, D_WIDTH, D_HEIGHT - 1);

  for (int i = 0; i < 6; i++)
  {
    ssd1306_draw_line(&display, D_WIDTH - bins[i].y, bins[i].x_h, D_WIDTH, bins[i].x_h);
  }
}

void draw_text_sideways(int32_t start_x, uint32_t y, const char *text)
{
  for (int32_t x_n = start_x; *text; x_n -= (font_8x5[1] + font_8x5[2]))
  {
    ssd1306_draw_char_with_font_sideways(&display, x_n, y, 1, font_8x5, *(text++));
  }
}

void display_ball_qty(int qty)
{
  char buffer[4];
  snprintf(buffer, sizeof(buffer), "%u", qty);
  char *text = buffer;

  draw_text_sideways(13, 1, text);
}

void display_balls(int radius)
{
  const int r_squared = radius * radius;

  for (int i = 0; i < ball_next_idx; i++)
  {
    if (balls[i].visible)
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
