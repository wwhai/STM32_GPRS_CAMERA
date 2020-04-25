#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "sys.h"
#include "stdint.h"

#define STM32_FLASH_SIZE 128
#define STM32_FLASH_WREN 1
#define STM32_FLASH_BASE 0x08000000

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);
void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len);
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr,u16 Len);
void STMFLASH_Write(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void STMFLASH_Read(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);

#endif
