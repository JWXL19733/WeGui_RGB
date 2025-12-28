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

#if (LCD_PORT == _SOFT_IIC)
#include "stm32f103_lcd_soft_iic_port.h"
#include "lcd_driver.h"

//I2C延迟信号 设置

//速度最快(屏幕点不亮选择慢速)
//#define I2C_SCL_Rise_Delay() {}
//#define I2C_SCL_Fall_Delay() {}
//#define I2C_SDA_Rise_Delay() {}
//#define I2C_SDA_Fall_Delay() {}

//速度极快(屏幕点不亮选择慢速)
//#define I2C_SCL_Rise_Delay() {volatile uint8_t t=0;while(t--);}
//#define I2C_SCL_Fall_Delay() {volatile uint8_t t=0;while(t--);}
//#define I2C_SDA_Rise_Delay() {volatile uint8_t t=0;while(t--);}
//#define I2C_SDA_Fall_Delay() {volatile uint8_t t=0;while(t--);}

//速度较快
//#define I2C_SCL_Rise_Delay() {volatile uint8_t t=1;while(t--);}
//#define I2C_SCL_Fall_Delay() {volatile uint8_t t=1;while(t--);}
//#define I2C_SDA_Rise_Delay() {volatile uint8_t t=1;while(t--);}
//#define I2C_SDA_Fall_Delay() {volatile uint8_t t=1;while(t--);}

//速度稍快2
//#define I2C_SCL_Rise_Delay() {volatile uint8_t t=2;while(t--);}
//#define I2C_SCL_Fall_Delay() {volatile uint8_t t=2;while(t--);}
//#define I2C_SDA_Rise_Delay() {volatile uint8_t t=2;while(t--);}
//#define I2C_SDA_Fall_Delay() {volatile uint8_t t=2;while(t--);}

//速度适中
#define I2C_SCL_Rise_Delay() {volatile uint8_t t=5;while(t--);}
#define I2C_SCL_Fall_Delay() {volatile uint8_t t=5;while(t--);}
#define I2C_SDA_Rise_Delay() {volatile uint8_t t=5;while(t--);}
#define I2C_SDA_Fall_Delay() {volatile uint8_t t=5;while(t--);}

//速度最慢
//#define I2C_SCL_Rise_Delay() {volatile uint8_t t=10;while(t--);}
//#define I2C_SCL_Fall_Delay() {volatile uint8_t t=10;while(t--);}
//#define I2C_SDA_Rise_Delay() {volatile uint8_t t=10;while(t--);}
//#define I2C_SDA_Fall_Delay() {volatile uint8_t t=10;while(t--);}


//I2C起始信号
static void LCD_I2C_Start(void)
{
	LCD_SDA_Clr();I2C_SDA_Fall_Delay();
	LCD_SCL_Clr();
}

//I2C结束信号
static void LCD_I2C_Stop(void)
{
	//LCD_SCL_Clr();//初始化确保状态正确
	
	LCD_SDA_Clr();I2C_SDA_Fall_Delay();
	LCD_SCL_Set();I2C_SCL_Rise_Delay();
	LCD_SDA_Set();I2C_SDA_Rise_Delay();
}

//I2C等待信号响应
static void LCD_I2C_WaitAck(void)
{
	LCD_SDA_Set();I2C_SDA_Rise_Delay();
	LCD_SCL_Set();I2C_SCL_Rise_Delay();
	LCD_SCL_Clr();I2C_SCL_Fall_Delay();
}
//I2C发送1字节数据
static void I2C_send_1Byte(uint8_t dat)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//将dat的8位从最高位依次写入
		{
			__NOP();
			LCD_SCL_Clr();I2C_SCL_Fall_Delay();
			LCD_SDA_Set();I2C_SDA_Rise_Delay();
    }
		else
		{
			LCD_SCL_Clr();I2C_SCL_Fall_Delay();
			LCD_SDA_Clr();I2C_SDA_Fall_Delay();
    }
		
		dat<<=1;
		LCD_SCL_Set();I2C_SCL_Rise_Delay();
		
  }
	__NOP();__NOP();__NOP();
	LCD_SCL_Clr();I2C_SCL_Fall_Delay();
}



/*--------------------------------------------------------------
  * 名称: lcd_delay_ms(volatile uint32_t ms)
  * 传入1: ms
  * 返回: 无
  * 功能: 软件延时
  * 说明: 非精准
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
  * 说明: 打开屏幕背光 weak类型 改自lcd_port_template.c
----------------------------------------------------------------*/
void lcd_bl_on(void)
{
	//LCD_BL_Set();
} 

/*--------------------------------------------------------------
  * 名称: lcd_bl_off()
  * 说明: 关闭屏幕背光 weak类型 改自lcd_port_template.c
----------------------------------------------------------------*/
void lcd_bl_off(void)
{
	//LCD_BL_Clr();
}

/*--------------------------------------------------------------
  * 名称: lcd_is_busy()
  * 传入: 无
  * 返回: 0屏幕接口空闲 1屏幕接口忙碌
  * 说明: 纯软件I2C无需判断忙碌
----------------------------------------------------------------*/
inline uint8_t lcd_is_busy(void)
{
	return 0;
}

/*--------------------------------------------------------------
  * 名称: lcd_port_init()
  * 功能: 屏幕接口初始化
  * 说明: SPI背光IO等 weak类型 改自lcd_port_template.c
----------------------------------------------------------------*/
void lcd_port_init(void)
{
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	
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
  * 名称: lcd_send_1Cmd(uint8_t dat)
  * 传入: dat待发送的命令
  * 功能: 向屏幕发送1个命令
----------------------------------------------------------------*/
void lcd_send_1Cmd(uint8_t dat)
{
	LCD_I2C_Start();
	I2C_send_1Byte(OLED_IIC_7ADDR<<1);
	LCD_I2C_WaitAck();
  I2C_send_1Byte(0x00);
	LCD_I2C_WaitAck();
	I2C_send_1Byte(dat);
	LCD_I2C_WaitAck();
	LCD_I2C_Stop();
}

/*--------------------------------------------------------------
  * 名称: lcd_send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p待发送的数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个命令
----------------------------------------------------------------*/
void lcd_send_nCmd(uint8_t *p,uint16_t num)
{
	LCD_I2C_Start();
	I2C_send_1Byte(OLED_IIC_7ADDR<<1);
	LCD_I2C_WaitAck();
	I2C_send_1Byte(0x00);
	LCD_I2C_WaitAck();
	uint8_t i=0;
	while(num>i)
	{
		I2C_send_1Byte(p[i]);
		LCD_I2C_WaitAck();
		i++;
	}
	LCD_I2C_Stop();
}

/*--------------------------------------------------------------
  * 名称: lcd_send_1Dat(uint8_t dat)
  * 传入: dat待发的数据
  * 功能: 向屏幕发送1个数据
----------------------------------------------------------------*/
void lcd_send_1Dat(uint8_t dat)
{
	LCD_I2C_Start();
	I2C_send_1Byte(OLED_IIC_7ADDR<<1);
	LCD_I2C_WaitAck();
	I2C_send_1Byte(0x40);
	LCD_I2C_WaitAck();
	I2C_send_1Byte(dat);
	LCD_I2C_WaitAck();
	LCD_I2C_Stop();
}

/*--------------------------------------------------------------
  * 名称: lcd_send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 功能: 向屏幕发送num个数据
----------------------------------------------------------------*/
void lcd_send_nDat(uint8_t *p,uint16_t num)
{
	LCD_I2C_Start();
	I2C_send_1Byte(OLED_IIC_7ADDR<<1);
	LCD_I2C_WaitAck();
	I2C_send_1Byte(0x40);
	LCD_I2C_WaitAck();
	uint8_t i=0;
	while(num>i)
	{
		I2C_send_1Byte(p[i]);
		LCD_I2C_WaitAck();
		i++;
	}
	LCD_I2C_Stop();
}


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
#if (LCD_TYPE == LCD_OLED)
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	//--1.配置刷新窗口位置--
	lcd_set_addr(x0,page);
	//--2.快速发送点阵数据--
	lcd_send_nDat(page_gram,(x1-x0));
}
#endif


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
#if (LCD_TYPE == LCD_GRAY)
void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	//待移植
	while(1)//需要移植
	{
		;
	}
}
#error ("GRAY not support I2C driver yet!");
#endif

//----------------------------RGB565屏幕刷屏接口-------------------------------------
#if (LCD_TYPE == LCD_RGB565)
	//彩屏TFT屏不支持I2C方式驱动 请更改屏幕驱动IC型号
	#error ("TFT not support I2C driver yet!");
#endif

/*--------------------------------------------------------------
  * 名称: uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
  * 传入1:*gram待校验数组指针
	* 传入2:len待校验数组长度
	* 返回: crc校验值
  * 说明: weak类型 改自lcd_port_template.c
----------------------------------------------------------------*/
#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
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

#endif

