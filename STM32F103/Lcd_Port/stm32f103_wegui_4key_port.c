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

#if (WEGUI_PORT == _STM32F103X_4KEY_BZ_PORT)
#include "stddef.h"
#include "stm32f103_wegui_4key_port.h"
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
	//{8000Hz,停顿50ms,播放20ms,再停顿0ms}
	{BZ_FREQ_PERIOD(8000),50,20,0},
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
	GPIO_InitTypeDef GPIO_InitStruct;
	uint16_t PrescalerValue;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_PCLK1Config(RCC_HCLK_Divx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
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
  TIM_TimeBaseStructure.TIM_Period = 100-1; //周期单位1M
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; //预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//边缘/中心对齐相关
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
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

//-------------------------------------按键结构体-------------------------------------
typedef enum Key_return//返回值
{
	key_return_none = 0 ,
	start_short_press,           //开始短按
	start_long_press,            //开始长按
	start_long_long_press,       //开始超长按
	long_press_trig,             //连续长按(机关枪)
	long_long_press_trig,        //连续超长按(机关枪)
	end_short_press,             //短按结束
	end_long_press,              //长按结束
	end_long_long_press,         //超长按结束
	
}Key_return_t;

typedef enum Key_state//按键状态
{
	key_state_none = 0,
	short_press_and_hold,         //短按中
	long_press_and_hold,          //长按中
	long_long_press_and_hold,     //超长按中
}Key_state_t;

typedef struct mykey//变量结构体
{
	Key_state_t keysw_state;
	uint16_t keysw_det_count;//时间计数值1
	uint16_t keysw_det_count2;//时间计数值2
}mykey_t;

/*--------------------------------------------------------------
  * 创建4个按键结构体
----------------------------------------------------------------*/
mykey_t key_left;
mykey_t key_right;
mykey_t key_up;
mykey_t key_down;


//----------------------------短按长按超长按+连续长按超长按------------------------------------
#define key_delay_count 10          //按下消抖次数
#define key_long_count 500          //"短按"到"长按"的计数时间
#define key_long_trig_count 80      //"连续长按"的机关枪间隔触发时间
#define key_long_long_count 2000    //"长按"到"超长按"的计数时间
#define key_long_long_trig_count 12 //"连续超长按"的机关枪间隔触发时间

Key_return_t Keysw_det(mykey_t *p,uint8_t pin_state,uint16_t ms_stick)
{
	if(pin_state)
	{
		p->keysw_det_count += ms_stick;
		switch(p->keysw_state)
		{
			case key_state_none:{p->keysw_det_count2=0;if(p->keysw_det_count>=key_delay_count){p->keysw_state=short_press_and_hold;p->keysw_det_count=0;return start_short_press;}}break;
			case short_press_and_hold:{p->keysw_det_count2=0;if(p->keysw_det_count>=key_long_count){p->keysw_state=long_press_and_hold;p->keysw_det_count=0;return start_long_press;};}break;
			case long_press_and_hold:{p->keysw_det_count2+=ms_stick;if((p->keysw_det_count>=key_long_long_count)){p->keysw_state=long_long_press_and_hold;p->keysw_det_count=0;return start_long_long_press;}if(p->keysw_det_count2>=key_long_trig_count){p->keysw_det_count2-=key_long_trig_count;return long_press_trig;}}break;
			case long_long_press_and_hold:{if(p->keysw_det_count>=key_long_long_trig_count){p->keysw_det_count=0;return long_long_press_trig;}}break;
		}
	}
	else
	{
		p->keysw_det_count2=0;
		switch(p->keysw_state)
		{	
			case key_state_none:{p->keysw_det_count=0;}break;
			case short_press_and_hold:{p->keysw_det_count=0;p->keysw_state=key_state_none;return end_short_press;}//break;
			case long_press_and_hold:{p->keysw_det_count=0;p->keysw_state=key_state_none;return end_long_press;}//break;
			case long_long_press_and_hold:{p->keysw_det_count=0;p->keysw_state=key_state_none;return end_long_press;}//break;
		}
	}
	return key_return_none;
}
/*--------------------------------------------------------------
  * 名称: key_par_init(mykey_t *p)
  * 功能: 按键结构体初始化
----------------------------------------------------------------*/
void key_par_init(mykey_t *p)
{
	p->keysw_state = key_state_none;
	p->keysw_det_count = 0;
	p->keysw_det_count2 = 0;
}

/*--------------------------------------------------------------
  * 名称: key_up_func(uint16_t ms_stick)
  * 传入: ms_stick 距离上次运行的时间,传入用于计算时间
  * 功能: 自定义按键功能
  * 说明: 范围[0:16]
----------------------------------------------------------------*/
static void key_up_func(uint16_t ms_stick)
{
	Key_return_t i = Keysw_det(&key_up ,BOOL_KEY_UP_IS_PRESSED(),ms_stick);
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
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_tip_quit();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					case slider:
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_tip_save_and_quit();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}break;
				}
		}break;
		//-------------------弹窗空闲:处理菜单/APP---------------------
		default:
		case FREE://弹窗已退出
		case EXITING://弹窗正在退出
		{
			switch(wegui.menu->menuType)
			{
				case mList:     //列表菜单
				{
					switch(i)
					{
						case start_short_press:       //开始短按
						case start_long_press:        //开始长按
						case long_press_trig:				  //连续长按
						case long_long_press_trig:    //连续超长按
							if(wegui_mlist_cursor_Prev())
							{
								buzz_play_music(music_ctrl);
							}
							break;
						default:break;
					}
				}break;
				case mPorgram:  //自定义功能APP
					if(wegui.menu == &m_App_MusicPlayer)
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					else
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					break;
				case wCheckBox: //控件:复选框(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wSlider:   //控件:滑条(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wMessage:
					break;
			}
		}break;
	}
}

/*--------------------------------------------------------------
  * 名称: key_down_func(uint16_t ms_stick)
  * 传入: ms_stick 距离上次运行的时间
  * 功能: 自定义按键功能
  * 说明: 传入ms_stick用于按键计算时间
----------------------------------------------------------------*/
static void key_down_func(uint16_t ms_stick)
{
	Key_return_t i = Keysw_det(&key_down ,BOOL_KEY_DOWN_IS_PRESSED(),ms_stick);
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
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_tip_quit();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					case slider:break;
				}
		}break;
		//-------------------弹窗空闲:处理菜单/APP---------------------
		case FREE://弹窗已退出
		case EXITING://弹窗正在退出
		default:
		{
			switch(wegui.menu->menuType)
			{
				case mList:     //列表菜单
				{
					switch(i)
					{
						case start_short_press:       //开始短按
						case start_long_press:        //开始长按
						case long_press_trig:				  //连续长按
						case long_long_press_trig:    //连续超长按
							if(wegui_mlist_cursor_Next())
							{
								buzz_play_music(music_ctrl);
							}
							break;
						default:break;
					}
				}break;
				case mPorgram:  //自定义功能APP
					if(wegui.menu == &m_App_MusicPlayer)
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					else
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					break;
				case wCheckBox: //控件:复选框(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wSlider:   //控件:滑条(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wMessage:
					break;
			}
		}
	}
}

/*--------------------------------------------------------------
  * 名称: key_left_func(uint16_t ms_stick)
  * 传入: ms_stick 距离上次运行的时间
  * 功能: 自定义按键功能
  * 说明: 传入ms_stick用于按键计算时间
----------------------------------------------------------------*/
static void key_left_func(uint16_t ms_stick)
{
	Key_return_t i = Keysw_det(&key_left ,BOOL_KEY_LEFT_IS_PRESSED(),ms_stick);
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
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_tip_quit();
								break;
							default:break;
						}
					}
					case slider:
					{
						switch(i)
						{
							case start_short_press:       //开始短按
							case start_long_press:        //开始长按
							case long_press_trig:				  //连续长按
							case long_long_press_trig:    //连续超长按
								if(wegui_tip_value_dec())
								{
									buzz_play_music(music_ctrl);
								}
								break;
							default:break;
						}
					}break;
				}
		}break;
		//-------------------弹窗空闲:处理菜单/APP---------------------
		case FREE://弹窗已退出
		case EXITING://弹窗正在退出
		default:
		{
			switch(wegui.menu->menuType)
			{
				case mList:     //列表菜单
				{
					switch(i)
					{
						case start_short_press:       //开始短按
							if(wegui_mlist_Back_menu())
							{
								buzz_play_music(music_ctrl);
							}
							break;
						default:break;
					}
				}break;
				case mPorgram:  //自定义功能APP
					if(wegui.menu == &m_App_MusicPlayer)
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					else
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					break;
				case wCheckBox: //控件:复选框(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wSlider:   //控件:滑条(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wMessage:
					break;
			}
		}break;
	}
}

/*--------------------------------------------------------------
  * 名称: key_right_func(uint16_t ms_stick)
  * 传入: ms_stick 距离上次运行的时间
  * 功能: 自定义按键功能
  * 说明: 传入ms_stick用于按键计算时间
----------------------------------------------------------------*/
static void key_right_func(uint16_t ms_stick)
{
	Key_return_t i = Keysw_det(&key_right ,BOOL_KEY_RIGHT_IS_PRESSED(),ms_stick);
	//-------------------弹窗忙率:优先处理弹窗---------------------
	switch (wegui.tip.state)
	{
		case ENTERING:
		case DISPLAYING:
		{
				switch (wegui.tip.type)
				{
					case message:
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_tip_quit();
								buzz_play_music(music_ctrl);
								break;
							default:break;
						}
					}
					case slider:
					{
						switch(i)
						{
							case start_short_press:       //开始短按
							case start_long_press:        //开始长按
							case long_press_trig:				  //连续长按
							case long_long_press_trig:    //连续超长按
								if(wegui_tip_value_add())
								{
									buzz_play_music(music_ctrl);
								}
								break;
							default:break;
						}
					}break;
				}
		}break;
		//-------------------弹窗空闲:处理菜单/APP---------------------
		case FREE://弹窗已退出
		case EXITING://弹窗正在退出
		default:
		{
			switch(wegui.menu->menuType)
			{
				case mList:     //列表菜单
				{
					switch(i)
					{
						case start_short_press:       //开始短按
						{
							if(wegui_mlist_Enter_cursor())//光标确认
							{
								buzz_play_music(music_ctrl);
							}
							break;
						}
						default:break;
					}
				}break;
				case mPorgram:  //自定义功能APP
					if(wegui.menu == &m_App_MusicPlayer)
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								if(buzz_music_player_is_busy())
									buzz_play_music(NULL);
								else
									buzz_play_music(music_qingtian);
								break;
							default:break;
						}
					}
					else
					{
						switch(i)
						{
							case start_short_press:       //开始短按
								wegui_mlist_Back_menu();
								break;
							default:break;
						}
					}
					break;
				case wCheckBox: //控件:复选框(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wSlider:   //控件:滑条(默认不会进入控件菜单,只会以Tip弹窗形式运行)
					break;
				case wMessage:
					break;
			}
		}break;
	}
}

/*--------------------------------------------------------------
  * 名称: wegui_4key_port_init()
  * 功能: 4按键蜂鸣器接口初始化
----------------------------------------------------------------*/
void wegui_4key_port_init()//4按键蜂鸣器接口初始化
{
	//按键io初始化
	key_left_io_Init();
	key_up_io_init();
	key_down_io_init();
	key_right_io_init();
	
	//按键变量初始化
	key_par_init(&key_left);
	key_par_init(&key_right);
	key_par_init(&key_up);
	key_par_init(&key_down);
	
	//蜂鸣器
	Buzz_Init();
	//开机音乐
	//buzz_play_music(music_qingtian);
}

/*--------------------------------------------------------------
  * 名称: wegui_4key_port_task(uint16_t ms_stick)
  * 传入: ms_stick 距离上次运行的时间
  * 功能: 自定义按键功能
  * 说明: 传入ms用于按键计算时间
----------------------------------------------------------------*/
void wegui_4key_port_task(uint16_t ms)//4按键蜂鸣器接口任务处理
{
	//--1.查询方式--
	key_up_func(ms)   ;
	key_down_func(ms) ;
	key_left_func(ms) ;
	key_right_func(ms);
	
	//--2.中断方式--
	//(放到中断)
	//key_up(1)   ;
	//key_down(1) ;
	//key_left(1) ;
	//key_right(1);
}

/*--------------------------------------------------------------
  * 名称: wegui_4key_port_irq()
  * 功能: 4按键蜂鸣器接口中断处理
  * 说明: 可放到定时中断
----------------------------------------------------------------*/
void wegui_4key_port_irq()//4按键蜂鸣器接口中断处理
{
	//--按键中断方式--
	//(已使用查询方式,不使用中断)
	//key_up(1)   ;
	//key_down(1) ;
	//key_left(1) ;
	//key_right(1);
	
	//蜂鸣器
	buzz_timer_irq();
}

#endif
