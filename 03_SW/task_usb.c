#include "includes.h"


usb_t USB;

void Task_USB(void)
{
  USB_RxData();
  if(USB.status == USB_RX_FINISH)
  {
    Task_USB_Process();
    USB.status = USB_IDLE;
  }
}


void USB_RxData(void)
{
  uint8_t rx_data;
  int16_t rx_status;

  while(1)
  {
    rx_status = stdio_usb_in_chars(&rx_data, 1);
    if(rx_status>0)
    {
      if(USB.status==USB_IDLE)
      {
        USB.rx_length = 0;
        USB.status = USB_BUSY;
      }
      USB.rx_buf[USB.rx_length++] = rx_data;
    }
    else
    {
      if(USB.status == USB_BUSY)
      {
        USB.status = USB_RX_FINISH;
      }
      break;
    }
  }
}


void Task_USB_Process(void)
{
  uint16_t address;
  uint16_t length;
  float tempture;
  uint8_t string_buffer[20];

  if(USB.rx_buf[0]==0xAA)
  {
    switch(USB.rx_buf[1])
    {
      case 0x00:
        ICP_Exit();
        DUT_PWR_OFF();
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = 0x00;
        stdio_usb_out_chars(&USB.tx_buf[0], 2);
        break;
      case 0x01:
        ICP_Exit();
        DUT_PWR_OFF();
        DUT_PWR_ON();
        ICP_Enter();
        if(ICP_CheckSRAM(0xFF)==ICP_FAIL)
        {
          USB.tx_buf[0] = 0xAA;
          USB.tx_buf[1] = 0xE0;
          stdio_usb_out_chars(&USB.tx_buf[0], 2);
        }
        else
        {
          USB.tx_buf[0] = 0xAA;
          USB.tx_buf[1] = 0x00;
          stdio_usb_out_chars(&USB.tx_buf[0], 2);
        }
        break;
      case 0x10:                                                                  // Read PEE
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = ICP_Read_PEE(address, &USB.tx_buf[2], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2+length);
        break;
      case 0x11:                                                                  // Write PEE
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = ICP_Write_PEE(address, &USB.rx_buf[6], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2);
        break;
      case 0x20:
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = ICP_Read_DEE(address, &USB.tx_buf[2], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2+length);
        break;
      case 0x21:
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = ICP_Write_DEE(address, &USB.rx_buf[6], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2);
        break;
      case 0x30:
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = ICP_Read_NVR(address, &USB.tx_buf[2], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2+length);
        break;
      case 0x31:
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = ICP_Write_NVR(address, &USB.rx_buf[6], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2);
        break;
      case 0xA0:
        tempture = TMP117_ReadTempture();
        sprintf(string_buffer, "%f", tempture);
        stdio_usb_out_chars(&string_buffer[0], 10);
        break;
      case 0xB0:
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = 0x00;
        Flash_Read(address, &USB.tx_buf[2], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2+length);
        break;
      case 0xB1:
        address = USB.rx_buf[2];
        address <<= 8;
        address |= USB.rx_buf[3];
        length = USB.rx_buf[4];
        length <<= 8;
        length |= USB.rx_buf[5];
        USB.tx_buf[0] = 0xAA;
        USB.tx_buf[1] = 0x00;
        Flash_Write(address, &USB.rx_buf[6], length);
        stdio_usb_out_chars(&USB.tx_buf[0], 2);
        break;
    }
  }
}


