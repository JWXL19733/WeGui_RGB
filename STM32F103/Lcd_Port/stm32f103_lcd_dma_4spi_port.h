/*
	Copyright 2025 Lu Zhihao

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef STM32F103_LCD_DMA_4SPI_PORT_H
#define STM32F103_LCD_DMA_4SPI_PORT_H

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stdint.h"


#define LCD_SPIx                   SPI1
#define LCD_DMAx                   DMA1
#define LCD_DMA_CHANNELx           DMA1_Channel3 //LCD_SPIx_TX
#define LCD_DMA_COMPLETE_FLAG      DMA1_FLAG_TC3
#define LCD_DMA_PeripheralBaseAddr (&SPI1->DR)
#define RCC_APB2Periph_SPIx        RCC_APB2Periph_SPI1

////---库函数版本---
//DMA忙碌
//#define LCD_is_Busy()              (DMA_GetFlagStatus(LCD_DMA_COMPLETE_FLAG) == (uint16_t)RESET)
//等待DMA空闲
//#define wait_lcd_dma_free()        do{while(LCD_is_Busy());}while(0)
////等待SPI发送缓冲器为空
//#define wait_lcd_spi_txtemp_free() do{while(SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_TXE)==RESET);}while(0)
////向SPI发送缓冲器发送一个数据
//#define send_lcd_spi_dat(dat)      do{SPI_I2S_SendData(LCD_SPIx, dat);}while(0)
////等待SPI发送器空闲(发完)
//#define send_lcd_spi_done()        do{while(SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_BSY));}while(0)

//---寄存器版本---
//DMA忙碌
#define LCD_is_Busy()              (lcd_busy!=0)
//等待DMA空闲
#define wait_lcd_dma_free()        do{while(LCD_is_Busy());}while(0)
//等待SPI发送缓冲器为空
#define wait_lcd_spi_txtemp_free() do{while((LCD_SPIx->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);}while(0)
//向SPI发送缓冲器发送一个数据
#define send_lcd_spi_dat(dat)      do{LCD_SPIx->DR = dat;}while(0)
//等待SPI发送器空闲(发完)
#define send_lcd_spi_done()        do{while((LCD_SPIx->SR & SPI_I2S_FLAG_BSY) != (uint16_t)RESET);}while(0)


//-----------------IO接口定义---------------- 

//-----SCL-----
//#define LCD_SCL_Clr() do{GPIO_ResetBits(GPIOA,GPIO_Pin_5);}while(0)//库函数操作IO
//#define LCD_SCL_Set() do{GPIO_SetBits(GPIOA,GPIO_Pin_5);}while(0)//库函数操作IO
#define LCD_SCL_Clr() do{GPIOA->BRR = GPIO_Pin_5;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_Set() do{GPIOA->BSRR = GPIO_Pin_5;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;\
		GPIO_Init(GPIOA, &GPIO_InitStruct);\
		LCD_SCL_Set();\
	}while(0)

//-----SDA-----
//#define LCD_SDA_Clr() do{GPIO_ResetBits(GPIOA,GPIO_Pin_7);}while(0)//库函数操作IO
//#define LCD_SDA_Set() do{GPIO_SetBits(GPIOA,GPIO_Pin_7);}while(0)//库函数操作IO
#define LCD_SDA_Clr() do{GPIOA->BRR = GPIO_Pin_7;}while(0)//寄存器操作,节省函数调用时间
#define LCD_SDA_Set() do{GPIOA->BSRR = GPIO_Pin_7;}while(0)//寄存器操作,节省函数调用时间
#define LCD_SDA_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;\
		GPIO_Init(GPIOA, &GPIO_InitStruct);\
		LCD_SDA_Set();\
	}while(0)

	
//-----RES-----
//#define LCD_RES_Clr() do{GPIO_ResetBits(GPIOB,GPIO_Pin_10);}while(0)//库函数操作IO
//#define LCD_RES_Set() do{GPIO_SetBits(GPIOB,GPIO_Pin_10);}while(0)//库函数操作IO
#define LCD_RES_Clr() do{GPIOB->BRR = GPIO_Pin_10;}while(0)//寄存器操作,节省函数调用时间
#define LCD_RES_Set() do{GPIOB->BSRR = GPIO_Pin_10;}while(0)//寄存器操作,节省函数调用时间
#define LCD_RES_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOB, &GPIO_InitStruct);\
		LCD_RES_Set();\
	}while(0)

//-----DC-----
//#define LCD_DC_Clr()  do{GPIO_ResetBits(GPIOB,GPIO_Pin_11);}while(0)//库函数操作IO
//#define LCD_DC_Set()  do{GPIO_SetBits(GPIOB,GPIO_Pin_11);}while(0)//库函数操作IO
#define LCD_DC_Clr() do{GPIOB->BRR = GPIO_Pin_11;}while(0)//寄存器操作,节省函数调用时间
#define LCD_DC_Set() do{GPIOB->BSRR = GPIO_Pin_11;}while(0)//寄存器操作,节省函数调用时间
#define LCD_DC_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOB, &GPIO_InitStruct);\
		LCD_DC_Set();\
	}while(0)
	
//-----BL-----(可选/BL背光一般RGB TFT屏幕专有)
#define LCD_BL_Clr() do{GPIOB->BRR = GPIO_Pin_0;}while(0)//寄存器操作,节省函数调用时间
#define LCD_BL_Set() do{GPIOB->BSRR = GPIO_Pin_0;}while(0)//寄存器操作,节省函数调用时间
//#define LCD_BL_On() do{LCD_BL_Clr();}while(0)
//#define LCD_BL_Off() do{LCD_BL_Set();}while(0)
#define LCD_BL_On() do{LCD_BL_Set();}while(0)
#define LCD_BL_Off() do{LCD_BL_Clr();}while(0)
#define LCD_BL_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOB, &GPIO_InitStruct);\
		LCD_BL_On();\
	}while(0)
	
//-----CS-----(可选)
//#define LCD_CS_Clr()  do{GPIO_ResetBits(GPIOA,GPIO_Pin_4);}while(0)//库函数操作IO
//#define LCD_CS_Set()  do{GPIO_SetBits(GPIOA,GPIO_Pin_4);}while(0)//库函数操作IO
#define LCD_CS_Clr() do{GPIOA->BRR = GPIO_Pin_4;}while(0)//寄存器操作,节省函数调用时间
#define LCD_CS_Set() do{GPIOA->BSRR = GPIO_Pin_4;}while(0)//寄存器操作,节省函数调用时间

#define LCD_CS_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOA, &GPIO_InitStruct);\
		LCD_CS_Set();\
	}while(0)
	
	
void LCD_Port_Init(void);//接口初始化	
void LCD_delay_ms(volatile uint32_t ms);
void LCD_Send_1Cmd(uint8_t dat);//向屏幕发送1个命令
void LCD_Send_1Dat(uint8_t dat);//向屏幕发送1个数据
//static void LCD_Send_nDat(uint8_t *p,uint16_t num);//向屏幕发送num个数据
void LCD_Send_nCmd(uint8_t *p,uint16_t num);//向屏幕发送num个命令
void LCD_DMA_SPIx_ISR(void);//需要移植到DMA完毕中断里

extern volatile uint8_t lcd_busy;

#endif
