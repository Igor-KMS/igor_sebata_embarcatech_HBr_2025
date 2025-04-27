#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "math.h"
#include "display.h"
#include "collision.h"

int random_walk()
{
  unsigned int value = rand() % (100 + 1);
  if (value & 0x01)
    return -2;
  return 2;
}

void clamp_inclusive(int *value, int lower, int upper)
{
  if (*value > upper)
    *value = upper;
  else if (*value < lower)
    *value = lower;
}

void handle_collision(obj *ball)
{
  ball->x -= 2;
  ball->y += random_walk();
  clamp_inclusive(&ball->y, (ball->radius * 2), D_HEIGHT - (ball->radius * 2));
}

void detect_pin_collision(obj *ball)
{
  for (int p = 0; p < pin_next_idx; p++)
  {
    obj pin = pins[p];

    int delta_x = ball->x - pin.x;
    int delta_y = ball->y - pin.x;

    float dist = sqrtf(delta_x * delta_x - delta_y * delta_y);
    if ((int)dist < (ball->radius + pin.radius))
    {
      handle_collision(ball);
    }
  }
}

void detect_collision(obj *ball)
{
  detect_pin_collision(ball);
}

void handle_descend(obj *ball)
{
  ball->x += 2;
  clamp_inclusive(&ball->x, (ball->radius * 2), D_WIDTH - (ball->radius * 2));
}
