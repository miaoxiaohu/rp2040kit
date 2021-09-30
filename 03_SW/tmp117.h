#ifndef __TMP117_H
#define __TMP117_H



#define TMP117_ADDRESS      0x90

#define TMP117_ALERT_PIN    13
#define TMP117_ALERT_DAT()  gpio_get(TMP117_ALERT_PIN)

#define TMP117_SCL_PIN      14
#define TMP117_SCL_H()      gpio_put(TMP117_SCL_PIN, 1)
#define TMP117_SCL_L()      gpio_put(TMP117_SCL_PIN, 0)

#define TMP117_SDA_PIN      15
#define TMP117_SDA_H()      {gpio_pull_up(TMP117_SDA_PIN);gpio_set_dir(TMP117_SDA_PIN, GPIO_IN);}
#define TMP117_SDA_L()      {gpio_put(TMP117_SDA_PIN, 0); gpio_set_dir(TMP117_SDA_PIN, GPIO_OUT);}
#define TMP117_SDA_IN()     gpio_set_dir(TMP117_SDA_PIN, GPIO_IN)
#define TMP117_SDA_DAT()    gpio_get(TMP117_SDA_PIN)

enum
{
  TMP117_SUCCESS=0,
  TMP117_FAIL=1
};



#define TMP117_I2C_DELAY() sleep_us(10)


void TMP117_Init(void);
void TMP117_PortInit(void);
void TMP117_I2C_Start(void);
void TMP117_I2C_Stop(void) ;
void TMP117_I2C_Ack(void);
void TMP117_I2C_NAck(void);
uint8_t TMP117_I2C_Check_Ack(void);
void TMP117_I2C_WriteByte(uint8_t dat);
uint8_t TMP117_I2C_ReadByte(void);
uint8_t TMP117_WriteBuffer(uint8_t reg_address, uint8_t *buf, uint8_t length);
uint8_t TMP117_ReadBuffer(uint8_t reg_address, uint8_t *buf, uint8_t length);




void TMP117_Init(void);
void Tmp117_Alert_In(void);
void TMP117_Shutdown(void);
void TMP117_OneShotConv(void);
void TMP117_ContinuousMode(void);
float TMP117_ReadTempture(void);


#endif	//__TMP117_H


