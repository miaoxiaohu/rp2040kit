#include "includes.h"


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_Init(void)
{
  TMP117_PortInit();
  TMP117_ContinuousMode();
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_PortInit(void)
{
  // ALERT
  gpio_init(TMP117_ALERT_PIN);
  gpio_pull_up(TMP117_ALERT_PIN);
  gpio_set_dir(TMP117_ALERT_PIN, GPIO_IN);
  // SCL
  gpio_init(TMP117_SCL_PIN);
  TMP117_SCL_H();
  gpio_pull_up(TMP117_SCL_PIN);
  gpio_set_dir(TMP117_SCL_PIN, GPIO_OUT);
  // SDA
  gpio_init(TMP117_SDA_PIN);
  gpio_pull_up(TMP117_SDA_PIN);
  gpio_put(TMP117_SDA_PIN, 0);
  gpio_set_dir(TMP117_SDA_PIN, GPIO_IN);
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_I2C_Start(void)
{
  TMP117_I2C_DELAY();
  TMP117_SDA_H();
  TMP117_SCL_H();
  TMP117_I2C_DELAY(); 
  TMP117_SDA_L();
  TMP117_I2C_DELAY();
  TMP117_I2C_DELAY();
  TMP117_SCL_L();
  TMP117_I2C_DELAY();
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_I2C_Stop(void) 
{
  TMP117_I2C_DELAY();
  TMP117_SDA_L();
  TMP117_SCL_H();
  TMP117_I2C_DELAY(); 
  TMP117_SDA_H();
  TMP117_I2C_DELAY();
  TMP117_I2C_DELAY();
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_I2C_Ack(void)
{
  TMP117_I2C_DELAY(); 
  TMP117_SDA_L();
  TMP117_I2C_DELAY(); 
  TMP117_SCL_H();
  TMP117_I2C_DELAY(); 
  TMP117_SCL_L();
  TMP117_I2C_DELAY();
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_I2C_NAck(void)
{
  TMP117_I2C_DELAY(); 
  TMP117_SDA_H();
  TMP117_I2C_DELAY();
  TMP117_SCL_H();
  TMP117_I2C_DELAY();
  TMP117_SCL_L();
  TMP117_I2C_DELAY(); 
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t TMP117_I2C_Check_Ack(void)
{ 
  uint8_t status; 

  TMP117_I2C_DELAY();
  TMP117_SDA_IN();
  TMP117_SCL_H();
  TMP117_I2C_DELAY();
  if(TMP117_SDA_DAT()==0)
  {
    status = TMP117_SUCCESS;
  }
  else
  {
    status = TMP117_FAIL; 
  }
  TMP117_SCL_L();
  TMP117_I2C_DELAY();
  
  return status; 
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_I2C_WriteByte(uint8_t dat)
{ 
  uint8_t i; 

  TMP117_SCL_L();
  for(i=0; i<8; i++) 
  {
    if(dat&0x80)
    {
      TMP117_SDA_H();
    }
    else
    {
      TMP117_SDA_L();
    }
    dat <<= 1;
    TMP117_I2C_DELAY();
    TMP117_SCL_H(); 
    TMP117_I2C_DELAY(); 
    TMP117_SCL_L();
  }
  TMP117_I2C_DELAY();
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t TMP117_I2C_ReadByte(void)
{ 
  uint8_t i; 
  uint8_t dat; 
  
  dat = 0;
  TMP117_I2C_DELAY();
  TMP117_SDA_IN();
  for(i=0; i<8; i++) 
  {
    TMP117_I2C_DELAY();
    TMP117_SCL_H();
    TMP117_I2C_DELAY();
    dat <<= 1;
    if(TMP117_SDA_DAT()!=0)
    {
      dat |= 0x01;
    }
    TMP117_SCL_L();
  }
  TMP117_I2C_DELAY();

  return dat; 
} 

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t TMP117_WriteBuffer(uint8_t reg_address, uint8_t *buf, uint8_t length)
{
  uint8_t i;
  
  TMP117_I2C_Start();
  TMP117_I2C_WriteByte(TMP117_ADDRESS);
  if(TMP117_I2C_Check_Ack()==TMP117_FAIL) 
  {
    TMP117_I2C_Stop();
    return TMP117_FAIL;
  }
  TMP117_I2C_WriteByte(reg_address);
  if(TMP117_I2C_Check_Ack()==TMP117_FAIL)
  {
    TMP117_I2C_Stop();
    return TMP117_FAIL;
  }
  for(i=0; i<length; i++)
  {
    TMP117_I2C_WriteByte(buf[i]);
    if((TMP117_I2C_Check_Ack()==TMP117_FAIL)&&(i!=(length-1)))
    {
      TMP117_I2C_Stop();
      return TMP117_FAIL;
    }
  }
  TMP117_I2C_Stop();

  return TMP117_SUCCESS;
} 

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t TMP117_ReadBuffer(uint8_t reg_address, uint8_t *buf, uint8_t length)
{ 
  uint8_t i;
  
  TMP117_I2C_Start();
  TMP117_I2C_WriteByte(TMP117_ADDRESS);
  if(TMP117_I2C_Check_Ack()==TMP117_FAIL) 
  {
    TMP117_I2C_Stop();
    return TMP117_FAIL;
  }
  TMP117_I2C_WriteByte(reg_address);
  if(TMP117_I2C_Check_Ack()==TMP117_FAIL) 
  {
    TMP117_I2C_Stop();
    return TMP117_FAIL;
  }
  TMP117_I2C_Start();
  TMP117_I2C_WriteByte(TMP117_ADDRESS|0x01);
  if(TMP117_I2C_Check_Ack()==TMP117_FAIL) 
  {
    TMP117_I2C_Stop();
    return TMP117_FAIL;
  }
  for(i=0; i<length; i++)
  {
    buf[i] = TMP117_I2C_ReadByte();
    if(i!=(length-1))
    {
      TMP117_I2C_Ack();
    }
    else
    {
      TMP117_I2C_NAck();
    }
  }
  TMP117_I2C_Stop();
  
  return TMP117_SUCCESS;
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_Shutdown(void)
{
	uint8_t regval8[2];
	
	regval8[0] = 0x06;
	regval8[1] = 0x20;
	TMP117_WriteBuffer(0x01, &regval8[0], 2);
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_OneShotConv(void)
{
  uint8_t regval8[2];
	
	regval8[0] = 0x0C;
	regval8[1] = 0x24;
	TMP117_WriteBuffer(0x01, &regval8[0], 2);
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void TMP117_ContinuousMode(void)
{
  uint8_t regval8[2];
	
	regval8[0] = 0x00;
	regval8[1] = 0x24;
	TMP117_WriteBuffer(0x01, &regval8[0], 2);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
float TMP117_ReadTempture(void)
{
  int16_t temp16;
	float tempture;
	uint8_t regval8[2];
  uint8_t retry_cnt;
  uint16_t delay_cnt;
  
  for(retry_cnt=0; retry_cnt<3; retry_cnt++)
  {
    delay_cnt = 0;
    while(TMP117_ALERT_DAT())
    {
      sleep_us(100);
      delay_cnt++;
      if(delay_cnt>5000)
      {
        break;
      }
    }
    if(delay_cnt<=5000)
    {
      break;
    }
  }
  if(retry_cnt==3)
  {
    return 0;
  }
	TMP117_ReadBuffer(0x00, &regval8[0], 2);             // 读取温度数据
  temp16 = regval8[0];
  temp16 <<= 8;
  temp16 |= regval8[1];
	tempture = temp16*0.0078125;
	
	return tempture;
}


