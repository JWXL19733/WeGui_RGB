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

#if (WEGUI_PORT == _STM32F103X_EC_BZ_PORT)
#include "stddef.h"
#include "stm32f103_wegui_ec_port.h"
#include "wegui_menu_demo.h"
#include "lcd_wegui_menu_mlist.h"
#include "lcd_wegui_tip.h"

#ifndef RCC_HCLK_Divx
	#define RCC_HCLK_Divx (RCC_HCLK_Div2)
#endif

//-----------------------------蜂鸣器拓展-----------------------------
musicPlayer_t bz_Player;
/*
//<<DEMO>>乐谱
const bz_Music_t music_ctrl[]=
{
	//{500Hz,停顿10ms,播放500ms,再停顿500ms}
	{BZ_FREQ_PERIOD(500),10,500,500},
	//{无声音,停顿0ms,死等500ms,停顿300ms}
	{0,0,500,300},
	//{结束组合符}
	{0,0,0,0},
};
buzz_play_music(music_ctrl);//播放示例
*/
//<<系统控制音>>
const bz_Music_t music_ctrl[]=
{
	//{8000Hz,停顿1ms,播放20ms,再停顿0ms}
	{BZ_FREQ_PERIOD(8000), 1,20,0},
	//{结束组合符}
	{0,0,0,0},
};
//<<游戏控制音>>
const bz_Music_t game_ctrl[]=
{
	//{8000Hz,停顿0ms,播放20ms,再停顿0ms}
	{BZ_FREQ_PERIOD(4000), 1,20,0},
	//{结束组合符}
	{0,0,0,0},
};
//<<晴天完整音乐>>
#include "music_qingtian.c"//const bz_Music_t music_qingtian[]={....}

/*--------------------------------------------------------------
  * 名称: buzz_set_freq(uint16_t hz)
  * 传入: hz 频率
  * 功能: 设置蜂鸣器发声频率
  * 说明: 设置范围[50:10000]
----------------------------------------------------------------*/
void buzz_set_freq(uint16_t hz)
{
	if(hz < 50){hz=50;}
	if(hz > 10000){hz=10000;}
	TIM2->ARR = 1000000/hz-1;
}
/*--------------------------------------------------------------
  * 名称: buzz_set_volume(uint8_t volume)
  * 传入: volume 音量
  * 功能: 设置底层音量(占空比)
  * 说明: 范围[0:16]
----------------------------------------------------------------*/
void buzz_set_volume(uint8_t volume)
{
	TIM2->CCR2 = (TIM2->ARR+1) * volume / 80;
}
/*--------------------------------------------------------------
  * 名称: buzz_on()
  * 功能: 打开蜂鸣器
----------------------------------------------------------------*/
void buzz_on()
{
	TIM_Cmd(TIM2, ENABLE);
}
/*--------------------------------------------------------------
  * 名称: buzz_off()
  * 功能: 关闭蜂鸣器
----------------------------------------------------------------*/
void buzz_off()
{
	TIM_Cmd(TIM2, DISABLE);
	TIM_SetCounter(TIM2, 65535);
}
/*--------------------------------------------------------------
  * 名称: buzz_play_music(const bz_Music_t *music)
  * 传入: music "乐谱"指针
  * 功能: 播放"乐谱"音乐
----------------------------------------------------------------*/
void buzz_play_music(const bz_Music_t *music)
{
	bz_Player.music = music;
	bz_Player.p_num = 0;
	bz_Player.step = 0;
}
/*--------------------------------------------------------------
  * 名称: buzz_music_player_is_busy()
  * 返回: 播放音乐中返回1 空闲中返回0
  * 功能: 
----------------------------------------------------------------*/
uint8_t buzz_music_player_is_busy()
{
	if(bz_Player.music != NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*--------------------------------------------------------------
  * 名称: Buzz_Init()
  * 功能: 蜂鸣器系统初始化
----------------------------------------------------------------*/
static void Buzz_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	RCC_PCLK1Config(RCC_HCLK_Divx);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	uint16_t PrescalerValue;
	switch (RCC_HCLK_Divx)
	{
		default:
		case RCC_HCLK_Div1: PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;break;//周期单位1M (500000?)
		case RCC_HCLK_Div2: PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;break;//周期单位1M
		case RCC_HCLK_Div4: PrescalerValue = (uint16_t) (SystemCoreClock / 2000000) - 1;break;//周期单位1M
		case RCC_HCLK_Div8: PrescalerValue = (uint16_t) (SystemCoreClock / 4000000) - 1;break;//周期单位1M
		case RCC_HCLK_Div16: PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;break;//周期单位1M
	}
  
  /* Time base configuration */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = 100-1; //周期单位1M
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; //预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//边缘/中心对齐相关
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_Pulse = 5;//占空比
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//CH1通道 (?)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//空闲低电平
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);//CH2通道
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//CH2通道
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, DISABLE);
	
	buzz_set_freq(20000);
	buzz_set_volume(1);
	buzz_off();
;
}
/*--------------------------------------------------------------
  * 名称: buzz_timer_irq()
  * 功能: 蜂鸣器系统时间中断
----------------------------------------------------------------*/
static void buzz_timer_irq()
{
	static const bz_Music_t *lastime_music = NULL;
	if(lastime_music != bz_Player.music)
	{
		buzz_off();
		lastime_music = bz_Player.music;
	}
	else if(bz_Player.music != NULL)
	{
		switch(bz_Player.step)
		{
			case 0://0.设置声调
			{
				if((bz_Player.music[bz_Player.p_num].bz_freq != 0x00)||
					(bz_Player.music[bz_Player.p_num].stop1_time != 0) ||
				(bz_Player.music[bz_Player.p_num].tone_time != 0x00) || 
				(bz_Player.music[bz_Player.p_num].stop2_time != 0))
				{
					if(bz_Player.music[bz_Player.p_num].bz_freq != 0x00)
					{
						//设置周期值
						TIM2->ARR = bz_Player.music[bz_Player.p_num].bz_freq;
						//重新设置占空比值音量
						buzz_set_volume(wegui.setting.voice_volume);
					}
					if(bz_Player.music[bz_Player.p_num].stop1_time == 0)
					{
						if((bz_Player.music[bz_Player.p_num].tone_time != 0)&&(bz_Player.music[bz_Player.p_num].bz_freq != 0x00))
						{
							buzz_on();
						}
						bz_Player.step=2;
					}
					else
					{
						buzz_off();
						bz_Player.step=1;
					}
					bz_Player.timer=0;
				}
				else
				{
					buzz_off();
					bz_Player.music = NULL;
				}
				
			}break;
			case 1://1.停顿
			{
				if(++bz_Player.timer>=bz_Player.music[bz_Player.p_num].stop1_time)
				{
					if((bz_Player.music[bz_Player.p_num].tone_time != 0)&&(bz_Player.music[bz_Player.p_num].bz_freq != 0x00))
					{
						buzz_on();
						bz_Player.step=3;
					}
					else
					{
						bz_Player.step=2;
					}
					bz_Player.timer=0;
				}
			}break;
			case 2://2.播放
			{
				
				if(++bz_Player.timer>=bz_Player.music[bz_Player.p_num].tone_time)
				{
					if(bz_Player.music[bz_Player.p_num].stop2_time > 0)
					{
						buzz_off();
					}
					bz_Player.timer=0;
					bz_Player.step=3;
				}
			}break;
			case 3://3.再停顿
			{
				if(++bz_Player.timer>=bz_Player.music[bz_Player.p_num].stop2_time)
				{
					bz_Player.p_num++;
					bz_Player.step=0;
				}
			}break;
		}
	}
}

//-----------------------------按键结构体-----------------------------
typedef enum Key_return//返回值
{
	KEY_RETURN_NONE = 0 ,
	START_SHORT_PRESS,           //开始短按
	START_LONG_PRESS,            //开始长按
	END_SHORT_PRESS,             //短按结束
	END_LONG_PRESS,              //长按结束
}Key_return_t;

typedef enum Key_state//按键状态
{
	KEY_STATE_NONE = 0,
	SHORT_PRESS_AND_HOLD,         //短按中
	LONG_PRESS_AND_HOLD,          //长按中
}Key_state_t;

typedef struct mykey//变量结构体
{
	Key_state_t keysw_state;
	uint16_t keysw_det_count;//时间计数值1
}mykey_t;

mykey_t key_ec;//编码器按键
//----------------------------短按长按------------------------------------
#define KEY_DELAY_COUNT 10          //按下消抖次数
#define KEY_LONG_COUNT 500          //"短按"到"长按"的计数时间
Key_return_t Keysw_det(mykey_t *p,uint8_t pin_state,uint16_t ms_stick)
{
	if(pin_state)
	{
		p->keysw_det_count += ms_stick;
		switch(p->keysw_state)
		{
			case KEY_STATE_NONE:{if(p->keysw_det_count>=KEY_DELAY_COUNT){p->keysw_state=SHORT_PRESS_AND_HOLD;p->keysw_det_count=0;return START_SHORT_PRESS;}}break;
			case SHORT_PRESS_AND_HOLD:{if(p->keysw_det_count>=KEY_LONG_COUNT){p->keysw_state=LONG_PRESS_AND_HOLD;p->keysw_det_count=0;return START_LONG_PRESS;};}break;
			case LONG_PRESS_AND_HOLD:{}break;
		}
	}
	else
	{
		switch(p->keysw_state)
		{	
			case KEY_STATE_NONE:{p->keysw_det_count=0;}break;
			case SHORT_PRESS_AND_HOLD:{p->keysw_det_count=0;p->keysw_state=KEY_STATE_NONE;return END_SHORT_PRESS;}//break;
			case LONG_PRESS_AND_HOLD:{p->keysw_det_count=0;p->keysw_state=KEY_STATE_NONE;return END_LONG_PRESS;}//break;
		}
	}
	return KEY_RETURN_NONE;
}

void key_par_init(mykey_t *p)
{
	p->keysw_state = KEY_STATE_NONE;
	p->keysw_det_count = 0;
}

int8_t ec_value=0;
uint8_t ec_io_history=0;
int8_t get_ec_value()
{
	return ec_value;
}

/*--------------------------------------------------------------
  * 名称: wegui_ec_buz_port_irq(void)
  * 传入: 无
  * 返回: 无
  * 功能: 编码器接口中断处理
  * 说明: 1ms定时执行
----------------------------------------------------------------*/
void wegui_ec_buz_port_irq(void)
{
	uint8_t io=0x00;
	static uint8_t speed_ms;//检测旋转速度
	if(get_ECA_io()){io|=0x01;}
	if(get_ECB_io()){io|=0x02;} 
	if((ec_io_history&0x03) != io)
	{
		ec_io_history = (ec_io_history << 2)|io;
		switch(ec_io_history&0xff)
		{
			case 0xD2://11 01 00 10 (11)
			{
					if(speed_ms > 6)
					{
						if(ec_value < 127){ec_value++;}//转得慢的不加速
					}
					else
					{
						if(ec_value < 125){ec_value+=4;}else{ec_value=127;}//转的快的加速
					}
					speed_ms=0;
			}break;
			case 0xE1://11 10 00 01 (11)
			{
					if(speed_ms > 6)
					{
						if(ec_value > -128){ec_value--;}//转得慢的不加速
					}
					else
					{
						if(ec_value > -126){ec_value-=4;}else{ec_value=-128;}//转的快的加速
					}
					speed_ms=0;
			}break;
		}
	}
	else
	{
		if(speed_ms<255){speed_ms++;};
	}
	buzz_timer_irq();
}

void wegui_ec_buz_port_task(uint8_t ms)
{
	Key_return_t key = Keysw_det(&key_ec ,(get_ECK_io()==RESET),ms);
	if((ec_value != 0)||(key != KEY_RETURN_NONE))
	{
		switch (wegui.tip.state)
		{
			//-------------------弹窗忙率:优先处理弹窗---------------------
			case ENTERING:
			case DISPLAYING:
			{
					switch (wegui.tip.type)
					{
						case message:
						{
							switch(key)
							{
								case START_SHORT_PRESS:       //开始短按
									wegui_tip_quit();break;
								default:break;
							}
							if(ec_value > 0)
							{
								wegui_tip_quit();
								ec_value--;
							}
							if(ec_value < 0)
							{
								wegui_tip_quit();
								ec_value++;
							}
						}break;
						case slider:
						{
							switch(key)
							{
								case START_SHORT_PRESS:       //开始短按
									wegui_tip_save_and_quit();break;
								default:break;
							}
							while(ec_value > 0)
							{
								if(wegui_tip_value_add())
								{
									buzz_play_music(music_ctrl);
								}
								ec_value--;
							}
							while(ec_value < 0)
							{
								if(wegui_tip_value_dec())
								{
									buzz_play_music(music_ctrl);
								}
								ec_value++;
							}
						}break;
					}
					
			}break;
			//---------------弹窗空闲或无弹窗:处理ui选项-----------------
			//
			case FREE://弹窗已退出
			case EXITING://弹窗退出中
			default:
			{
				switch(wegui.menu->menuType)
				{
					//-------------列表菜单-----------
					case mList:
					{
						switch(key)
						{
							case START_SHORT_PRESS:
								if(wegui_mlist_Enter_cursor())//光标确认
								{
									buzz_play_music(music_ctrl);
								}
							break;
							default:break;
						}
						while(ec_value > 0)
						{
							if(wegui_mlist_cursor_Prev())//光标向上
							{
								buzz_play_music(music_ctrl);
							}
							ec_value--;
						}
						while(ec_value < 0)
						{
							if(wegui_mlist_cursor_Next())//光标向下
							{
								buzz_play_music(music_ctrl);
							}
							ec_value++;
						}
					}break;
					//------------自定义功能APP----------
					case mPorgram:
					{
						if(wegui.menu == &m_App_MusicPlayer)
						{
							switch(key)
							{
								case START_SHORT_PRESS://开始短按
									wegui_mlist_Back_menu();
									break;
								default:break;
							}
							while(ec_value > 0)
							{
								demo_timer = 0;//播放音乐计时
								buzz_play_music(music_qingtian);//<<晴天>>
								ec_value--;
							}
							while(ec_value < 0)
							{
								demo_timer = 0;
								buzz_play_music(NULL);//关闭音乐
								ec_value++;
							}
						}
						//其他APP自定义
						else 
						{
							switch(key)
							{
								case START_SHORT_PRESS: //开始短按
									wegui_mlist_Back_menu();
								break;
								case START_LONG_PRESS:  //开始长按
									break; 
								case END_SHORT_PRESS:   //短按结束
									break;  
								case END_LONG_PRESS:    //长按结束
									break;   
								default:break;
							}
							while(ec_value > 0)       //编码器向上
							{
								wegui_mlist_Back_menu();
								ec_value--;
							}
							while(ec_value < 0)       //编码器向下
							{
								wegui_mlist_Back_menu();
								ec_value++;
							}
						}
					}break;
					case wCheckBox://控件:复选框(默认不会进入控件菜单)
						break;
					case wSlider:  //控件:滑条(默认不会进入控件菜单)
						break;
					case wMessage://控件:滑条(默认不会进入控件菜单)
						break;
				}
			}break;
		}
	}
}

/*--------------------------------------------------------------
  * 名称: wegui_ec_buz_port_init(void)
  * 传入: 无
  * 返回: 无
  * 功能: 编码器蜂鸣器初始化
  * 说明: 
----------------------------------------------------------------*/
void wegui_ec_buz_port_init()
{
	ECA_io_init();
	ECB_io_init();
	ECK_io_init();
	
	key_par_init(&key_ec);
	Buzz_Init();
	buzz_play_music(music_ctrl);
}


#endif
