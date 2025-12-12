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

#ifndef _STM32F103_WEGUI_4KEY_PORT_H_
#define _STM32F103_WEGUI_4KEY_PORT_H_

#include "stm32f10x.h"

/*---------------------
	*  ---默认EC接口---
	*  |A  | A8
	*  |B  | A10
	*  |K  | A9 
	*
	*  -默认蜂鸣器接口-
	*  A1
----------------------*/
/*
   ___|    ___|    ___    
  |   |   |   |   |   |   
A_|   |___|   |___|   |___
     _|_     _|_     ___   
    | | |   | | |   |   |  
B___| | |___| | |___|   |__
      |       |
	
	提示:若外接编码器无反应,尝试动一动开发板上的编码器
	
*/

//-------------------------------------按键配置-------------------------------------

#define KEY_UP_GPIOx                 GPIOA
#define KEY_UP_GPIO_Pin_x            GPIO_Pin_11
#define KEY_UP_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define key_up_io_init()                                        \
do                                                                  \
{                                                                   \
    GPIO_InitTypeDef GPIO_InitStruct;                               \
    GPIO_InitStruct.GPIO_Pin   = KEY_UP_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;                   \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;                     \
	  RCC_APB2PeriphClockCmd(KEY_UP_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(KEY_UP_GPIOx, &GPIO_InitStruct);                  \
}while(0)


#define KEY_DOWN_GPIOx                 GPIOA
#define KEY_DOWN_GPIO_Pin_x            GPIO_Pin_15
#define KEY_DOWN_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define key_down_io_init()                                        \
do                                                                  \
{                                                                   \
    GPIO_InitTypeDef GPIO_InitStruct;                               \
    GPIO_InitStruct.GPIO_Pin   = KEY_DOWN_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;                   \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;                     \
	  RCC_APB2PeriphClockCmd(KEY_DOWN_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(KEY_DOWN_GPIOx, &GPIO_InitStruct);                  \
}while(0)

#define KEY_LEFT_GPIOx                 GPIOB
#define KEY_LEFT_GPIO_Pin_x            GPIO_Pin_3
#define KEY_LEFT_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOB
#define key_left_io_Init()                                        \
do                                                                  \
{                                                                   \
    GPIO_InitTypeDef GPIO_InitStruct;                               \
    GPIO_InitStruct.GPIO_Pin   = KEY_LEFT_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;                   \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;                     \
	  RCC_APB2PeriphClockCmd(KEY_LEFT_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(KEY_LEFT_GPIOx, &GPIO_InitStruct);                  \
}while(0)

#define KEY_RIGHT_GPIOx                 GPIOA
#define KEY_RIGHT_GPIO_Pin_x            GPIO_Pin_12
#define KEY_RIGHT_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define key_right_io_init()                                        \
do                                                                  \
{                                                                   \
    GPIO_InitTypeDef GPIO_InitStruct;                               \
    GPIO_InitStruct.GPIO_Pin   = KEY_RIGHT_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;                   \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;                     \
	  RCC_APB2PeriphClockCmd(KEY_RIGHT_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(KEY_RIGHT_GPIOx, &GPIO_InitStruct);                  \
}while(0)

#define BOOL_KEY_UP_IS_PRESSED()    (GPIO_ReadInputDataBit(KEY_UP_GPIOx, KEY_UP_GPIO_Pin_x) == RESET)
#define BOOL_KEY_DOWN_IS_PRESSED()  (GPIO_ReadInputDataBit(KEY_DOWN_GPIOx, KEY_DOWN_GPIO_Pin_x) == RESET)
#define BOOL_KEY_RIGHT_IS_PRESSED() (GPIO_ReadInputDataBit(KEY_RIGHT_GPIOx, KEY_RIGHT_GPIO_Pin_x) == RESET)
#define BOOL_KEY_LEFT_IS_PRESSED()  (GPIO_ReadInputDataBit(KEY_LEFT_GPIOx, KEY_LEFT_GPIO_Pin_x) == RESET)

/*--------------------------------------------------------------
  * 名称: wegui_4key_port_init()
  * 功能: 4按键蜂鸣器接口初始化
----------------------------------------------------------------*/
void wegui_4key_port_init(void);

/*--------------------------------------------------------------
  * 名称: wegui_4key_port_task(uint16_t ms_stick)
  * 传入: ms_stick 距离上次运行的时间
  * 功能: 自定义按键功能
  * 说明: 传入ms_stick用于按键计算时间,无需使用中断
----------------------------------------------------------------*/
void wegui_4key_port_task(uint16_t ms);
/*--------------------------------------------------------------
  * 名称: wegui_4key_port_irq()
  * 功能: 4按键蜂鸣器接口中断处理
  * 说明: 
----------------------------------------------------------------*/
void wegui_4key_port_irq(void);



//-----------------------------蜂鸣器拓展-----------------------------
typedef struct
{
	uint16_t bz_freq;  //声调频率
	uint16_t stop1_time;//开始停顿时间
	uint16_t tone_time;//声调播放时间
	uint16_t stop2_time;//结束停顿时间
}bz_Music_t;

typedef struct
{
	const bz_Music_t *music;
	uint16_t p_num;
	uint16_t timer;
	uint8_t step;
}musicPlayer_t;
#define BZ_FREQ_PERIOD(freq_hz) ((uint16_t)(1000000/freq_hz-1))

/*--------------------------------------------------------------
  * 名称: buzz_set_freq(uint16_t hz)
  * 传入: hz 频率
  * 功能: 设置蜂鸣器发声频率
  * 说明: 设置范围[50:10000]
----------------------------------------------------------------*/
void buzz_set_freq(uint16_t hz);

/*--------------------------------------------------------------
  * 名称: buzz_set_volume(uint8_t volume)
  * 传入: volume 音量
  * 功能: 设置底层音量(占空比)
  * 说明: 范围[0:16]
----------------------------------------------------------------*/
void buzz_set_volume(uint8_t volume);

/*--------------------------------------------------------------
  * 名称: buzz_on()
  * 功能: 打开蜂鸣器
----------------------------------------------------------------*/
void buzz_on(void);

/*--------------------------------------------------------------
  * 名称: buzz_off()
  * 功能: 关闭蜂鸣器
----------------------------------------------------------------*/
void buzz_off(void);

/*--------------------------------------------------------------
  * 名称: buzz_play_music(const bz_Music_t *music)
  * 传入: music "乐谱"指针
  * 功能: 播放"乐谱"音乐
----------------------------------------------------------------*/
void buzz_play_music(const bz_Music_t *music);

/*--------------------------------------------------------------
  * 名称: buzz_music_player_is_busy()
  * 返回: 播放音乐中返回1 空闲中返回0
  * 功能: 
----------------------------------------------------------------*/
uint8_t buzz_music_player_is_busy(void);

#endif
