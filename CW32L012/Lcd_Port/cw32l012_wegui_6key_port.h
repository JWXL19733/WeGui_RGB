/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef CW32L012_Wegui_4KEY_PORT_H
#define CW32L012_Wegui_4KEY_PORT_H

#include "cw32l012.h"
#include "cw32l012_gpio.h"
#include "cw32l012_sysctrl.h"


//-------------------------------------按键配置-------------------------------------



//-------------------------------------按键配置-------------------------------------

#define Wegui_KeyBack_GPIOx      CW_GPIOC
#define Wegui_KeyBack_GPIO_PIN_x GPIO_PIN_15
#define Wegui_KeyBack_IO_Init()                                                                   \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOC_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;\
    GPIO_InitStruct.Pins = Wegui_KeyBack_GPIO_PIN_x;\
		GPIO_Init(Wegui_KeyBack_GPIOx, &GPIO_InitStruct);\
	}while(0)

#define Wegui_KeyOk_GPIOx      CW_GPIOA
#define Wegui_KeyOk_GPIO_PIN_x GPIO_PIN_0
#define Wegui_KeyOk_IO_Init()                                                                 \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOA_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;\
    GPIO_InitStruct.Pins = Wegui_KeyOk_GPIO_PIN_x;\
		GPIO_Init(Wegui_KeyOk_GPIOx, &GPIO_InitStruct);\
	}while(0)

							
#define Wegui_KeyRight_GPIOx      CW_GPIOB
#define Wegui_KeyRight_GPIO_PIN_x GPIO_PIN_12
#define Wegui_KeyRight_IO_Init()                                                                    \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOB_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;\
    GPIO_InitStruct.Pins = Wegui_KeyRight_GPIO_PIN_x;\
		GPIO_Init(Wegui_KeyRight_GPIOx, &GPIO_InitStruct);\
	}while(0)

#define Wegui_KeyUp_GPIOx      CW_GPIOB
#define Wegui_KeyUp_GPIO_PIN_x GPIO_PIN_13
#define Wegui_KeyUp_IO_Init()                                                                 \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOB_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;\
    GPIO_InitStruct.Pins = Wegui_KeyUp_GPIO_PIN_x;\
		GPIO_Init(Wegui_KeyUp_GPIOx, &GPIO_InitStruct);\
	}while(0)

#define Wegui_KeyDown_GPIOx      CW_GPIOB
#define Wegui_KeyDown_GPIO_PIN_x GPIO_PIN_14
#define Wegui_KeyDown_IO_Init()                                                                   \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOB_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;\
    GPIO_InitStruct.Pins = Wegui_KeyDown_GPIO_PIN_x;\
		GPIO_Init(Wegui_KeyDown_GPIOx, &GPIO_InitStruct);\
	}while(0)

#define Wegui_KeyLeft_GPIOx      CW_GPIOB
#define Wegui_KeyLeft_GPIO_PIN_x GPIO_PIN_15
#define Wegui_KeyLeft_IO_Init()                                                                   \
	do{\
		GPIO_InitTypeDef GPIO_InitStruct;\
		__SYSCTRL_GPIOB_CLK_ENABLE();\
		GPIO_InitStruct.IT = GPIO_IT_NONE;\
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;\
    GPIO_InitStruct.Pins = Wegui_KeyLeft_GPIO_PIN_x;\
		GPIO_Init(Wegui_KeyLeft_GPIOx, &GPIO_InitStruct);\
	}while(0)






//开发板正向按键
/*---------------------------
	*  ---默认按键接口---
	*  |OK   | A0  |确定|
	*  |BACK | C15 |返回|
	*  |UP   | B13 | 上 |
	*  |DOWN | B14 | 下 |
	*  |LEFT | B15 | 左 |
	*  |RIGHT| B12 | 右 |
----------------------------*/
#define Bool_WKeyBack_is_Pressed()  (GPIO_ReadPin(Wegui_KeyBack_GPIOx, Wegui_KeyBack_GPIO_PIN_x) == GPIO_Pin_RESET)
#define Bool_WKeyOk_is_Pressed()    (GPIO_ReadPin(Wegui_KeyOk_GPIOx, Wegui_KeyOk_GPIO_PIN_x) == GPIO_Pin_RESET)
#define Bool_WkeyRight_is_Pressed() (GPIO_ReadPin(Wegui_KeyRight_GPIOx, Wegui_KeyRight_GPIO_PIN_x) == GPIO_Pin_RESET)
#define Bool_WKeyUp_is_Pressed()    (GPIO_ReadPin(Wegui_KeyUp_GPIOx, Wegui_KeyUp_GPIO_PIN_x) == GPIO_Pin_RESET)
#define Bool_WKeyDown_is_Pressed()  (GPIO_ReadPin(Wegui_KeyDown_GPIOx, Wegui_KeyDown_GPIO_PIN_x) == GPIO_Pin_RESET)
#define Bool_WkeyLeft_is_Pressed()  (GPIO_ReadPin(Wegui_KeyLeft_GPIOx, Wegui_KeyLeft_GPIO_PIN_x) == GPIO_Pin_RESET)






//-------------------------------------声明-------------------------------------

void Wegui_6key_port_Init(void);
void Wegui_Interface_stick(uint16_t ms);

#endif
