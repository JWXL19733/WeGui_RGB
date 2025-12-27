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

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if(LCD_PORT == _HARD_4SPI)
#include "stm32f103_lcd_hard_4spi_port.h"
#include "lcd_driver.h"

#ifndef RAM_SPEEDUP_FUNC_0
#	define RAM_SPEEDUP_FUNC_0
#endif

/*--------------------------------------------------------------
  * 名称: LCD_delay_ms(uint32_t ms)
  * 传入1: ms
  * 返回: 无
  * 功能: 软件延时
  * 说明: 非精准
----------------------------------------------------------------*/
void LCD_delay_ms(volatile uint32_t ms)
{
		volatile uint16_t i;
		while (ms--) 
		{
			i = 10000; //根据实际情况调整
			while (i--);
		}
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令(DC=0时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Clr();
	LCD_CS_Clr();
	{
		send_lcd_spi_dat(dat);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}
/*--------------------------------------------------------------
  * 名称: LCD_Send_1Dat(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个数据(DC=1时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Dat(uint8_t dat)
{
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	LCD_DC_Set();
	LCD_CS_Clr();
	{
		send_lcd_spi_dat(dat);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个数据
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nDat(uint8_t *p,uint16_t num)
{
	uint16_t i=0;
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	LCD_DC_Set();
	LCD_CS_Clr();
	while(num>i)	  
	{
		wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
		send_lcd_spi_dat(p[i++]);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}

//-------------------------------------------OLED/灰度屏幕驱动接口----------------------------------------------
#if((LCD_TYPE == LCD_OLED) || (LCD_TYPE == LCD_GRAY))
/*--------------------------------------------------------------
  * 名称: LCD_Port_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 屏幕接口初始化
  * 说明: 
----------------------------------------------------------------*/
void LCD_Port_Init(void)
{
	//OELD SPI SCL默认常高,第二个跳变数据沿有效
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = LCD_SPI_BaudRatePrescaler_2;//分频(最快:SPI_BaudRatePrescaler_2)
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPIx, ENABLE);
	RCC_PCLK2Config(RCC_HCLK_Divx);//分频RCC_HCLK_Div1 RCC_HCLK_Div2 RCC_HCLK_Div4...
	
	SPI_Init(LCD_SPIx, &SPI_InitStructure);
	SPI_Cmd(LCD_SPIx, ENABLE);
	
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();LCD_CS_Set();
	LCD_BL_IO_Init();LCD_BL_On();
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//动态刷新CRC校验用
	#endif
	
	LCD_RES_Clr();
	{
		volatile unsigned int i=10000;
		while(i--);
	}
	LCD_RES_Set();
}


/*--------------------------------------------------------------
  * 名称: LCD_Send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nCmd(uint8_t *p,uint16_t num)
{
	uint16_t i=0;
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Clr();
	LCD_CS_Clr();
	while(num>i)	  
	{
		wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
		send_lcd_spi_dat(p[i++]);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}

//-------------------------------------------RGB屏幕驱动接口----------------------------------------------
#elif (LCD_TYPE == LCD_RGB565)
void LCD_Port_Init(void)
{
	//TFT SPI SCL默认常低,第一个跳变数据沿有效
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = LCD_SPI_BaudRatePrescaler_2;//分频(最快:SPI_BaudRatePrescaler_2)
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPIx, ENABLE);
	RCC_PCLK2Config(RCC_HCLK_Divx);//分频RCC_HCLK_Div1 RCC_HCLK_Div2 RCC_HCLK_Div4...
	
	SPI_Init(LCD_SPIx, &SPI_InitStructure);
	SPI_Cmd(LCD_SPIx, ENABLE);
	
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();LCD_CS_Set();
	LCD_BL_IO_Init();LCD_BL_On();
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//动态刷新CRC校验用
	#endif
	
	LCD_RES_Clr();
	LCD_delay_ms(100);
	LCD_RES_Set();
	LCD_delay_ms(100);
}

static void Set_SPI_DataSize_8b()
{
	//1.关闭SPI
	//SPI_Cmd(LCD_SPIx, DISABLE);//STM32F103无需关闭
	
	//2.调整SPI参数
	//SPI_DataSizeConfig(LCD_SPIx, SPI_DataSize_8b);
	LCD_SPIx->CR1 = LCD_SPIx->CR1&(uint16_t)~SPI_DataSize_16b;//寄存器操作
	
	//3.打开SPI
	//SPI_Cmd(LCD_SPIx, ENABLE);//STM32F103无需重新打开
}

static void Set_SPI_DataSize_16b()
{
	//1.关闭SPI
	//SPI_Cmd(LCD_SPIx, DISABLE);//STM32F103无需关闭
	
	//2.调整SPI参数
	//SPI_DataSizeConfig(LCD_SPIx, SPI_DataSize_8b);
	LCD_SPIx->CR1 = LCD_SPIx->CR1|SPI_DataSize_16b;//寄存器操作
	
	//3.打开SPI
	//SPI_Cmd(LCD_SPIx, ENABLE);//STM32F103无需重新打开
}

/*--------------------------------------------------------------
  * 名称: void LCD_Send_nCmd(uint16_t *p,uint16_t num)
  * 传入1: uint16_t *p:要发送的"1位命令+n个数据"的数组起始地址
  * 传入2: uint16_t num "命令+数据"的总数量(数组的长度)
  * 返回: 无
  * 功能: 向SPI发送指令
  * 说明: 阻塞运行
----------------------------------------------------------------*/
void LCD_Send_nCmd(uint8_t *p,uint16_t num)
{
	//数组[0]按照指令发送
	//余下位按照数据发送
		
		//3.设置SPI为8位模式
		Set_SPI_DataSize_8b();
			
		//4.开始传输第一个命令
		LCD_DC_Clr();
		LCD_CS_Clr();
		send_lcd_spi_dat(p[0]);//向SPI发送缓冲器发送一个数据

		//5.等待发送完成
		num--;
		wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
		send_lcd_spi_done();//等待SPI发送器空闲(发完)
	
		//6.发送剩下的数据
		LCD_DC_Set();
		while(num-->0)
		{
			p++;
			wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
			send_lcd_spi_dat(*p);//向SPI发送缓冲器发送一个数据
		}
		
		//7.等待发送完成
		wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
		send_lcd_spi_done();//等待SPI发送器空闲(发完)
		
		//8.结束
		LCD_CS_Set();	
}
#endif


#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
/*--------------------------------------------------------------
  * 名称: uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
  * 传入1:*gram待校验数组指针
	* 传入2:len待校验数组长度
	* 返回: crc校验值
  * 说明: 原函数为weak,改写自lcd_Driver.c
----------------------------------------------------------------*/
RAM_SPEEDUP_FUNC_0
uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
{
		uint16_t i;
		CRC->CR = CRC_CR_RESET;//CRC_ResetDR();//清空CRC计算值
		for(i=0;i<=len;i++)
		{
			CRC->DR = *gram++;//CRC_CalcCRC(*gram++);//计算校验
		}
		return CRC->DR;//return CRC_GetCRC();
}
#endif

#if (LCD_TYPE == LCD_OLED)
//----------------------------普通OLED屏幕刷屏接口-------------------------------------
/*--------------------------------------------------------------
  * 名称: void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: OLED屏幕从x,page位置开始刷屏
  * 说明: OLED屏幕移植接口例程 weak类型 需要改写
----------------------------------------------------------------*/
RAM_SPEEDUP_FUNC_0
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	//--1.配置刷新窗口位置--
	LCD_Set_Addr(x0,page);
	//--2.快速发送点阵数据--
	LCD_Send_nDat(page_gram,(x1-x0));
}

#elif (LCD_TYPE == LCD_GRAY)
//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
/*--------------------------------------------------------------
  * 名称: void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: 灰度OLED屏幕从x,page位置开始刷屏
  * 说明: 灰度OLED屏幕移植接口例程 weak类型 需要改写
----------------------------------------------------------------*/
RAM_SPEEDUP_FUNC_0
__attribute__((weak)) void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	//灰度屏待移植
	while(1)//需要移植
	{
		;
	}
}

#elif (LCD_TYPE == LCD_RGB565)
//----------------------------RGB565屏幕刷屏接口-------------------------------------
/*--------------------------------------------------------------
  * 名称: void rgb565_flush(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始坐标x
	* 传入2:x1刷新起始坐标x
  * 传入3:page刷新页
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: 从x,page位置开始刷屏 点阵数据转换成tft数据发送 
  * 说明: 源代码为weak类型 改自lcd_driver.c
----------------------------------------------------------------*/
RAM_SPEEDUP_FUNC_0 //代码加速宏定义
void lcd_rgb565_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	uint8_t page_bit,*p,mask,c;
	uint16_t x,y0,y1,x_len;
	y0 = page * 8;
	y1 = y0 + 8;//y1 = SCREEN_HIGH-1;
	x_len = x1-x0;
	
	//--1.配置刷新窗口位置--
	LCD_Set_Addr(x0,y0,x1,y1);
	//----------------------
	
	//--2.准备发送屏幕数据--
	LCD_DC_Set();
	LCD_CS_Clr();
	Set_SPI_DataSize_16b();
	//----------------------
	
	for(page_bit=0;page_bit<8;)
	{
		mask = 0x01<<page_bit++;
		p = page_gram;
		if(++y0 > SCREEN_HIGH){break;}
		for(x=0;x<=x_len;x++)
		{
			#if (LCD_COLOUR_BIT==1)//---1位色---
				c=0;if(*p++&mask){c=1;}
			#elif (LCD_COLOUR_BIT==2)//---2位色---
				c=0;if(*p++&mask){c=1;}if(*p++&mask){c+=2;}
			#elif (LCD_COLOUR_BIT==3)//---3位色---
				c=0;if(*p++&mask){c=1;}if(*p++&mask){c+=2;}if(*p++&mask){c+=4;}
			#endif
				
			//--2.快速发送一个颜色--
			wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
			send_lcd_spi_dat(lcd_driver.colour[c]);//发送发送一个颜色
			//----------------------	
		}
	}
	//------3.结束发送------
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空 (某些芯片可以省略掉这一句)
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
	Set_SPI_DataSize_8b();
	//----------------------
}
#endif

#endif
