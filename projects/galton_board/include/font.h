#ifndef _inc_font
#define _inc_font

  #include <pico/stdlib.h>
  #include <stdio.h>
  #include <stdlib.h>

  /*
  * Format
  * <height>, <width>, <additional spacing per char>, 
  * <first ascii char>, <last ascii char>,
  * <data>
  */
  extern const uint8_t font_8x5[];

#endif
