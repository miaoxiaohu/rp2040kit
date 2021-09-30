#ifndef __DUT_H
#define __DUT_H


#define DUT_PCL_PIN	    36
#define DUT_PCL_H()     gpio_put(DUT_PCL_PIN, 1)
#define DUT_PCL_L()     gpio_put(DUT_PCL_PIN, 0)

#define DUT_PDA_PIN	    8
#define DUT_PDA_H()     gpio_set_dir(DUT_PDA_PIN, GPIO_IN)
#define DUT_PDA_L()     gpio_set_dir(DUT_PDA_PIN, GPIO_OUT)
#define DUT_PDA_IN()    gpio_set_dir(DUT_PDA_PIN, GPIO_IN)
#define DUT_PDA_DAT()   gpio_get(DUT_PDA_PIN)

#define DUT_PWR_PIN	    9
#define DUT_PWR_H()     gpio_put(DUT_PWR_PIN, 1)
#define DUT_PWR_L()     gpio_put(DUT_PWR_PIN, 0)

#define DUT_DET_PIN	    26
#define DUT_DET_DAT()   gpio_get(DUT_DET_PIN)
#define DUT_DET_H()     gpio_put(DUT_DET_PIN, 1)
#define DUT_DET_L()     gpio_put(DUT_DET_PIN, 0)


#define DUT_I2C_DELAY()  sleep_us(6)
#define DUT_DELAY_MS(x)  sleep_ms(x)


enum
{
  DUT_SUCCESS = 0,
  DUT_FAIL = 1
};


void DUT_Init(void);
void DUT_GPIO_Init(void);
void DUT_DET_OUT_L(void);
void DUT_DET_IN(void);
uint8_t DUT_PCL_DAT(void);
void DUT_PDA_Output(void);
void DUT_PDA_Input(void);
void DUT_Run_Prepare(void);
void DUT_Run_Finish(void);

void DUT_PWR_Delay(void);
void DUT_PWR_ON(void);
void DUT_PWR_OFF(void);
void DUT_I2C_Start(void);
void DUT_I2C_Stop(void);
void DUT_I2C_Ack(void);
void DUT_I2C_NAck(void);
uint8_t DUT_I2C_Check_Ack(void) ;
void DUT_I2C_WriteByte(uint8_t dat);
void DUT_I2C_ReadByte(uint8_t *dat) ;
void DUT_IIC_CLK(uint16_t clknum);
void DUT_Polling(void);
void DUT_Polling_Middle(void);
void DUT_Polling_Long(void);


#endif  // __DUT_H
