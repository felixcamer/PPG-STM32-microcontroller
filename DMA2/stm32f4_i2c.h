/* #include "stm32f4_i2c.h" */
#include "sys.h"  
#ifndef __STM32F4_I2C_H
#define __STM32F4_I2C_H
/*=====================================================================================================*/
/*=====================================================================================================*/
#define I2C_TIME		((u32)65535)
#define I2C1_SPEED	((u32)400000)
/*=====================================================================================================*/
/*=====================================================================================================*/
void I2C_Config( );
u32 I2C_DMA_Read( u8* ReadBuf, u8 SlaveAddr, u8 ReadAddr, u8* NumByte );
u32 I2C_DMA_ReadReg( u8* ReadBuf, u8 SlaveAddr, u8 ReadAddr, u8 NumByte );
u32 I2C_DMA_Write( u8* WriteBuf, u8 SlaveAddr, u8 WriteAddr, u8* NumByte );
u32 I2C_DMA_WriteReg( u8* WriteBuf, u8 SlaveAddr, u8 WriteAddr, u8 NumByte );
void I2C1_Send_DMA_IRQ( void );
void I2C1_Recv_DMA_IRQ( void );
u32 I2C_TimeOut( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
