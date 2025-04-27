#ifndef DRIVER_LED_H
#define DRIVER_LED_H

  #include "pico/stdlib.h"

  int driver_init(void);
  void driver_set_state(bool);

#endif
