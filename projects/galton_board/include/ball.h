#ifndef BALL_H
#define BALL_H

  #define FALL_VELOCITY 0.5 // assume constant acceleration
  #define MAX_BALL_QTY 50
  #define DEFAULT_RADIUS 1
  #define MAX_BIN_QTY 7

  extern unsigned int max_ball_spawn;

  typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int radius;
    bool visible;
  } obj;

  typedef struct {
    unsigned int x_l;
    unsigned int x_h;
    unsigned int y;
    unsigned int count;
    unsigned int visual_count;
  } bin;

  extern obj balls[MAX_BALL_QTY];
  extern int ball_next_idx;
  
  extern obj pins[MAX_BALL_QTY];
  extern int pin_next_idx;
  
  extern bin bins[MAX_BIN_QTY];

  void populate_pins(int);
  void populate_bins(void);
  void spawn_ball(void);

#endif
