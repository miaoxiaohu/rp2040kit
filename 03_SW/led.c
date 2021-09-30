#include "includes.h"


void LED_Init(void)
{
  gpio_set_drive_strength(LED1_PIN, GPIO_DRIVE_STRENGTH_2MA);
  gpio_init(LED1_PIN);
  gpio_set_dir(LED1_PIN, GPIO_OUT);
  gpio_put(LED1_PIN, 1);

  gpio_set_drive_strength(LED2_PIN, GPIO_DRIVE_STRENGTH_2MA);
  gpio_init(LED2_PIN);
  gpio_set_dir(LED2_PIN, GPIO_OUT);
  gpio_put(LED2_PIN, 1);
}
