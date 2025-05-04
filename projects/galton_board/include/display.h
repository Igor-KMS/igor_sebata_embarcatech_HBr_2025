#ifndef DISPLAY_H
#define DISPLAY_H

  #include "ssd1306.h"
  #include "font.h"

  #define OLED_ADDR 0x3C
  #define DEFAULT_SCALE 1
  #define D_WIDTH 128
  #define D_HEIGHT 64

  extern ssd1306_t display;

  bool init_display(void);
  void display_pins(void);
  void display_bins(void);
  void display_balls(int);
  void display_ball_qty(int);
  void set_histogram_line(int, int, int);
  void show(void);
  void draw_text_sideways(int32_t, uint32_t, const char *);

#endif
