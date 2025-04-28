#include "src/p_adc.h"

float adc_to_celsius(uint16_t adc_val)
{
    float volt = adc_val * (3.3f / (1 << 12));
    float temp = 27.f - (volt - 0.706f) / 0.001721f;
    return temp;
}
