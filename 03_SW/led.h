#ifndef __LED_H
#define __LED_H

#define LED1_PIN 6
#define LED2_PIN 7

#define LED1_ON()  gpio_put(LED1_PIN, 1)
#define LED1_OFF() gpio_put(LED1_PIN, 0)

#define LED2_ON()  gpio_put(LED2_PIN, 1)
#define LED2_OFF() gpio_put(LED2_PIN, 0)

void LED_Init(void);

#endif  // __LED_H
