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

#if (LCD_IC == _SSD1327)
#include "ssd1327.h"

/*--------------------------------------------------------------
  * 名称: SSD1327_Set_Address_x(unsigned char x)
  * 传入: unsigned char x
  * 返回: 无
  * 功能: 向屏幕设置x方向的"光标"
----------------------------------------------------------------*/
void SSD1327_Set_Addr_x(unsigned char x0,unsigned char x1)
{
	lcd_send_1Cmd(0x15);//CMD
	lcd_send_1Cmd(x0);//START
	lcd_send_1Cmd(x1);//END
	//SSD1327_Set_Column_Address(0,127);//15h start[0:127]默认0,end[0:127]默认127
}

/*--------------------------------------------------------------
  * 名称: SSD1327_Set_Address_y(unsigned char page)
  * 传入: page
  * 返回: 无
  * 功能:
  * 说明:
----------------------------------------------------------------*/
void SSD1327_Set_Addr_y(unsigned char y0,unsigned char y1)
{
	lcd_send_1Cmd(0x75);//CMD
	lcd_send_1Cmd(y0);//START
	lcd_send_1Cmd(y1);//END
}

void SSD1327_Set_Addr(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1)
{
	//方式1:单字节发送
	//SSD1327_Set_Addr_x(x0,x1);
	//SSD1327_Set_Addr_y(y0,y1);
	
	//方式2:连续发送(优化等级开高了可能有问题)
	uint8_t i[]={0x15,x0,x1,0x75,y0,y1};
	lcd_send_nCmd(i,6);
}

/*--------------------------------------------------------------
  * 名称: SSD1327_Clear()
  * 传入: 无
  * 返回: 无
  * 功能: 清空IC内部图像寄存器
  * 说明: 
----------------------------------------------------------------*/
void SSD1327_Clear()
{
	uint32_t i;
	SSD1327_Set_Addr(0,0,127,127);
	for(i=0;i<128*128/2;i++)
	{
			lcd_send_1Dat(0x00);
	}
}

/*--------------------------------------------------------------
  * 名称: SSD1327_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 初始化屏幕
  * 说明: 推荐更改为屏幕资料中的初始化指令
----------------------------------------------------------------*/
void SSD1327_Init(void)
{
	lcd_send_1Cmd(0xae);//Set display off
	
	//控制刷屏方向1
	#define A0_Bit0_SEG_REMAPPING      1
	#define A0_Bit1_NIBBLE_REMAPPING   0
	#define A0_Bit2_INC_MODE           1
	#define A0_Bit4_COM_REMAPPING      0
	#define A0_Bit6_SPLITTING_COM_MODE 1
	//控制刷屏方向2
	//#define A0_Bit0_SEG_REMAPPING      1 //SEG镜像
	//#define A0_Bit1_NIBBLE_REMAPPING   0 //根据屏幕制造商设置
	//#define A0_Bit2_INC_MODE           0 //90度旋转(连点不转,因此需要与驱动匹配)
	//#define A0_Bit4_COM_REMAPPING      1 //COM镜像
	//#define A0_Bit6_SPLITTING_COM_MODE 1 //根据屏幕制造商设置
	lcd_send_1Cmd(0xa0);//Set re-map
	lcd_send_1Cmd((A0_Bit6_SPLITTING_COM_MODE<<6)|(A0_Bit4_COM_REMAPPING<<4)|(A0_Bit2_INC_MODE<<2)|(A0_Bit1_NIBBLE_REMAPPING<<1)|A0_Bit0_SEG_REMAPPING);//
	
	lcd_send_1Cmd(0xa1);//Set display start line
	lcd_send_1Cmd(0x00);
	lcd_send_1Cmd(0xa2);//Set display offset
	lcd_send_1Cmd(0x00);
	lcd_send_1Cmd(0xa4);//Normal Display
	lcd_send_1Cmd(0xa8);//Set multiplex ratio
	lcd_send_1Cmd(0x7f);
	lcd_send_1Cmd(0xab);//Function Selection A
	lcd_send_1Cmd(0x01);//Enable internal VDD regulator
	lcd_send_1Cmd(0x81);//Set contrast
	lcd_send_1Cmd(0x77);
	lcd_send_1Cmd(0xb1);//Set Phase Length
	lcd_send_1Cmd(0x31);
	lcd_send_1Cmd(0xb3);//Set Front Clock Divider /Oscillator Frequency
	lcd_send_1Cmd(0xb1);
  lcd_send_1Cmd(0xb5);//
  lcd_send_1Cmd(0x03);//0X03 enable
	lcd_send_1Cmd(0xb6);//Set Second pre-charge Period
	lcd_send_1Cmd(0x0d);
	lcd_send_1Cmd(0xbc);//Set Pre-charge voltage
	lcd_send_1Cmd(0x07);
	lcd_send_1Cmd(0xbe);//Set VCOMH
	lcd_send_1Cmd(0x07);
	lcd_send_1Cmd(0xd5);//Function Selection B
	lcd_send_1Cmd(0x02);//Enable second pre-charge
	SSD1327_Clear();
	lcd_send_1Cmd(0xaf);//Display on
}

#endif
