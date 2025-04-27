#include "led_embutido.h"
#include "hal_led.h"

inline void hal_led_turn_on()
{
  driver_set_state(true);
}

inline void hal_led_turn_off()
{
  driver_set_state(false);
}

inline void hal_led_toggle(bool on)
{
  on ? hal_led_turn_on() : hal_led_turn_off();
}

void hal_led_blink_delay(uint32_t ms)
{
  hal_led_toggle(true);
  sleep_ms(ms);
  hal_led_toggle(false);
  sleep_ms(ms);
}

inline int hal_led_init()
{
  return driver_init();
}
