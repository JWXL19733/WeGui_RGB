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

#if(LCD_PORT == _DMA_4SPI)
#include "stm32f103_lcd_dma_4spi_port.h"
#include "lcd_driver.h"

volatile uint8_t lcd_busy;

#if((LCD_TYPE == LCD_OLED) || (LCD_TYPE == LCD_GRAY))
//-------------------------------------------以下是OLED屏幕专用驱动接口----------------------------------------------
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
	
	DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)LCD_DMA_PeripheralBaseAddr;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)lcd_driver.LCD_GRAM;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_DeInit(DMA1_Channel6);
  DMA_Init(LCD_DMA_CHANNELx, &DMA_InitStructure);
	
	SPI_I2S_DMACmd(LCD_SPIx,SPI_I2S_DMAReq_Tx,ENABLE);
	
	SPI_Cmd(LCD_SPIx, ENABLE);
	
	DMA_ITConfig(LCD_DMA_CHANNELx, DMA_IT_TC, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//动态刷新CRC校验用
	#endif
	
	
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();
	
	LCD_RES_Clr();
	LCD_delay_ms(100);
	LCD_RES_Set();
	LCD_delay_ms(100);
	
	lcd_busy = 0;
}


/*--------------------------------------------------------------
  * 名称: LCD_delay_ms(volatile uint32_t ms)
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
	wait_lcd_dma_free();//等待dma空闲
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
	wait_lcd_dma_free();//等待dma空闲
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
	wait_lcd_dma_free();//等待dma空闲
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	LCD_DC_Set();
	LCD_CS_Clr();
	
	lcd_busy = 1;
	LCD_DMA_CHANNELx->CMAR = (uint32_t)p;
	LCD_DMA_CHANNELx->CNDTR = (uint32_t)num; 
	DMA_Cmd(LCD_DMA_CHANNELx, ENABLE);
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
	wait_lcd_dma_free();//等待dma空闲
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	uint8_t i=0;
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

//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if(LCD_TYPE == LCD_OLED)
/*--------------------------------------------------------------
  * 名称: void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: OLED屏幕从x,page位置开始刷屏
  * 说明: OLED屏幕移植接口例程 weak类型 需要改写
----------------------------------------------------------------*/
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	//--1.等待DMA和spi空闲--
	wait_lcd_dma_free();
	wait_lcd_spi_txtemp_free();
	//--2.配置刷新窗口位置--
	LCD_Set_Addr(x0,page);
	//--3.快速发送点阵数据--
	LCD_Send_nDat(page_gram,(x1-x0));
}

/*--------------------------------------------------------------
  * 名称: DMA1_Channel3_IRQHandler(void)
  * 功能: DMA中断接口
----------------------------------------------------------------*/
void DMA1_Channel3_IRQHandler()
{
		DMA_Cmd(LCD_DMA_CHANNELx, DISABLE);
		DMA_ClearFlag(LCD_DMA_COMPLETE_FLAG);
	
		//等待SPI发完(DMA完毕不代表SPI完毕)
		while((LCD_SPIx->SR & SPI_I2S_FLAG_BSY) != (uint16_t)RESET){;}
		LCD_CS_Set();
		lcd_busy = 0;
}

//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
#elif(LCD_TYPE == LCD_GRAY)
	//灰度OLED屏不支持dma4spi方式驱动 请更改屏幕驱动方式
	#error ("Gray OLED not support dma4spi driver yet!Use 4spi_driver please!")

//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif (LCD_TYPE == LCD_RGB565)
	//彩屏TFT屏不支持dma4spi方式驱动 请更改屏幕驱动方式
	#error ("TFT not support dma4spi driver yet!");
#else
	#error ("Not support LCD!");
#endif



#endif
