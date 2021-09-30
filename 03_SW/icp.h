#ifndef __ICP_H
#define __ICP_H



#define PEE_PAGE_SIZE   64
#define DEE_PAGE_SIZE   64
#define NVR_PAGE_SIZE   64

#define ICP_PDA_L           DUT_PDA_L
#define ICP_PDA_H           DUT_PDA_H
#define ICP_PCL_L           DUT_PCL_L
#define ICP_PCL_H           DUT_PCL_H

#define ICP_I2C_DELAY       DUT_I2C_DELAY
#define ICP_PWR_ON          DUT_PWR_ON
#define ICP_PWR_OFF         DUT_PWR_OFF
#define ICP_I2C_Start       DUT_I2C_Start
#define ICP_I2C_Stop        DUT_I2C_Stop
#define ICP_I2C_Ack         DUT_I2C_Ack
#define ICP_I2C_NAck        DUT_I2C_NAck
#define ICP_I2C_Check_Ack   DUT_I2C_Check_Ack
#define ICP_I2C_WriteByte   DUT_I2C_WriteByte
#define ICP_I2C_ReadByte    DUT_I2C_ReadByte
#define ICP_IIC_CLK         DUT_IIC_CLK
#define ICP_Polling         DUT_Polling
#define ICP_Polling_Middle  DUT_Polling_Middle
#define ICP_Polling_Long    DUT_Polling_Middle


#define ICP_SUCCESS         DUT_SUCCESS
#define ICP_FAIL            DUT_FAIL



uint8_t ICP_Enter(void);
uint8_t ICP_Exit(void);

uint8_t ICP_Write_PEE(uint16_t address, uint8_t *pbuffer, uint16_t length);
uint8_t ICP_Read_PEE(uint16_t address, uint8_t *pbuffer, uint16_t length);

uint8_t ICP_Write_DEE(uint16_t address, uint8_t *pbuffer, uint16_t length);
uint8_t ICP_Read_DEE(uint16_t byte_address, uint8_t *pbuffer, uint16_t length);

uint8_t ICP_Write_NVR(uint16_t address, uint8_t *pbuffer, uint16_t length);
uint8_t ICP_Read_NVR(uint16_t address, uint8_t *pbuffer, uint16_t length);

uint8_t ICP_CheckSRAM(uint8_t dat);
uint8_t ICP_CheckXRAM(uint8_t dat);



#endif  // __ICP_H
