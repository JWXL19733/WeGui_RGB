/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if (LCD_PORT == _HARD_I2C)

#include "stdint.h"
#include "cw32l012_lcd_hard_i2c_port.h"
#include "lcd_driver.h"



#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
static uint16_t crc[((SCREEN_HIGH+7)/8)][PAGE_CRC_NUM];//储存crc
/*--------------------------------------------------------------
  * 名称: LCD_Reset_crc()
  * 功能: 刷新一次crc值(动态刷新专用)
  * 说明: 用于强制刷新屏幕,防止动态刷新出现区域不刷新
----------------------------------------------------------------*/
void LCD_Reset_crc()
{
	uint16_t i=0;
	uint16_t *p=&crc[0][0];
	while(i < ((SCREEN_HIGH+7)/8)*PAGE_CRC_NUM)
	{
		p[i]=0xff;//随机值,不是0x0就ok
		i++;
	}
}

//默认的动态刷新crc算法
#define lcd_crc() do \
{\
		CW_CRC->CR = CRC16_IBM;\
		uint8_t *i = &lcd_driver.LCD_GRAM[ypage][0][0];\
		for(x=0;x<(SCREEN_WIDTH*LCD_COLOUR_BIT);x++)\
		{\
			CW_CRC->DR = i[x];\
		}\
		i_crc = CW_CRC->RESULT;\
}while(0)
#endif

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	uint8_t i2c_temp[]={0x00,dat};
	//----------起始信号+地址------------
	I2C_MasterSTART_Addr7b(CW_I2C1, OLED_IIC_7ADDR, I2C_MODE_WRITE);
	//------------发送数据------------
	I2C_MasterSend(CW_I2C1, i2c_temp, 2);
	//------------结束信号------------                                                           
	I2C_MasterSTOP(CW_I2C1);
	
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
	uint8_t i2c_temp[]={0x40,dat};
	//----------起始信号+地址------------
	I2C_MasterSTART_Addr7b(CW_I2C1, OLED_IIC_7ADDR, I2C_MODE_WRITE);
	//------------发送数据------------
	I2C_MasterSend(CW_I2C1, i2c_temp, 2);
	//------------结束信号------------                                                           
	I2C_MasterSTOP(CW_I2C1);
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
	uint8_t i2c_temp[]={0x40};
	//----------起始信号+地址------------
	I2C_MasterSTART_Addr7b(CW_I2C1, OLED_IIC_7ADDR, I2C_MODE_WRITE);
	//------------发送数据------------
	I2C_MasterSend(CW_I2C1, i2c_temp, 1);//发送0x40
	I2C_MasterSend(CW_I2C1, p, num);
	//------------结束信号------------                                                           
	I2C_MasterSTOP(CW_I2C1);	
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
	uint8_t i2c_temp[]={0x00};
	//----------起始信号+地址------------
	I2C_MasterSTART_Addr7b(CW_I2C1, OLED_IIC_7ADDR, I2C_MODE_WRITE);
	//------------发送数据------------
	I2C_MasterSend(CW_I2C1, i2c_temp, 1);//发送0x40
	I2C_MasterSend(CW_I2C1, p, num);
	//------------结束信号------------                                                           
	I2C_MasterSTOP(CW_I2C1);		
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
	//delay_ms(ms);
	
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
#include "cw32l012_flash.h"
void LCD_Port_Init(void)
{
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	
	I2C_DeInit(CW_I2C1);
	I2C_MasterConfigTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_BaudRate_Hz = I2C_BaudRate_Hz_x;
  I2C_InitStruct.I2C_ClockSouce = I2C_CLK_SRC_PCLK;
  I2C_InitStruct.I2C_DataMatchMode = I2C_ByteMatch_Prohibit;
  I2C_InitStruct.I2C_PinsConfig = 0;
  I2C_InitStruct.I2C_SDA_Source = I2C_SDA_INSRC_GPIO;
  I2C_InitStruct.I2C_SCL_Source = I2C_SCL_INSRC_GPIO;
  I2C_InitStruct.I2C_InDebugState = FALSE;
  I2C_InitStruct.I2C_InDeepSleepState = TRUE;
  I2C_InitStruct.I2C_RxDataMatch = FALSE;
  I2C_InitStruct.I2C_TxFIFO_Autoload = FALSE;
  I2C_InitStruct.ignoreAck = TRUE;
  I2C_InitStruct.sclGlitchFilterWidth_ns = 0; /* Set to 0 to disable the function */
  I2C_InitStruct.sdaGlitchFilterWidth_ns = 0;
  I2C_InitStruct.busIdleTimeout_ns = 0;
  I2C_InitStruct.pinLowTimeout_ns = 0;
  I2C_InitStruct.hostRequest.enable = FALSE;
  I2C_InitStruct.hostRequest.source = kLPI2C_HostRequestExternalPin;
  I2C_InitStruct.hostRequest.polarity = kLPI2C_HostRequestPinActiveHigh;
	I2C_MasterInit(CW_I2C1, &I2C_InitStruct, SystemCoreClock);
	I2C_MasterCmd(CW_I2C1, ENABLE); // I2C模块使能
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	__SYSCTRL_CRC_CLK_ENABLE();//动态刷新CRC校验用
	#endif
	
	LCD_RES_Clr();
	LCD_delay_ms(200);
	LCD_RES_Set();
}




/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/

//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if defined (LCD_USE_NORMAL_OLED)

#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全缓存全屏刷新----------
RAM_SPEEDUP_FUNC_0 //代码加速宏定义
uint8_t LCD_Refresh(void)
{
	uint8_t ypage;
	for(ypage=0;ypage<(SCREEN_HIGH/8);ypage++)
	{
		LCD_Set_Addr(0,ypage);
		LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
	}
	return 0;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
//---------方式2:全缓存动态刷新----------
RAM_SPEEDUP_FUNC_0 //代码加速宏定义
uint8_t LCD_Refresh(void)
{
	//每page做"sum+mini_crc组合"校验,若校验码没变,则不刷新该page
	uint8_t ypage;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		
		//-----方式1:CRC算法校验-----
		lcd_crc();//宏定义
		//---------------------------
		
		if(crc[ypage][0] != i_crc)
		{
			LCD_Set_Addr(0,ypage);
			LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
			crc[ypage][0] = i_crc;
		}
	}
	return 0;
}
//---------方式3:页缓存全局刷新----------
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)
RAM_SPEEDUP_FUNC_0 //代码加速宏定义
uint8_t LCD_Refresh(void)
{
	uint8_t i=0;
	for(i=0;i<GRAM_YPAGE_NUM;i++)
	{
		LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage);
		LCD_Send_nDat(&lcd_driver.LCD_GRAM[i][0][0],SCREEN_WIDTH);
		lcd_driver.lcd_refresh_ypage++;
		if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
		{
			lcd_driver.lcd_refresh_ypage = 0;
			break;
		}
	}
	return lcd_driver.lcd_refresh_ypage;
}
//---------方式4:页缓存动态刷新----------
#elif (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)
RAM_SPEEDUP_FUNC_0 //代码加速宏定义
uint8_t LCD_Refresh(void)
{
	uint8_t ypage;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		//判断屏幕是否已刷完
		if((lcd_driver.lcd_refresh_ypage + ypage)>=((SCREEN_HIGH+7)/8))
		{
			break;
		}
		//-----方式1:CRC算法校验-----
		lcd_crc();//宏定义
		//---------------------------
		if(crc[lcd_driver.lcd_refresh_ypage + ypage][0] != i_crc)
		{
			LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage + ypage);
			LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
			crc[lcd_driver.lcd_refresh_ypage + ypage][0] = i_crc;
		}
	}
	
	lcd_driver.lcd_refresh_ypage += GRAM_YPAGE_NUM;
	if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
	{
		lcd_driver.lcd_refresh_ypage = 0;
	}
	return lcd_driver.lcd_refresh_ypage;
}
#endif




//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_GRAY_OLED)//灰度OLED
	#error("Gray OLED not support hard_iic driver yet! Use soft_iic please!")
	
	
//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)
	//彩屏TFT屏不支持I2C方式驱动 请更改屏幕驱动IC型号
	#error ("TFT not support I2C Driver!");
#endif





#endif
