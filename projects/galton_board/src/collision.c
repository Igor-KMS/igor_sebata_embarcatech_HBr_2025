#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "math.h"
#include "display.h"
#include "collision.h"
#include "random.h"

int bias = 0;

int random_walk()
{
  switch (bias)
  {
    case 0:
      if (get_random_bit())
        return -5;
      return 5;
    case 1:
      if ((get_random_byte() % 101) < 25)
        return -5;
      return 5;
    case 2:
      if ((get_random_byte() % 101) < 75)
        return -5;
      return 5;
  }
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
  ball->x -= 3;
  ball->y += random_walk();
  clamp_inclusive(&ball->y, (ball->radius * 2), D_HEIGHT - (ball->radius * 2));
}

void detect_pin_collision(obj *ball)
{
  for (int p = 0; p < pin_next_idx; p++)
  {
    obj pin = pins[p];

    int delta_x = ball->x - pin.x;
    int delta_y = ball->y - pin.y;

    float dist = sqrtf(delta_x * delta_x + delta_y * delta_y);

    // if (dist < 10.f)
    // {
    //   unsigned int sum = ball->radius + pin.radius;
    //   printf("\n x: %d - %d = %d", ball->x, pin.x, delta_x);
    //   printf("\n y: %d - %d = %d", ball->y, pin.y, delta_y);
    //   printf("\n d: %f || %u", dist, sum);
    //   printf("\n r: %u || %u", ball->radius, pin.radius);
    //   printf("\n-------");
    // }

    if ((int)dist < (ball->radius + pin.radius + 1))
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
