#include "unity.h"
#include "src/p_adc.h"

void setUp()
{
}

void tearDown()
{
}

void test_adc_706V()
{
  uint32_t adc = 876;
  float expected_temp = 27.0f;
  float margin = 0.5f;

  float actual = adc_to_celsius(adc);

  TEST_ASSERT_FLOAT_WITHIN(margin, expected_temp, actual);
}

int main()
{
  stdio_init_all();
  printf("starting");
  sleep_ms(1000);

  UNITY_BEGIN();
  RUN_TEST(test_adc_706V);
  sleep_ms(5000);
  return UNITY_END();
}
