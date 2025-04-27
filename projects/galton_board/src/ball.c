#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "ball.h"
#include "display.h"
#include "ssd1306.h"
#include "math.h"
#include <time.h>

obj balls[MAX_BALL_QTY];
int ball_next_idx = 0;

obj pins[MAX_BALL_QTY];
int pin_next_idx = 0;

bin bins[7];

void spawn_ball(){
  // unsigned int spawn_point = 15 + (unsigned int)(rand() % (35 + 1));
  unsigned int spawn_point = D_HEIGHT/2; 
  
  if(ball_next_idx > MAX_BALL_QTY)
    return;
  
  obj new_ball;
  new_ball.y = spawn_point;
  new_ball.x = 10;
  new_ball.radius = DEFAULT_RADIUS;

  balls[ ball_next_idx++] = new_ball;
}

void populate_bins(){ // TODO calculate bin collision
  const float bin_qty = 7.f;
  int bin_width = round(D_HEIGHT/bin_qty);

  int control = 0;

  for(int i = 0; i < (sizeof(bins)/sizeof(bin)); i++){
    bins[i].x_l = control;
    control += bin_width;
    bins[i].x_h = control;
    bins[i].count = 0;
    bins[i].y = 35;
  }
}

void populate_pins(int row_qty)
{
  if (row_qty <= 0)
    return;
  if (row_qty > MAX_BALL_QTY)
    row_qty = MAX_BALL_QTY;

  const int width = D_HEIGHT;
  const int height = D_WIDTH;

  int start_y = 25;
  int horiz_spacing = width / (row_qty + 1);
  int vert_spacing = (height - start_y) / (row_qty + 3);
  int start_x = width / 2;

  for (int row = 0; row < row_qty; row++)
  {
    int pin_row = row + 4;
    
    float row_offset = horiz_spacing / 2.0f * (pin_row - 1);
    
    for (int col = 0; col < pin_row; col++)
    {
      int x = start_x - row_offset + col * horiz_spacing;
      int y = start_y + row * vert_spacing;

      if((0 <= x <= D_HEIGHT) && (0 <= y <= D_WIDTH)){
        pins[pin_next_idx].x = y;
        pins[pin_next_idx].y = x;
  
        pin_next_idx++;
      }
    }
  }
}
