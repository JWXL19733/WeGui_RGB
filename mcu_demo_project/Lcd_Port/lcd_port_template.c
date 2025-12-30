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


//请勿包含使用main.h一股脑包含,容易造成循环包含和编译速度慢,按需添加头文件
//请勿包含使用main.h一股脑包含,容易造成循环包含和编译速度慢,按需添加头文件
//请勿包含使用main.h一股脑包含,容易造成循环包含和编译速度慢,按需添加头文件
//#include "gpio.h"
//#include "crc.h"
#include "stm32f10x.h"


/*--------------------------------------------------------------
  * 名称: lcd_port_init()
  * 功能: 屏幕接口初始化
  * 说明: SPI 背光IO等
----------------------------------------------------------------*/
void lcd_port_init(void)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
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
  * 说明: 打开屏幕背光
----------------------------------------------------------------*/
void lcd_bl_on(void)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: lcd_bl_off()
  * 说明: 关闭屏幕背光
----------------------------------------------------------------*/
void lcd_bl_off(void)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
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
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: lcd_send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p待发送的数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个命令
----------------------------------------------------------------*/
void lcd_send_nCmd(uint8_t *p,uint16_t num)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: lcd_send_1Dat(uint8_t dat)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送1个数据
----------------------------------------------------------------*/
void lcd_send_1Dat(uint8_t dat)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: lcd_send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个数据
----------------------------------------------------------------*/
void lcd_send_nDat(uint8_t *p,uint16_t num)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
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
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}

//	uint8_t page_bit,*p,mask,c;
//	uint16_t x,y0,y1,x_len;
//	y0 = page * 8;
//	y1 = y0 + 8;//y1 = SCREEN_HIGH-1;
//	x_len = x1-x0;
//	
//	//--1.配置刷新窗口位置--
//	lcd_set_addr(x0,y0,x1,y1);
//	//----------------------
//	
//	//--2.准备发送屏幕数据--
//	LCD_DC_Set();
//	LCD_CS_Clr();
//	//----------------------
//	
//	for(page_bit=0;page_bit<8;)
//	{
//		mask = 0x01<<page_bit++;
//		p = page_gram;
//		if(++y0 > SCREEN_HIGH){break;}
//		for(x=0;x<=x_len;x++)
//		{
//			#if (LCD_COLOUR_BIT==1)//---1位色---
//				c=0;if(*p++&mask){c=1;}
//			#elif (LCD_COLOUR_BIT==2)//---2位色---
//				c=0;if(*p++&mask){c=1;}if(*p++&mask){c+=2;}
//			#elif (LCD_COLOUR_BIT==3)//---3位色---
//				c=0;if(*p++&mask){c=1;}if(*p++&mask){c+=2;}if(*p++&mask){c+=4;}
//			#endif
//				
//			//--2.快速发送一个颜色--
//			lcd_spi_send_1byte(lcd_driver.colour[c]>>8);
//			lcd_spi_send_1byte(lcd_driver.colour[c]);
//			//----------------------	
//		}
//	}
//	//------3.结束发送------
//	LCD_CS_Set();
//	//----------------------
}
#endif

/*--------------------------------------------------------------
  * 名称: uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
  * 传入1:*gram待校验数组指针
	* 传入2:len待校验数组长度
	* 返回: crc校验值
  * 说明: 需要移植,否则无法使用动态刷新
----------------------------------------------------------------*/
#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//动态刷新相关
uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
{
	while(1)//需要移植 参考下方stm32f103例程
	{
		;
	}
	
//	uint16_t i;
//	CRC->CR = CRC_CR_RESET;//CRC_ResetDR();//清空CRC计算值
//	for(i=0;i<=len;i++)
//	{
//		CRC->DR = *gram++;//CRC_CalcCRC(*gram++);//计算校验
//	}
//	return CRC->DR;//return CRC_GetCRC();
}
#endif

#endif //#if(LCD_PORT == _LCD_PORT_DEMO)
