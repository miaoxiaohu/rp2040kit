#include "includes.h"


int main(void)
{
  watchdog_enable(1000, 1);
  stdio_init_all();
  stdio_usb_init();

  LED_Init();
  DUT_Init();
  TMP117_Init();
  for(uint8_t i=0; i<5; i++)
  {
    sleep_ms(50);
    LED1_OFF();
    sleep_ms(50);
    LED1_ON();
  }

  while(1)
  {
    watchdog_update();
    Task_USB();
  }
}


