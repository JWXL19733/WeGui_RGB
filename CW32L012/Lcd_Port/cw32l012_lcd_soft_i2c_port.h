/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef CW32L012_LCD_SOFT_I2C_PORT_H
#define CW32L012_LCD_SOFT_I2C_PORT_H

#include "cw32l012.h"
#include "cw32l012_gpio.h"
#include "cw32l012_sysctrl.h"
#include "cw32l012_crc.h"

//--RAM加速宏--
#ifndef RAM_SPEEDUP_FUNC_0
#	define RAM_SPEEDUP_FUNC_0
#endif


/*------------------------------------------------------
** 标准情况下,I2C最大通讯速率控制在400k
** 在屏幕通讯速率耐受较低时,建议使用"库函数"操作IO通讯,亦可减小上拉电阻
** 在屏幕通讯速率耐受较高时,建议使用"寄存器"操作IO通讯,亦可增加I2C驱动延时
------------------------------------------------------*/



//-----------------IO接口定义---------------- 

//-----SCL-----
//#define LCD_SCL_Clr() do{GPIO_WritePin(CW_GPIOB,GPIO_PIN_10,GPIO_Pin_RESET);}while(0)//库函数操作IO
//#define LCD_SCL_Set() do{GPIO_WritePin(CW_GPIOB,GPIO_PIN_10,GPIO_Pin_SET);}while(0)//库函数操作IO
#define LCD_SCL_Clr() do{CW_GPIOB->BRR = GPIO_PIN_10;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_Set() do{CW_GPIOB->BSRR = GPIO_PIN_10;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOA_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
    GPIO_InitStruct.Pins = GPIO_PIN_10;\
		GPIO_Init(CW_GPIOB, &GPIO_InitStruct);\
	}while(0)

//-----SDA-----
//#define LCD_SDA_Clr() do{GPIO_WritePin(CW_GPIOB,GPIO_PIN_11,GPIO_Pin_RESET);}while(0)//库函数操作IO
//#define LCD_SDA_Set() do{GPIO_WritePin(CW_GPIOB,GPIO_PIN_11,GPIO_Pin_SET);}while(0)//库函数操作IO
#define LCD_SDA_Clr() do{CW_GPIOB->BRR = GPIO_PIN_11;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SDA_Set() do{CW_GPIOB->BSRR = GPIO_PIN_11;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SDA_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOA_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
    GPIO_InitStruct.Pins = GPIO_PIN_11;\
		GPIO_Init(CW_GPIOB, &GPIO_InitStruct);\
		LCD_SDA_Set();\
	}while(0)
	
//-----RES-----(部分屏幕可选)
//#define LCD_RES_Clr() do{GPIO_WritePin(CW_GPIOA,GPIO_PIN_6,GPIO_Pin_RESET);}while(0)//库函数操作IO
//#define LCD_RES_Set() do{GPIO_WritePin(CW_GPIOA,GPIO_PIN_6,GPIO_Pin_SET);}while(0)//库函数操作IO
#define LCD_RES_Clr() do{CW_GPIOA->BRR = GPIO_PIN_6;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_RES_Set() do{CW_GPIOA->BSRR = GPIO_PIN_6;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_RES_IO_Init() \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOB_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
    GPIO_InitStruct.Pins = GPIO_PIN_6;\
		GPIO_Init(CW_GPIOA, &GPIO_InitStruct);\
		LCD_RES_Set();\
	}while(0)

	
	
#define LCD_is_Busy() (0)
void LCD_delay_ms(volatile uint32_t ms);
void LCD_Send_1Cmd(uint8_t dat);//向屏幕发送1个命令
void LCD_Send_1Dat(uint8_t dat);//向屏幕发送1个数据
void LCD_Send_nDat(uint8_t *p,uint16_t num);//向屏幕发送num个数据
void LCD_Send_nCmd(uint8_t *p,uint16_t num);//向屏幕发送num个命令
void LCD_Port_Init(void);//接口初始化	
uint8_t LCD_Refresh(void);

#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	void LCD_Reset_crc(void);//刷新一次crc值, 用于强制刷新屏幕,防止动态刷新出现区域不刷新
#endif

#endif
	
	
	
