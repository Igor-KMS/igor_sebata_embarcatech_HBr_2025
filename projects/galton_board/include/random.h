#ifndef RANDOM_H
#define RANDOM_H

  uint8_t get_random_bit(void);
  uint8_t get_random_byte(void);
  uint8_t get_rosc_bit(void);
  int8_t von_neumann_extractor(void);

  extern int rng;

#endif
