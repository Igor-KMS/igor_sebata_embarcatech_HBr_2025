#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "ball.h"
#include <time.h>
#include "math.h"
#include "collision.h"

void detect_oob(obj *ball)
{
  if (ball->x > D_WIDTH - bins[0].y)
  {
    for (int i = 0; i < MAX_BIN_QTY; i++)
    {
      if (bins[i].x_l <= ball->y && ball->y <= bins[i].x_h)
      {
        // printf("%d ->> %d <= %d <= %d", i, bins[i].x_l, ball->y, bins[i].x_h);
        bins[i].count += 1;
        bins[i].visual_count += 1;
        break;
      }
    }
    ball->visible = false;
  };
}

void adjust_histogram()
{
  for (int i = 0; i < MAX_BIN_QTY; i++)
  {
    bins[i].visual_count = (unsigned int)ceil(bins[i].visual_count * 0.70);
  }
}

void update_histogram()
{
  for (int i = 0; i < MAX_BIN_QTY; i++)
  {
    if (bins[i].visual_count > 0)
      clamp_inclusive(&bins[i].visual_count, 0, bins[i].y);
    if (bins[i].visual_count >= bins[i].y)
      adjust_histogram();
    set_histogram_line(bins[i].x_l, bins[i].x_h, bins[i].visual_count);
  }
  show();
}
