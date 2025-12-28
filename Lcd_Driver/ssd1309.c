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

#if (LCD_IC == _SSD1309)
#include "ssd1309.h"

/*--------------------------------------------------------------
  * 名称: SSD1309_Set_Address_x(unsigned char x)
  * 传入: unsigned char x
  * 返回: 无
  * 功能: 向屏幕设置x方向的"光标"
----------------------------------------------------------------*/
void SSD1309_Set_Address_x(unsigned char x)
{
	lcd_send_1Cmd(0x10|(x>>4));//高3字节
	lcd_send_1Cmd(x&0x0f);//低4字节
}

/*--------------------------------------------------------------
  * 名称: SSD1309_Set_Address_ypage(unsigned char page)
  * 传入: page
  * 返回: 无
  * 功能: 向屏幕设置y方向的"页page"
  * 说明: 页1page = 8个像素点
----------------------------------------------------------------*/
void SSD1309_Set_Address_ypage(unsigned char page)
{
		lcd_send_1Cmd(0xb0+page);
}

/*--------------------------------------------------------------
  * 名称: SSD1309_Set_Address_x_ypage(unsigned char x,unsigned char page)
  * 传入: x,page坐标
  * 返回: 无
  * 功能: 向屏幕设置x和ypage坐标
  * 说明: 
----------------------------------------------------------------*/
void SSD1309_Set_Address_x_ypage(unsigned char x,unsigned char page)
{
	//方式1:单字节发送
	//SSD1309_Set_Address_x(x);
	//SSD1309_Set_Address_ypage(page);
	
	//方式2:连续发送
	uint8_t i[]={((x>>4) | 0x10),(x&0x0f),0xb0+page};
	lcd_send_nCmd(i,3);
}

/*--------------------------------------------------------------
  * 名称: SSD1309_Clear()
  * 传入: 无
  * 返回: 无
  * 功能: 清空IC内部图像寄存器
  * 说明: 
----------------------------------------------------------------*/
void SSD1309_Clear()
{
	unsigned char page,x;
	for(page=0;page<16;page++)
	{
		SSD1309_Set_Address_ypage(page);
		SSD1309_Set_Address_x(0);
		for(x=0;x<128;x++)
		{
			lcd_send_1Dat(0x00);
			//lcd_send_1Dat(0xff);//测试
		}
	}
}

/*--------------------------------------------------------------
  * 名称: SSD1309_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 初始化屏幕
  * 说明: 推荐更改为屏幕资料中的初始化指令
----------------------------------------------------------------*/
void SSD1309_Init(void)
{
	
	lcd_send_1Cmd(0xFD);
	lcd_send_1Cmd(0x12);
	lcd_send_1Cmd(0xAE);//--turn off oled panel
	lcd_send_1Cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	lcd_send_1Cmd(0xA0);
	lcd_send_1Cmd(0xA8);//--set multiplex ratio(1 to 64)
	lcd_send_1Cmd(0x3f);//--1/64 duty
	lcd_send_1Cmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	lcd_send_1Cmd(0x00);//-not offset
	lcd_send_1Cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	lcd_send_1Cmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	lcd_send_1Cmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	lcd_send_1Cmd(0xDA);//--set com pins hardware configuration
	lcd_send_1Cmd(0x12);
	lcd_send_1Cmd(0x81);//--set contrast control register
	lcd_send_1Cmd(0x7F);// Set SEG Output Current Brightness
	lcd_send_1Cmd(0xD9);//--set pre-charge period
	lcd_send_1Cmd(0x82);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	lcd_send_1Cmd(0xDB);//--set vcomh
	lcd_send_1Cmd(0x34);//Set VCOM Deselect Level
	lcd_send_1Cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	lcd_send_1Cmd(0xA6);// Disable Inverse Display On (0xa6/a7)
	lcd_send_1Cmd(0xAF);
}

#endif
