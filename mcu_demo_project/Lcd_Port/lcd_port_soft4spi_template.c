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

#if(LCD_PORT == _LCD_PORT_DEMO)
#include "lcd_port_template.h"
#include "lcd_driver.h"
#include "stm32f10x.h"
//-----------------STM32F103 4SPI IO接口定义---------------- 

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
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOA, &GPIO_InitStruct);\
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
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
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
#define LCD_BL_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);\
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;\
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;\
		GPIO_Init(GPIOB, &GPIO_InitStruct);\
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
	
static void lcd_spi_send_1byte(uint8_t dat)
{
	#if((LCD_TYPE == LCD_OLED) || (LCD_TYPE == LCD_GRAY))
	//-----------------OLED类型屏幕-------------------
	//OLED SPI SCL默认常高,第二个跳变数据沿有效
	uint8_t i;
	for(i=0;i<8;i++)
	{			  
		LCD_SCL_Clr();
		if(dat&0x80)
		   LCD_SDA_Set();
		else 
		   LCD_SDA_Clr();
		dat<<=1; 
		LCD_SCL_Set();
	}		  
	#else //if(LCD_TYPE == LCD_RGB565)
	//-----------------TFT类型屏幕-------------------
	//TFT SPI SCL默认常低,第一个跳变数据沿有效
	uint8_t i;
	for(i=0;i<8;i++)
	{			  
		if(dat&0x80)
		   LCD_SDA_Set();
		else 
		   LCD_SDA_Clr();
		LCD_SCL_Set();
		dat<<=1; 
		LCD_SCL_Clr();
	}	
	#endif
}

/*--------------------------------------------------------------
  * 名称: lcd_port_init()
  * 功能: 屏幕接口初始化
  * 说明: SPI 背光IO等
----------------------------------------------------------------*/
void lcd_port_init(void)
{
	//stm32f103软件4spi例程
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();
	LCD_BL_IO_Init();
	//-----------------OLED类型屏幕-------------------
	#if((LCD_TYPE == LCD_OLED) || (LCD_TYPE == LCD_GRAY))
	//OELD SPI SCL默认常高,第二个跳变数据沿有效
	LCD_SCL_Set();
	#else //if(LCD_TYPE == LCD_RGB565)
	//-----------------TFT类型屏幕-------------------
	//TFT SPI SCL默认常低,第一个跳变数据沿有效
	LCD_SCL_Clr();
	#endif
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//动态刷新CRC校验用
	#endif
	LCD_RES_Clr();
	lcd_delay_ms(100);
	LCD_RES_Set();
	lcd_delay_ms(100);
	
	lcd_ic_init();
}

/*--------------------------------------------------------------
  * 名称: lcd_delay_ms(uint32_t ms)
  * 传入: ms时间
  * 功能: 软件延时
  * 说明: 无需精准
----------------------------------------------------------------*/
void lcd_delay_ms(volatile uint32_t ms)
{
		volatile uint16_t i;
		while (ms--)
		{
			i = 10000; //根据实际情况调整
			while (i--);
		}
}

/*--------------------------------------------------------------
  * 名称: lcd_bl_on()
  * 说明: 打开屏幕背光 weak类型 需要改写
----------------------------------------------------------------*/
void lcd_bl_on(void)
{
	LCD_BL_Set();
}

/*--------------------------------------------------------------
  * 名称: lcd_bl_off()
  * 说明: 关闭屏幕背光 weak类型 需要改写
----------------------------------------------------------------*/
void lcd_bl_off(void)
{
	LCD_BL_Clr();
}

/*--------------------------------------------------------------
  * 名称: lcd_is_busy()
  * 返回: 0屏幕接口空闲 1屏幕接口忙碌
  * 说明: 阻塞型驱动无需判断忙碌 DMA驱动需要判断
----------------------------------------------------------------*/
uint8_t lcd_is_busy(void)
{
	return 0;
}


/*--------------------------------------------------------------
  * 名称: lcd_send_1Cmd(uint8_t dat)
  * 传入1: dat待发送的命令
  * 功能: 向屏幕发送1个命令
----------------------------------------------------------------*/
void lcd_send_1Cmd(uint8_t dat)
{
	LCD_DC_Clr();
	LCD_CS_Clr();
	lcd_spi_send_1byte(dat);
	LCD_CS_Set();
}

/*--------------------------------------------------------------
  * 名称: lcd_send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p待发送的数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个命令
----------------------------------------------------------------*/
void lcd_send_nCmd(uint8_t *p,uint16_t num)
{
//	while(1)//需要移植 参考stm32f103例程
//	{
//		;
//	}
	
	//-----------------OLED类型屏幕-------------------
	#if((LCD_TYPE == LCD_OLED) || (LCD_TYPE == LCD_GRAY))
		uint16_t i=0;
		LCD_DC_Clr();
		LCD_CS_Clr();
		while(num>i)
		{
			lcd_spi_send_1byte(p[i++]);	  
		}
		LCD_CS_Set();
	#else //if(LCD_TYPE == LCD_RGB565)
	//-----------------TFT类型屏幕-------------------
		//数组[0]按照指令发送
		//余下位按照数据发送
		
		//1.开始传输第一个命令
		LCD_DC_Clr();
		num--;
		LCD_CS_Clr();
		lcd_spi_send_1byte(p[0]);
	
		//2.发送剩下的按数据发送
		LCD_DC_Set();
		while(num-->0)
		{
			p++;
			lcd_spi_send_1byte(*p);
		}
		
		//3.结束
		LCD_CS_Set();
	#endif
	
}

/*--------------------------------------------------------------
  * 名称: lcd_send_1Dat(uint8_t dat)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送1个数据
----------------------------------------------------------------*/
void lcd_send_1Dat(uint8_t dat)
{
//	while(1)//需要移植 参考stm32f103例程
//	{
//		;
//	}
	
	LCD_DC_Set();
	LCD_CS_Clr();
	lcd_spi_send_1byte(dat);
	LCD_CS_Set();
	
}

/*--------------------------------------------------------------
  * 名称: lcd_send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个数据
----------------------------------------------------------------*/
void lcd_send_nDat(uint8_t *p,uint16_t num)
{
//	while(1)//需要移植 参考stm32f103例程
//	{
//		;
//	}
	
	uint16_t i=0;
	LCD_DC_Set();
	LCD_CS_Clr();
	while(num>i)
	{
		lcd_spi_send_1byte(p[i++]);	  
	}
	LCD_CS_Set();
}

/*--------------------------------------------------------------
  * 名称: void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: OLED屏幕从x,page位置开始刷屏
  * 说明: OLED屏幕移植接口例程
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_OLED)
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
//	while(1)//需要移植 参考stm32f103例程
//	{
//		;
//	}
	
	//--1.配置刷新窗口位置--
	lcd_set_addr(x0,page);
	//--2.快速发送点阵数据--
	lcd_send_nDat(page_gram,(x1-x0));
}
#endif

/*--------------------------------------------------------------
  * 名称: void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: 灰度OLED屏幕从x,page位置开始刷屏
  * 说明: 灰度OLED屏幕移植接口例程
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_GRAY)
void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}
#endif

/*--------------------------------------------------------------
  * 名称: void rgb565_flush(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: TFT_RGB565屏幕从x,page位置开始刷屏
  * 说明: RGB565屏幕移植接口
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_RGB565)
void lcd_rgb565_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
//	while(1)//需要移植 参考stm32f103例程
//	{
//		;
//	}

	uint8_t page_bit,*p,mask,c;
	uint16_t x,y0,y1,x_len;
	y0 = page * 8;
	y1 = y0 + 8;//y1 = SCREEN_HIGH-1;
	x_len = x1-x0;
	
	//--1.配置刷新窗口位置--
	lcd_set_addr(x0,y0,x1,y1);
	//----------------------
	
	//--2.准备发送屏幕数据--
	//LCD_DC_Set();
	//LCD_CS_Clr();
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
			lcd_send_1Dat(lcd_driver.colour[c]>>8);
			lcd_send_1Dat(lcd_driver.colour[c]);
			//----------------------	
		}
	}
	//------3.结束发送------
	//LCD_CS_Set();
	//----------------------
}
#endif

/*--------------------------------------------------------------
  * 名称: uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
  * 传入1:*gram待校验数组指针
	* 传入2:len待校验数组长度
	* 返回: crc校验值
  * 说明: weak类型 需要移植,否则无法使用动态刷新
----------------------------------------------------------------*/
#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//动态刷新相关
uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
{
//	while(1)//需要移植 参考下方stm32f103例程
//	{
//		;
//	}
	
	uint16_t i;
	CRC->CR = CRC_CR_RESET;//CRC_ResetDR();//清空CRC计算值
	for(i=0;i<=len;i++)
	{
		CRC->DR = *gram++;//CRC_CalcCRC(*gram++);//计算校验
	}
	return CRC->DR;//return CRC_GetCRC();
}
#endif

#endif //#if(LCD_PORT == _LCD_PORT_DEMO)
