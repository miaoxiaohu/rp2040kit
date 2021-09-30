#ifndef __PROGRAM_H
#define __PROGRAM_H


void Flash_Read(uint16_t address, uint8_t *pbuffer, uint16_t length);
void Flash_Write(uint16_t address, uint8_t *pbuffer, uint16_t length);

#endif  //  __PROGRAM_H

