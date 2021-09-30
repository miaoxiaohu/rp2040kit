#include "includes.h"


#define FLASH_TARGET_OFFSET (256 * 1024)


void Flash_Read(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  const uint8_t *flash_address;
  uint16_t i;

  flash_address = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET + address);
  for(i=0; i<length; i++)
  {
    pbuffer[i] = flash_address[i];
  }
}


void Flash_Write(uint16_t address, uint8_t *pbuffer, uint16_t length)
{
  //const uint8_t buffer[FLASH_PAGE_SIZE] = "TEST";
  //flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
  //flash_range_program(FLASH_TARGET_OFFSET, buffer, FLASH_PAGE_SIZE);
}
