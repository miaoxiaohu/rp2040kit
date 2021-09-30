#include "includes.h"

/*************************************************************
* @version v2020_04_24
* @file    icp.c
* @author  PMICRO Application Team
* @brief   This file provides all the icp functions.
*************************************************************/


/*************************************************************
* @brief  ICP_Enter
*         DUT enter ICP mode
* @param  pdev: device instance
* @retval status: Always success
*************************************************************/
uint8_t ICP_Enter(void)
{
  ICP_I2C_Start();              // START
  ICP_PDA_H();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 1
  ICP_PCL_L();
  ICP_PDA_L();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 0
  ICP_PCL_L();
  ICP_PDA_H();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 1
  ICP_PCL_L();
  ICP_PDA_L();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 0
  ICP_PCL_L();
  ICP_PDA_L();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 0
  ICP_PCL_L();
  ICP_PDA_H();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 1
  ICP_PCL_L();
  ICP_PDA_H();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 1
  ICP_I2C_Stop();               // STOP
  ICP_PCL_L();
  ICP_PDA_H();
  ICP_I2C_DELAY();
  ICP_PCL_H();
  ICP_I2C_DELAY();              // 1
  ICP_PCL_L();
  ICP_I2C_DELAY();
  
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  ICP_Exit
*         DUT exit ICP mode
* @param  void
* @retval status: Always success
*************************************************************/
uint8_t ICP_Exit(void)
{
  ICP_PDA_L();
  ICP_PCL_L();
  
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  ICP_Write_PEE_Page
*         ICP write PEE page
* @param  address: page address
          pbuffer: data pointer to write
          length: data length to write
* @retval result: SUCEESS/ FAIL
*************************************************************/
uint8_t ICP_Write_PEE_Page(uint8_t address, uint8_t *pbuffer, uint16_t length)
{
  uint16_t i;

  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x30|0x00);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  for(i=0; i<length; i++)
  {
    ICP_I2C_WriteByte(*pbuffer++);
    if(ICP_I2C_Check_Ack()==ICP_FAIL)
    {
      ICP_I2C_Stop();
      return ICP_FAIL;
    }
  }
  // Dummy write
  ICP_I2C_WriteByte(0x55);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();

  return ICP_SUCCESS;	
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_PEE(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t i;
  uint8_t result;
  uint8_t page_address;
  uint8_t page_byte_front;
  uint8_t page_byte_left;
  uint8_t write_byte;
  uint8_t page_buffer[PEE_PAGE_SIZE];
  
  result = ICP_SUCCESS;
  while(length)
  {
    page_address = address/PEE_PAGE_SIZE;
    page_byte_front = address%PEE_PAGE_SIZE;
    page_byte_left = PEE_PAGE_SIZE-page_byte_front;
    if(length<page_byte_left)
    {
      write_byte = length;
    }
    else
    {
      write_byte = page_byte_left;
    }
    if(write_byte==PEE_PAGE_SIZE)
    {
      result |= ICP_Write_PEE_Page(page_address, pbuffer, PEE_PAGE_SIZE);
    }
    else
    {
      result |= ICP_Read_PEE(page_address*PEE_PAGE_SIZE, page_buffer, PEE_PAGE_SIZE);
      for(i=0; i<write_byte; i++)
      {
        page_buffer[page_byte_front+i] = pbuffer[i];
      }
      result |= ICP_Write_PEE_Page(page_address, page_buffer, PEE_PAGE_SIZE);
    }
    pbuffer += write_byte;
    length -= write_byte;
    address += write_byte;
  }

  return result;
}

/*************************************************************
* @brief  ICP_Read_PEE_Page
*         ICP Read PEE page
* @param  address: page address
          pbuffer: data buffer pointer to be readed
          length: data length to be readed
* @retval result: SUCEESS/ FAIL
*************************************************************/
uint8_t ICP_Read_PEE_Page(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
	uint8_t address_bit87, address_bit6, address_bit10, address_bit5432;

	address_bit87   = (uint8_t)((address&0x0180)>>6);
	address_bit6    = (uint8_t)((address&0x0040)>>6);
	address_bit5432 = (uint8_t)((address&0x003C)>>2);
	address_bit10   = (uint8_t)(address&0x0003);

	ICP_I2C_Start();
	ICP_I2C_WriteByte(0x30|address_bit87|address_bit6);
	if (ICP_I2C_Check_Ack()==ICP_FAIL)		
	{
		ICP_I2C_Stop();;
		return ICP_FAIL;
	}
	ICP_I2C_WriteByte(address_bit10);
	if(ICP_I2C_Check_Ack()==ICP_FAIL)
	{
		ICP_I2C_Stop();;
		return ICP_FAIL;
	}
  ICP_I2C_Start();
	ICP_I2C_WriteByte(0x50|address_bit5432);
	if(ICP_I2C_Check_Ack()==ICP_FAIL)
	{
		ICP_I2C_Stop();
		return ICP_FAIL;
	}
	while(length--)
	{
		ICP_I2C_ReadByte(pbuffer++);
		if(length==0)
    {
      ICP_I2C_NAck();
    }
		else
    {
      ICP_I2C_Ack();
    }
	}
	ICP_I2C_Stop();

	return ICP_SUCCESS;	
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Read_PEE(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t address_bit13;
  uint8_t address_bit12;
  uint8_t address_bit111098;
  uint8_t address_bit76543210;
  
  address_bit13       = (uint8_t)((address&0x2000)>>12);
  address_bit12       = (uint8_t)((address&0x1000)>>12);
  address_bit111098   = (uint8_t)((address&0x0F00)>>8);
  address_bit76543210 = (uint8_t)(address&0x00FF);
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x40|address_bit13|address_bit12);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address_bit76543210);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x50|address_bit111098);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_ReadByte(pbuffer++);
    if(length==0)
    {
      ICP_I2C_NAck();
    }
    else
    {
      ICP_I2C_Ack();
    }
  }
  ICP_I2C_Stop();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  ICP_Write_DEE_Page
*         ICP write DEE page
* @param  address: Page address to write
          pbuffer: Data buffer pointer
          length: Data buffer length
* @retval 
*************************************************************/
uint8_t ICP_Write_DEE_Page(uint8_t address, uint8_t *pbuffer, uint16_t length)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x30|0x08);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_WriteByte(*pbuffer++);
    if (ICP_I2C_Check_Ack()==ICP_FAIL)
    {
      ICP_I2C_Stop();
      return ICP_FAIL;
    }
  }
  // Dummy write
  ICP_I2C_WriteByte(0xFF);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();

  return ICP_SUCCESS;
}


/*************************************************************
* @brief  ICP_Write_DEE_Page
*         ICP write DEE page
* @param  page_address: Page address to read
          pbuffer: Data buffer pointer
          length: Data buffer length to read
* @retval 
*************************************************************/
uint8_t ICP_Read_DEE_Page(uint8_t page_address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t Page_bit87, Page_bit6,Page_bit10, Page_bit5432;
  
  Page_bit87   = 0x04;
  Page_bit6    = ((page_address&0x0040)>>6)&0x01;
	Page_bit5432 = ((page_address&0x003C)>>2)&0x0F;
	Page_bit10   = (page_address&0x0003)&0x03;

  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x30|Page_bit87|Page_bit6);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(Page_bit10);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x50|Page_bit5432);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_ReadByte(pbuffer++);
    if(length==0)
    {
      ICP_I2C_NAck();
    }
    else
    {
      ICP_I2C_Ack();
    }
  }
  ICP_I2C_Stop();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  ICP_Write_DEE_Byte
*         
* @param  byte_address: DEE byte address to write
          pbuffer: Data buffer pointer
          length: Data length to write
* @retval 
*************************************************************/
uint8_t ICP_Write_DEE_Byte(uint8_t address, uint8_t *pbuffer, uint16_t length)
{
  uint16_t i;

  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x40|0x08);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  for(i=0; i<length; i++)
  {
    ICP_I2C_WriteByte(*pbuffer++);
    if (ICP_I2C_Check_Ack()==ICP_FAIL)
    {
      ICP_I2C_Stop();
      return ICP_FAIL;
    }
  }
  // Dummy write
  ICP_I2C_WriteByte(0xFF);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();

  return ICP_SUCCESS;
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_DEE(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t i;
  uint8_t result;
  uint8_t page_address;
  uint8_t page_byte_front;
  uint8_t page_byte_left;
  uint8_t write_byte;
  uint8_t page_buffer[DEE_PAGE_SIZE];
  
  result = ICP_SUCCESS;
  while(length)
  {
    page_address = address/DEE_PAGE_SIZE;
    page_byte_front = address%DEE_PAGE_SIZE;
    page_byte_left = DEE_PAGE_SIZE-page_byte_front;
    if(length<page_byte_left)
    {
      write_byte = length;
    }
    else
    {
      write_byte = page_byte_left;
    }
    if(write_byte==DEE_PAGE_SIZE)
    {
      result |= ICP_Write_DEE_Page(page_address, pbuffer, DEE_PAGE_SIZE);
    }
    else
    {
      result |= ICP_Read_DEE(page_address*DEE_PAGE_SIZE, page_buffer, DEE_PAGE_SIZE);
      for(i=0; i<write_byte; i++)
      {
        page_buffer[page_byte_front+i] = pbuffer[i];
      }
      result |= ICP_Write_DEE_Page(page_address, page_buffer, DEE_PAGE_SIZE);
    }
    pbuffer += write_byte;
    length -= write_byte;
    address += write_byte;
  }

  return result;
}

/*************************************************************
* @brief  ICP_Read_DEE
*         
* @param  data_address: DEE byte address to read
          pbuffer: Data buffer pointer
          length: Data length to read
* @retval 
*************************************************************/
uint8_t ICP_Read_DEE(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t address_bit12;
  uint8_t address_bit111098;
  uint8_t address_bit76543210;
  
  address_bit12       = (uint8_t)((address&0x1000)>>12);
  address_bit111098   = (uint8_t)((address&0x0F00)>>8);
  address_bit76543210 = (uint8_t)(address&0x00FF);
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x40|0x04|address_bit12);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address_bit76543210);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x50|address_bit111098);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_ReadByte(pbuffer++);
    if(length==0)
    {
      ICP_I2C_NAck();
    }
    else
    {
      ICP_I2C_Ack();
    }
  }
  ICP_I2C_Stop();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_NVR_Page(uint8_t page_addr, uint8_t * pbuffer, uint16_t length)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x30|0x0C);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(page_addr);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_WriteByte(*pbuffer++);
    if (ICP_I2C_Check_Ack()==ICP_FAIL)
    {
      ICP_I2C_Stop();
      return ICP_FAIL;
    }
  }
  // Dummy write
  ICP_I2C_WriteByte(0xFF);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();

  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Read_NVR_Page(uint8_t page_address, uint8_t *pbuffer, uint16_t length)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x30|0x06);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(page_address);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x50);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_ReadByte(pbuffer++);
    if(length==0)
    {
      ICP_I2C_NAck();
    }
    else
    {
      ICP_I2C_Ack();
    }
  }
  ICP_I2C_Stop();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_NVR_Byte(uint8_t address,uint8_t *pbuffer,uint16_t length)
{
  uint16_t i;

  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x40|0x0C);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  for(i=0; i<length; i++)
  {
    ICP_I2C_WriteByte(*pbuffer++);
    if (ICP_I2C_Check_Ack()==ICP_FAIL)
    {
      ICP_I2C_Stop();
      return ICP_FAIL;
    }
  }
  // Dummy write
  ICP_I2C_WriteByte(0xFF);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();

  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_NVR(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t i;
  uint8_t result;
  uint8_t page_address;
  uint8_t page_byte_front;
  uint8_t page_byte_left;
  uint8_t write_byte;
  uint8_t page_buffer[NVR_PAGE_SIZE];
  
  result = ICP_SUCCESS;
  while(length)
  {
    page_address = address/NVR_PAGE_SIZE;
    page_byte_front = address%NVR_PAGE_SIZE;
    page_byte_left = NVR_PAGE_SIZE-page_byte_front;
    if(length<page_byte_left)
    {
      write_byte = length;
    }
    else
    {
      write_byte = page_byte_left;
    }
    if(write_byte==NVR_PAGE_SIZE)
    {
      result |= ICP_Write_DEE_Page(page_address, pbuffer, NVR_PAGE_SIZE);
    }
    else
    {
      result |= ICP_Read_DEE(page_address*NVR_PAGE_SIZE, page_buffer, NVR_PAGE_SIZE);
      for(i=0; i<write_byte; i++)
      {
        page_buffer[page_byte_front+i] = pbuffer[i];
      }
      result |= ICP_Write_DEE_Page(page_address, page_buffer, NVR_PAGE_SIZE);
    }
    pbuffer += write_byte;
    length -= write_byte;
    address += write_byte;
  }

  return result;
}

/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Read_NVR(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  uint8_t address_bit12;
  uint8_t address_bit111098;
  uint8_t address_bit76543210;
  
  address_bit12       = (uint8_t)((address&0x1000)>>12);
  address_bit111098   = (uint8_t)((address&0x0F00)>>8);
  address_bit76543210 = (uint8_t)(address&0x00FF);
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x40|0x06|address_bit12);
  if (ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(address_bit76543210);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x50|address_bit111098);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  while(length--)
  {
    ICP_I2C_ReadByte(pbuffer++);
    if(length==0)
    {
      ICP_I2C_NAck();
    }
    else
    {
      ICP_I2C_Ack();
    }
  }
  ICP_I2C_Stop();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_All_PMEM_330CLK(uint8_t Data)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x10);		                      //Refer to latest datasheet Rev:0.7
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }

  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  // Buffer write
  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_IIC_CLK(330);
  
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_All_PMEM(uint8_t Data)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x10);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  // Buffer write
  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }

  ICP_I2C_Stop();
  ICP_Polling();
  return ICP_SUCCESS;
}



/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_Even_PMEM(uint8_t Data)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x18);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  //buffer write
  ICP_I2C_WriteByte(0xFF);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }

  ICP_I2C_Stop();
  ICP_Polling();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_Odd_PMEM(uint8_t Data)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x14);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  // Buffer write
  ICP_I2C_WriteByte(0xFF);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_All_RAM(uint8_t Data)
{
  uint8_t i=255;
  
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x00);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(Data);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  while(--i)
  {
    ICP_Polling();
    ICP_I2C_Start();
    ICP_I2C_WriteByte(0xA0);
    if(ICP_I2C_Check_Ack())
    {
      ICP_I2C_Stop();
      return ICP_SUCCESS;
    }
    ICP_I2C_Stop();
  }
  return ICP_FAIL;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_Diagonal0_PMEM()
{
  uint8_t i=255;
  
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x20);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  while(--i)
  {
    ICP_Polling();
    ICP_I2C_Start();
    ICP_I2C_WriteByte(0xA0);
    if(ICP_I2C_Check_Ack())
    {
      ICP_I2C_Stop();
      return ICP_SUCCESS;
    }
    ICP_I2C_Stop();
  }
  return ICP_FAIL;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_Write_Diagonal1_PMEM()
{
  uint8_t i=255;
  
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x28);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  while(--i)
  {
    ICP_Polling();
    ICP_I2C_Start();
    ICP_I2C_WriteByte(0xA0);
    if(ICP_I2C_Check_Ack())
    {
      ICP_I2C_Stop();
      return ICP_SUCCESS;
    }
    ICP_I2C_Stop();
  }
  return ICP_FAIL;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_CheckSRAM(uint8_t dat)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x0F);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(dat);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling();

  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_CheckXRAM(uint8_t dat)
{
  ICP_I2C_Start();
  ICP_I2C_WriteByte(0x8F);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }

  ICP_I2C_WriteByte(dat);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_Stop();
  ICP_Polling_Long();
  
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_WriteEEoption(uint8_t addr, uint8_t *pbuffer, uint16_t length)
{
  length = length;
  ICP_I2C_Start();
  if(addr)
  {
    ICP_I2C_WriteByte(0x61);		    // 0x60 operate option 0;
  }
  else
  {
    ICP_I2C_WriteByte(0x60);        // 0x61 operate option 1;
  }
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(0xFF);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_WriteByte(*pbuffer);
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }	
  ICP_I2C_Stop();
  ICP_Polling();
  
  return ICP_SUCCESS;
}


/*************************************************************
* @brief  
*         
* @param  
* @retval 
*************************************************************/
uint8_t ICP_ReadEEoption(uint8_t addr,uint8_t *pbuffer,uint16_t length)
{
  length = length;
  ICP_I2C_Start();
  if(addr)
  {
    ICP_I2C_WriteByte(0x71);		    //0x70 operate option 0;
  }
  else
  {
    ICP_I2C_WriteByte(0x70);        //0x71 operate option 1;
  }
  if(ICP_I2C_Check_Ack()==ICP_FAIL)
  {
    ICP_I2C_Stop();
    return ICP_FAIL;
  }
  ICP_I2C_ReadByte(pbuffer++);
  ICP_I2C_Stop();

  return ICP_SUCCESS;
}



