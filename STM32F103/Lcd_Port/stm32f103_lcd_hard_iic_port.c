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

#if (LCD_PORT == _HARD_IIC) 
#include "stm32f103_lcd_hard_iic_port.h"
#include "lcd_driver.h"

/*--------------------------------------------------------------
  * IIC基础接口驱动
----------------------------------------------------------------*/
#define LCD_HARD_IIC_TIMEOUT_UINT8 255 //硬件IIC超时退出时间

//I2C起始信号
#define LCD_I2C_Start()  do                                    \
{	                                                              \
	Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;                        \
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))                 \
	{                                                             \
		if((Timeout--) == 0) break;                                 \
	}                                                             \
	I2C_GenerateSTART(I2C2, ENABLE);                              \
	Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;                        \
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))    \
	{                                                             \
		if((Timeout--) == 0) break;                                 \
	}                                                             \
}while(0)

//I2C结束信号
#define LCD_I2C_Stop() do                                         \
{                                                                  \
	Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;                  \
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  \
	{                                                                \
		if((Timeout--) == 0) break;                                    \
	}                                                                \
	I2C_GenerateSTOP(I2C2, ENABLE);                                  \
}while(0)



//I2C发送地址
#define LCD_I2C_SendAddr(addr) do                                          \
{                                                                           \
	Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;                                    \
	I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter);               \
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))  \
	{                                                                         \
		if((Timeout--) == 0) break;                                             \
	}                                                                         \
}while(0)


//I2C发送1字节数据
#define LCD_I2C_send_1Byte(dat)  do                                       \
{                                                                          \
	Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;                                   \
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) \
	{                                                                        \
		if((Timeout--) == 0) break;                                            \
	}                                                                        \
	I2C_SendData(I2C2, dat);                                                 \
}while(0)



/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	uint16_t Timeout;
	//------------起始信号------------
	LCD_I2C_Start();
	//------------发送地址------------	
	LCD_I2C_SendAddr(OLED_IIC_7ADDR<<1);
	//------------发送数据------------
	LCD_I2C_send_1Byte(0x00);              
	LCD_I2C_send_1Byte(dat);
	//------------结束信号------------                                                           
	LCD_I2C_Stop();  
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Dat(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个数据
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Dat(uint8_t dat)
{
	volatile uint16_t Timeout;
	//------------起始信号------------
	LCD_I2C_Start();
	//------------发送地址------------	
	LCD_I2C_SendAddr(OLED_IIC_7ADDR<<1);
	//------------发送数据------------
	LCD_I2C_send_1Byte(0x40);              
	LCD_I2C_send_1Byte(dat);
	//------------结束信号------------                                                           
	LCD_I2C_Stop();  
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
	uint16_t Timeout;
	uint16_t i;
	
	//------------起始信号------------
	LCD_I2C_Start();
	//------------发送地址------------	
	LCD_I2C_SendAddr(OLED_IIC_7ADDR<<1);
	//------------发送1+n个数据------------
	I2C_SendData(I2C2, 0x40);
	i=0;
	while(num>i)
	{
		Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		{
			if((Timeout--) == 0) break;
		}
		I2C_SendData(I2C2, p[i]);
		i++;
	}                
	//------------结束信号------------                                                            
	LCD_I2C_Stop();                                 
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
	uint16_t Timeout;
	uint16_t i;
	
	//------------起始信号------------
	LCD_I2C_Start();
	//------------发送地址------------	
	LCD_I2C_SendAddr(OLED_IIC_7ADDR<<1);
	//------------发送1+n个指令------------
	I2C_SendData(I2C2, 0x00);
	i=0;
	while(num>i)
	{
		Timeout = LCD_HARD_IIC_TIMEOUT_UINT8;
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		{
			if((Timeout--) == 0) break;
		}
		I2C_SendData(I2C2, p[i]);
		i++;
	}              
	//------------结束信号------------                                                           
	LCD_I2C_Stop();                               
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
  * 名称: LCD_Port_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 屏幕接口初始化
  * 说明: 
----------------------------------------------------------------*/
void LCD_Port_Init(void)
{
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed=400000;//默认400000 (某屏幕测试最高可使用600000)
	I2C_InitStruct.I2C_DutyCycle=I2C_DutyCycle_2;//I2C_DutyCycle_16_9   I2C_DutyCycle_2
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = OLED_IIC_7ADDR;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	I2C_DeInit(I2C2);
	I2C_Init(I2C2, &I2C_InitStruct);
	I2C_Cmd(I2C2, ENABLE);
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	#endif
	
	LCD_RES_Clr();
	LCD_delay_ms(100);
	LCD_RES_Set();
	LCD_delay_ms(100);
}

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
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
	//--1.配置刷新窗口位置--
	LCD_Set_Addr(x0,page);
	//--2.快速发送点阵数据--
	LCD_Send_nDat(page_gram,(x1-x0));
}
//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_GRAY_OLED)//灰度OLED
	//灰度OLED暂不支持硬件IIC 请使用软件IIC(存在硬件问题暂未解决)
	#error("Gray OLED not support hard_iic driver yet! Use soft_iic please!")
//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)
	//彩屏TFT屏不支持I2C方式驱动 请更改屏幕驱动IC型号
	#error ("TFT not support I2C Driver!");
#endif


#endif
