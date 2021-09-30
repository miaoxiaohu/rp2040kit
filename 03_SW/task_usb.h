#ifndef __TASK_USB_H
#define __TASK_USB_H


typedef struct
{
  uint8_t rx_buf[512];
  uint8_t tx_buf[512]; 
  uint16_t rx_length;
  uint16_t tx_length;
  uint8_t status;
}usb_t;


enum
{
  USB_IDLE=0,
  USB_BUSY,
  USB_RX_FINISH
};


void Task_USB(void);
void USB_RxData(void);
void Task_USB_Process(void);

#endif  // __TASK_USB_H
