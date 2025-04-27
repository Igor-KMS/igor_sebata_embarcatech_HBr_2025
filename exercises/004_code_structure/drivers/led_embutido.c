#include "pico/cyw43_arch.h"
#include "led_embutido.h"

inline void driver_set_state(bool state)
{
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
}

inline int driver_init()
{
  return cyw43_arch_init();
}
