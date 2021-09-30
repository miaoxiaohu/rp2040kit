#include "includes.h"


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_Init(void)
{
  DUT_GPIO_Init();
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_Run_Prepare(void)
{
  gpio_set_dir(DUT_PCL_PIN, GPIO_IN);
  gpio_pull_up(DUT_PCL_PIN);
  
  gpio_set_dir(DUT_PDA_PIN, GPIO_IN);
  gpio_pull_up(DUT_PDA_PIN);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_Run_Finish(void)
{
  // PCL
  DUT_PCL_L();
  gpio_set_dir(DUT_PCL_PIN, GPIO_OUT);
  // PDA
  DUT_PDA_L();
  gpio_set_dir(DUT_PDA_PIN, GPIO_OUT);
}



/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_GPIO_Init(void)
{
  // PWR
  gpio_init(DUT_PWR_PIN);
  DUT_PWR_L();
  gpio_set_drive_strength(DUT_PWR_PIN, GPIO_DRIVE_STRENGTH_12MA);
  gpio_set_dir(DUT_PWR_PIN, GPIO_OUT);
  // DET
  gpio_init(DUT_DET_PIN);
  gpio_pull_down(DUT_DET_PIN);
  gpio_set_dir(DUT_DET_PIN, GPIO_IN);
  // PCL
  gpio_init(DUT_PCL_PIN);
  DUT_PCL_L();
  gpio_pull_up(DUT_PCL_PIN);
  gpio_set_dir(DUT_PCL_PIN, GPIO_OUT);
  // PDA
  gpio_init(DUT_PDA_PIN);
  gpio_pull_up(DUT_PDA_PIN);
  DUT_PDA_L();
  gpio_set_dir(DUT_PDA_PIN, GPIO_OUT);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_DET_OUT_L(void)
{
  // DET
  DUT_DET_L();
  gpio_set_dir(DUT_DET_PIN, GPIO_OUT);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_DET_IN(void)
{
  gpio_pull_down(DUT_DET_PIN);
  gpio_set_dir(DUT_DET_PIN, GPIO_IN);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t DUT_PCL_DAT(void)
{
  uint8_t bitstatus;
  
  if(gpio_get(DUT_PCL_PIN)!=0)
  {
    bitstatus = 1;
  }
  else
  {
    bitstatus = 0;
  }
  return bitstatus;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_PDA_Output(void)
{
  gpio_set_dir(DUT_PDA_PIN, GPIO_OUT);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_PDA_Input(void)
{
  gpio_set_dir(DUT_PDA_PIN, GPIO_IN);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_PWR_ON(void)
{
  DUT_PWR_H();
  while(DUT_DET_DAT()==0);
  DUT_DELAY_MS(20);
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_PWR_OFF(void)
{
  DUT_PWR_L();
  while(DUT_DET_DAT()!=0);
  DUT_DET_OUT_L();
  DUT_DELAY_MS(5);
  DUT_DET_IN();
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_I2C_Start(void) 
{
  DUT_PCL_L();
  DUT_PDA_H();
  DUT_I2C_DELAY();
  DUT_PCL_H();
  DUT_I2C_DELAY();
  DUT_PDA_L();
  DUT_I2C_DELAY(); 
  DUT_PCL_L();
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_I2C_Stop(void) 
{
  DUT_PCL_L();
  DUT_PDA_L();
  DUT_I2C_DELAY(); 
  DUT_PCL_H();
  DUT_I2C_DELAY();
  DUT_PDA_H();
  DUT_I2C_DELAY(); 
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_I2C_Ack(void)
{
  DUT_PCL_L();
  DUT_PDA_L();
  DUT_I2C_DELAY(); 
  DUT_PCL_H();
  DUT_I2C_DELAY();
  DUT_PCL_L();
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_I2C_NAck(void)
{
  DUT_PCL_L();
  DUT_PDA_H();
  DUT_I2C_DELAY();
  DUT_PCL_H();
  DUT_I2C_DELAY();
  DUT_PCL_L();
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t DUT_I2C_Check_Ack(void) 
{ 
  uint8_t status; 

  DUT_PDA_IN();
  DUT_PCL_L();
  DUT_I2C_DELAY();
  DUT_PCL_H();
  DUT_I2C_DELAY();
  if(DUT_PDA_DAT()==0)
  {
    status = DUT_SUCCESS;
  }
  else
  {
    status = DUT_FAIL; 
  }
  DUT_PCL_L();

  return status; 
} 


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_I2C_WriteByte(uint8_t dat) 
{ 
  uint8_t i; 
  
  DUT_PCL_L();
  for(i=0; i<8; i++) 
  {
    if(dat&0x80)
    {
      DUT_PDA_H();
    }
    else
    {
      DUT_PDA_L();
    }
    DUT_I2C_DELAY();
    DUT_PCL_H();
    DUT_I2C_DELAY();
    DUT_PCL_L();
    dat <<= 1;
  }

} 


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
void DUT_I2C_ReadByte(uint8_t *dat)
{ 
  uint8_t i; 

  DUT_PDA_IN();
  DUT_PCL_L();
  for(i=0; i<8; i++) 
  {
    *dat <<= 1;
    DUT_I2C_DELAY();
    *dat |= DUT_PDA_DAT();
    DUT_PCL_H();
    DUT_I2C_DELAY();
    DUT_PCL_L();
  }
} 



void DUT_Polling_Delay(void)
{
  sleep_us(2);
}

/*************************************************************
* @brief  
*         250kHz 7ms
* @param  
* @retval 
*************************************************************/
void DUT_Polling(void)
{
  uint16_t tm;
  
  for(tm=0; tm<2800; tm++)
  {
    DUT_Polling_Delay();
    DUT_PCL_H();
    DUT_Polling_Delay();
    DUT_PCL_L();
  }
}


/*************************************************************
* @brief  
*       250kHz 14ms
* @param  
* @retval 
*************************************************************/
void DUT_Polling_Middle(void)
{
  uint16_t tm;
  
  for(tm=0; tm<5600; tm++)
  {
    DUT_Polling_Delay();
    DUT_PCL_H();
    DUT_Polling_Delay();
    DUT_PCL_L();
  }
}


/*************************************************************
* @brief  
*         250kHz 28ms
* @param  
* @retval 
*************************************************************/
void DUT_Polling_Long(void)
{
  uint16_t tm;
  
  for(tm=0; tm<11200; tm++)
  {
    DUT_Polling_Delay();
    DUT_PCL_H();
    DUT_Polling_Delay();
    DUT_PCL_L();
  }
}





