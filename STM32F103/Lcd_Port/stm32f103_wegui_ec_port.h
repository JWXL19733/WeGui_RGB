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

#ifndef _STM32F103_WEGUI_EC_PORT_H_
#define _STM32F103_WEGUI_EC_PORT_H_

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

//-------------------------------------EC配置-------------------------------------

#define ECA_GPIOx                 GPIOA
#define ECA_GPIO_Pin_x            GPIO_Pin_8
#define ECA_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define ECA_io_init()                                  \
do                                                           \
{                                                            \
    GPIO_InitTypeDef GPIO_InitStruct;                        \
    GPIO_InitStruct.GPIO_Pin   = ECA_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;            \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;              \
	RCC_APB2PeriphClockCmd(ECK_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(ECA_GPIOx, &GPIO_InitStruct);                  \
}while(0)


#define ECB_GPIOx                 GPIOA
#define ECB_GPIO_Pin_x            GPIO_Pin_10
#define ECB_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define ECB_io_init()                                  \
do                                                           \
{                                                            \
    GPIO_InitTypeDef GPIO_InitStruct;                        \
    GPIO_InitStruct.GPIO_Pin   = ECB_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;            \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;              \
	RCC_APB2PeriphClockCmd(ECB_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(ECB_GPIOx, &GPIO_InitStruct);                  \
}while(0)

#define ECK_GPIOx                 GPIOA
#define ECK_GPIO_Pin_x            GPIO_Pin_9
#define ECK_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOB
#define ECK_io_init()                                  \
do                                                           \
{                                                            \
    GPIO_InitTypeDef GPIO_InitStruct;                        \
    GPIO_InitStruct.GPIO_Pin   = ECK_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;            \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPU;              \
	RCC_APB2PeriphClockCmd(ECK_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(ECK_GPIOx, &GPIO_InitStruct);                  \
}while(0)

#define get_ECA_io() (GPIO_ReadInputDataBit(ECA_GPIOx, ECA_GPIO_Pin_x))
#define get_ECB_io() (GPIO_ReadInputDataBit(ECB_GPIOx, ECB_GPIO_Pin_x))
#define get_ECK_io() (GPIO_ReadInputDataBit(ECK_GPIOx, ECK_GPIO_Pin_x))

	
	
//-------------------------------------声明-------------------------------------
int8_t get_ec_value(void);//正数正转 负数反转
void wegui_ec_buz_port_init(void);//初始化io和变量 需执行一次
void wegui_ec_buz_port_irq(void);//可1ms执行一次 放到中断防止丢步
void wegui_ec_buz_port_task(uint8_t ms);//EC和按键任务处理,定时执行




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
  * 名称: Buzz_MusicPlayer_is_busy()
  * 返回: 播放音乐中返回1 空闲中返回0
  * 功能: 
----------------------------------------------------------------*/
uint8_t buzz_music_player_is_busy(void);


#endif
