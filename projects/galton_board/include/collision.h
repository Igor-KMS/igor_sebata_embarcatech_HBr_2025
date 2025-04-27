#ifndef COLLISION_H
#define COLLISION_H

  #include "ball.h"
  
  void clamp_inclusive(int *, int, int);
  void handle_descend(obj *);
  void detect_collision(obj *);

#endif
