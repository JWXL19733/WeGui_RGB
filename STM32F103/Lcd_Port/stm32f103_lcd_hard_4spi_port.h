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

#ifndef STM32F103_LCD_HARD_4SPI_PORT_H
#define STM32F103_LCD_HARD_4SPI_PORT_H

#include "stdint.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"

#ifndef RAM_SPEEDUP_FUNC_0
#	define RAM_SPEEDUP_FUNC_0
#endif

//-----------------硬件SPI接口定义---------------- 
#define LCD_SPIx                   SPI1
#define RCC_APB2Periph_SPIx        RCC_APB2Periph_SPI1

////---库函数版本---
////等待SPI发送缓冲器为空
//#define wait_lcd_spi_txtemp_free() do{while(SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_TXE)==RESET);}while(0)
////向SPI发送缓冲器发送一个数据
//#define send_lcd_spi_dat(dat)      do{SPI_I2S_SendData(LCD_SPIx, dat);}while(0)
////等待SPI发送器空闲(发完)
//#define send_lcd_spi_done()        do{while(SPI_I2S_GetFlagStatus(LCD_SPIx,SPI_I2S_FLAG_BSY));}while(0)

////---寄存器版本---
////等待SPI发送缓冲器为空
#define wait_lcd_spi_txtemp_free() do{while((LCD_SPIx->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET);}while(0)
//向SPI发送缓冲器发送一个数据
#define send_lcd_spi_dat(dat)      do{LCD_SPIx->DR = dat;}while(0)
//等待SPI发送器空闲(发完)
#define send_lcd_spi_done()        do{while((LCD_SPIx->SR & SPI_I2S_FLAG_BSY) != (uint16_t)RESET);}while(0)

//-----------------IO接口定义---------------- 
//-----SCL-----
#define LCD_SCL_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;\
		GPIO_Init(GPIOA, &GPIO_InitStruct);\
	}while(0)

//-----SDA-----
#define LCD_SDA_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;\
		GPIO_Init(GPIOA, &GPIO_InitStruct);\
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
#define LCD_BL_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOB, &GPIO_InitStruct);\
		lcd_bl_on();\
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

/*--------------------------------------------------------------
  * 名称: lcd_delay_ms(uint32_t ms)
  * 传入1: ms时间
  * 功能: 软件延时
  * 说明: 无需精准
----------------------------------------------------------------*/
void lcd_delay_ms(volatile uint32_t ms);
	
/*--------------------------------------------------------------
  * 名称: lcd_bl_on()
  * 说明: 打开屏幕背光
----------------------------------------------------------------*/
void lcd_bl_on(void);

/*--------------------------------------------------------------
  * 名称: lcd_bl_off()
  * 说明: 关闭屏幕背光
----------------------------------------------------------------*/
void lcd_bl_off(void);
	
/*--------------------------------------------------------------
  * 名称: uint8_t()
  * 传入: 无
  * 返回: 0屏幕接口空闲 1屏幕接口忙碌
  * 说明: DMA或SPI忙碌
----------------------------------------------------------------*/
uint8_t lcd_is_busy(void);

/*--------------------------------------------------------------
  * 名称: lcd_port_init()
  * 功能: 屏幕接口初始化
  * 说明: SPI 背光IO等
----------------------------------------------------------------*/
void lcd_port_init(void);

/*--------------------------------------------------------------
  * 名称: lcd_send_1Cmd(uint8_t dat)
  * 传入1: dat待发送的命令
  * 功能: 向屏幕发送1个命令
----------------------------------------------------------------*/
void lcd_send_1Cmd(uint8_t dat);

/*--------------------------------------------------------------
  * 名称: lcd_send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p待发送的数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个命令
----------------------------------------------------------------*/
void lcd_send_nCmd(uint8_t *p,uint16_t num);

/*--------------------------------------------------------------
  * 名称: lcd_send_1Dat(uint8_t dat)
  * 传入: dat待发的数据
  * 功能: 向屏幕发送1个数据
----------------------------------------------------------------*/
void lcd_send_1Dat(uint8_t dat);
	
/*--------------------------------------------------------------
  * 名称: lcd_send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个数据
----------------------------------------------------------------*/
void lcd_send_nDat(uint8_t *p,uint16_t num);
	
/*--------------------------------------------------------------
  * 名称: void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: OLED屏幕从x,page位置开始刷屏
  * 说明: OLED屏幕移植接口 默认weak类型 需要移植改写
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_OLED)
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram);
#endif

/*--------------------------------------------------------------
  * 名称: void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: 灰度OLED屏幕从x,page位置开始刷屏
  * 说明: 灰度OLED屏幕移植接口 默认weak类型 需要移植改写
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_GRAY)
void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram);
#endif

/*--------------------------------------------------------------
  * 名称: void rgb565_flush(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: TFT_RGB565屏幕从x,page位置开始刷屏
  * 说明: RGB565屏幕移植接口 默认weak类型 需要移植改写
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_RGB565)
void lcd_rgb565_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram);
#endif

/*--------------------------------------------------------------
  * 名称: uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
  * 传入1:*gram待校验数组指针
	* 传入2:len待校验数组长度
	* 返回: crc校验值
  * 说明: weak类型 需要移植,否则无法使用动态刷新
----------------------------------------------------------------*/
#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//动态刷新相关
uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len);
#endif

#endif
	
	
