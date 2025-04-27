#ifndef HAL_LED_H
#define HAL_LED_H

  #include "pico/stdlib.h"

  void hal_led_blink_delay(uint32_t);
  void hal_led_turn_off(void);
  void hal_led_turn_on(void);
  void hal_led_toggle(bool);
  int hal_led_init(void);

#endif
