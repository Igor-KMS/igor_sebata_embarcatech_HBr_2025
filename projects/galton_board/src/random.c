#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"
#include "pico/rand.h"

volatile uint32_t *const ROSC_RANDOMBIT_REG = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

int rng = 0;

uint8_t get_rosc_bit(void)
{
  return *ROSC_RANDOMBIT_REG & 0x01;
}

int8_t von_neumann_extractor(void)
{
  uint8_t bit1 = get_rosc_bit();
  busy_wait_us(1);
  uint8_t bit2 = get_rosc_bit();

  if (bit1 == 0 && bit2 == 1)
    return 0;
  else if (bit1 == 1 && bit2 == 0)
    return 1;
  else
    return -1;
}

uint8_t get_random_bit(void)
{
  if (rng)
    return get_rand_32() & 0x01;
    
  int8_t result;
  do
  {
    result = von_neumann_extractor();
  } while (result == -1);

  return (uint8_t)result;
}

uint8_t get_random_byte(void)
{
  uint8_t byte = 0;
  for (int i = 0; i < 8; i++)
  {
    byte = (byte << 1) | get_random_bit();
  }
  return byte;
}
