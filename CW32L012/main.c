/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/


#include "main.h"

#include "cw32l012.h"
#include "cw32l012_systick.h"
#include "cw32l012_gpio.h"
#include "cw32l012_flash.h"

extern uint16_t ResADC;
extern uint8_t adc_en;
extern uint32_t apptimer_ms;

uint16_t sys1ms_stick;


//系统滴答定时器
void SysTick_Handler(void)
{
	uwTick += uwTickFreq;//cw32内置库需要使用, 否则delay不工作
	
	Wegui_1ms_stick();
	if(sys1ms_stick < 65535)
	{
		sys1ms_stick++;
	}
}

//开机初始化
void startup_init()
{
	FLASH_Init(96000000);//先调高主频等待周期(96MHz即出初始化成3周期)
	SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);//再调高主频分频到96MHz
	FLASH_PrefetchCmd(ENABLE);//再设置指令预取使能
	FLASH_CacheCmd(ENABLE);//再设置指令缓存使能
	SYSCTRL_SystemCoreClockUpdate(96000000);
	
	//--板载指示灯--
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.IT = GPIO_IT_NONE;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pins = GPIO_PIN_13;
		__SYSCTRL_GPIOC_CLK_ENABLE();
		GPIO_Init(CW_GPIOC, &GPIO_InitStruct);
	}
	
	//--系统滴答1ms定时--
	InitTick(96000000);
	sys1ms_stick = 0;
}

void delay_ms(uint16_t ms)
{
	SysTickDelay(ms);
}


//--板载指示灯--
void LED_Func()
{
	GPIO_TogglePin(CW_GPIOC, GPIO_PIN_13);
}

#if (LCD_COLOUR_BIT == 1)
void gui_1bit_demo()//单色刷图demo
{
	
#	ifdef LCD_USE_RGB565
		RGB_Set_Driver_Colour(0,COLOUR_MLIST_DEFAULT_0);//设置Lcd_Set_Driver_Mode(write_0)的笔刷颜色
		RGB_Set_Driver_Colour(1,COLOUR_MLIST_DEFAULT_1);//设置Lcd_Set_Driver_Mode(write_1)的笔刷颜色
		//Lcd_Set_Driver_Mode(write_1);//初始化默认以write_1笔刷运行
#	endif
	
	
	
	#define DEMO_DELAY_MS 500
	
	
	//--------------文字显示字体1-----------------
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();
		//2.-----绘图-----
		lcd_driver.fonts_ASCII = &ascii_SongTi_8X16;//ASCII字体
		lcd_driver.fonts_UTF8_cut = &SongTi_UTF8_16X16;//UTF8字体(裁切)
		lcd_driver.newline_high = lcd_driver.fonts_UTF8_cut->high;//更新\n换行间距
		Lcd_Draw_UTF8_String(0,0,"Fonts 中文\n8x16\n16x16");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);

	//--------------文字显示字体2-----------------
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();
		//2.-----绘图-----
		lcd_driver.fonts_ASCII = &ascii_SongTi_6X12;//ASCII字体
		lcd_driver.fonts_UTF8_cut = &SongTi_UTF8_12X12;//UTF8字体(裁切)
		lcd_driver.newline_high = lcd_driver.fonts_UTF8_cut->high;//更新\n换行间距
		Lcd_Draw_UTF8_String(0,0,"Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	

	

	//--------------------画线-----------------------
	{
		uint16_t x=0;
		while(x <= SCREEN_WIDTH-1)
		{
			do
			{
				//0.---等待发送完毕---
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				//1.---清空缓存---
				Lcd_Clear_GRAM();
				//2.-----绘图-----
				Lcd_Draw_Line(0,0,x,SCREEN_HIGH-1);
				Lcd_Draw_Line(SCREEN_WIDTH-1,0,SCREEN_WIDTH-1-x,SCREEN_HIGH-1);
				
			}while(LCD_Refresh());//直到每行都刷完
			x++;
		}
	}
	
	{
		uint16_t y=0;
		while(y <= SCREEN_HIGH-1)
		{
			do
			{
				//0.---等待发送完毕---
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				//1.---清空缓存---
				Lcd_Clear_GRAM();
				//2.-----绘图-----
				Lcd_Draw_Line(0,0,SCREEN_WIDTH-1,SCREEN_HIGH-y);
				Lcd_Draw_Line(SCREEN_WIDTH-1,0,0,SCREEN_HIGH-y);
				
			}while(LCD_Refresh());//直到每行都刷完
			y++;
		}
	}
	//--------------------画圆-----------------------
	{
		uint8_t i=0;
		uint8_t C[]=
		{
			C_RU,//右上八分之一圆
			C_UR,//上右八分之一圆
			C_UL,//上左八分之一圆
			C_LU,//左上八分之一圆
			C_LD,//左下八分之一圆
			C_DL,//下左八分之一圆
			C_DR,//下右八分之一圆
			C_RD,//右下八分之一圆
			
			C_QRU,//右上角四分圆
			C_QLU,//左上角四分圆
			C_QLD,//左下角四分圆
			C_QRD,//右下角四分圆
			
			C_HR,//右半圆
			C_HU,//上半圆
			C_HL,//左半圆
			C_HD,//下半圆
			
			C_HR|C_UL,//右半圆+上左 (任意组合)
			C_HU|C_LD,//上半圆+左下
			C_HL|C_DR,//左半圆+下右
			C_HD|C_RU,//下半圆+右上

			C_ALL,//完整的园
		};
		while(i<=(sizeof(C)/sizeof(uint8_t)-1))
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Draw_Circel_part(32,20,16,(circle_part_t)C[i]);
				Lcd_Fill_Circel_part(SCREEN_WIDTH-32,16,16,(circle_part_t)C[i]);
			}while(LCD_Refresh());//直到每行都刷完
			delay_ms(50);
			i++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	
	//--------------------矩形-----------------------
	{
		uint16_t i=0;
		while((i<SCREEN_HIGH) || (i<SCREEN_WIDTH))
		{
			uint16_t x,y;
			if(i < SCREEN_WIDTH){x=i;}else{x=SCREEN_WIDTH-1;}
			if(i < SCREEN_HIGH){y=i;}else{y=SCREEN_HIGH-1;}
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Draw_Box(0,0, x, y);
			}while(LCD_Refresh());
			i++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	{
		uint16_t i=0;
		while((i<SCREEN_HIGH) || (i<SCREEN_WIDTH))
		{
			uint16_t x,y;
			if(i < SCREEN_WIDTH){x=i;}else{x=SCREEN_WIDTH-1;}
			if(i < SCREEN_HIGH){y=i;}else{y=SCREEN_HIGH-1;}
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Fill_Box(0,0, x, y);
			}while(LCD_Refresh());
			i++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	
	//--------------------圆角矩形-----------------------
	{
		uint16_t r=0;
		while((r<SCREEN_HIGH/2)&&(r<SCREEN_WIDTH/2))
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Draw_RBox(0,0,SCREEN_WIDTH-1, SCREEN_HIGH-1,r);
			}while(LCD_Refresh());
			r++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	{
		uint16_t r=0;
		while((r<SCREEN_HIGH/2)&&(r<SCREEN_WIDTH/2))
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Fill_RBox(0,0,SCREEN_WIDTH-1, SCREEN_HIGH-1,r);
			}while(LCD_Refresh());
			r++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	//--------------------十进制数字-----------------------
	{
		int16_t r=-500;
		while(r<1200)
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Draw_int32(0,0,r);//十进制带符号
			}while(LCD_Refresh());
			//delay_ms(5);
			r++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	
	//--------------------刷点驱动切换-----------------------
	do
	{
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		
		Lcd_Set_Driver_Mode(write_1);//写1模式(普通) 白点模式
		Lcd_Clear_GRAM();
		
		//填充
		Lcd_Fill_RBox(0,0, SCREEN_WIDTH-1, SCREEN_HIGH-1,8);//倒圆方框

		//挖空
	  Lcd_Set_Driver_Mode(write_0);//写0模式(普通) 黑点模式
		Lcd_Fill_RBox(10,2, 50, 30,2);//倒圆方框
		
		//反写文字
	  Lcd_Set_Driver_Mode(write_inverse);//反写模式(普通)
		Lcd_Draw_UTF8_String(10,1,"Fill_RBox_ABCDEFGHIJK");

	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(800);
	
	//恢复正常的刷点模式
	Lcd_Set_Driver_Mode(write_1);//写1模式(普通) 白点模式


	//--------------------图片demo----------------------
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();
		//2.-----绘图-----
		
		Lcd_Draw_Bitmap(
											0,    //x位置
											0,    //y位置
											128,  //图片宽
											64,   //图片高
		                  (uint8_t*)demo_bitmap_128x64 //图片数组
		                 );
		
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	#undef DEMO_DELAY_MS
}


#else
void gui_2bit_demo()//单色刷图demo
{
	
#	ifdef LCD_USE_RGB565
		//RGB565屏幕在程序运行过程中允许修改显存显示颜色,2位色允许同时显示4种颜色
		RGB_Set_Driver_Colour(0,COLOUR_MLIST_DEFAULT_0);//设置Lcd_Set_Driver_Mode(write_0)的笔刷颜色
		RGB_Set_Driver_Colour(1,COLOUR_MLIST_DEFAULT_1);//设置Lcd_Set_Driver_Mode(write_1)的笔刷颜色
		RGB_Set_Driver_Colour(2,COLOUR_MLIST_DEFAULT_2);//设置Lcd_Set_Driver_Mode(write_2)的笔刷颜色
		RGB_Set_Driver_Colour(3,COLOUR_MLIST_DEFAULT_3);//设置Lcd_Set_Driver_Mode(write_3)的笔刷颜色
#	endif
	
	
	#define DEMO_DELAY_MS 250
	
	//--------------文字显示字体1-----------------
	lcd_driver.fonts_ASCII = &ascii_SongTi_8X16;//ASCII字体
	lcd_driver.fonts_UTF8_cut = &SongTi_UTF8_16X16;//UTF8字体(裁切)
	lcd_driver.newline_high = lcd_driver.fonts_UTF8_cut->high;//更新\n换行间距

	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n8x16\n16x16");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色设为LCD_COLOUR_2
		Lcd_Draw_UTF8_String(0,0,"2/3 Fonts 中文\n8x16\n16x16");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色设为LCD_COLOUR_3
		Lcd_Draw_UTF8_String(0,0,"3/3 Fonts 中文\n8x16\n16x16");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	
	//--------------文字显示字体2-----------------
	lcd_driver.fonts_ASCII = &ascii_SongTi_6X12;//ASCII字体
	lcd_driver.fonts_UTF8_cut = &SongTi_UTF8_12X12;//UTF8字体(裁切)
	lcd_driver.newline_high = lcd_driver.fonts_UTF8_cut->high;//更新\n换行间距
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色设为LCD_COLOUR_2
		Lcd_Draw_UTF8_String(4,4,"2/3 Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色设为LCD_COLOUR_2
		Lcd_Draw_UTF8_String(4,4,"2/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(8,8,"3/3 Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色设为LCD_COLOUR_2
		Lcd_Draw_UTF8_String(4,4,"2/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(8,8,"3/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(12,12,"1/3 Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色设为LCD_COLOUR_2
		Lcd_Draw_UTF8_String(4,4,"2/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(8,8,"3/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(12,12,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(16,16,"2/3 Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	delay_ms(DEMO_DELAY_MS);
	delay_ms(DEMO_DELAY_MS);
	
	//------------修改默认笔刷颜色------------
#	ifdef LCD_USE_RGB565
		RGB_Set_Driver_Colour(0,COLOUR_MLIST_DEFAULT_0);
		RGB_Set_Driver_Colour(1,COLOUR_MLIST_DEFAULT_1);
		RGB_Set_Driver_Colour(2,COLOUR_MLIST_DEFAULT_2);
		RGB_Set_Driver_Colour(3,COLOUR_MLIST_DEFAULT_3);
#	endif
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---清空缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0(快速)
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(0,0,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色设为LCD_COLOUR_2
		Lcd_Draw_UTF8_String(4,4,"2/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(8,8,"3/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(12,12,"1/3 Fonts 中文\n6x12\12x12");
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色设为LCD_COLOUR_1
		Lcd_Draw_UTF8_String(16,16,"2/3 Fonts 中文\n6x12\12x12");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	delay_ms(DEMO_DELAY_MS);
	delay_ms(DEMO_DELAY_MS);
	
	//--------------全屏颜色-----------------	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---填充缓存---
		Lcd_Clear_GRAM();//全屏刷为LCD_COLOUR_0
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_inverse);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(10,8,"COLOUR 0/3");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---填充缓存---
		Lcd_Fill_GRAM(1);//全屏刷为LCD_COLOUR_1
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_inverse);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(10,8,"COLOUR 1/3");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
		do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---填充缓存---
		Lcd_Fill_GRAM(2);//全屏刷为LCD_COLOUR_2
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_inverse);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(10,8,"COLOUR 2/3");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		//1.---填充缓存---
		Lcd_Fill_GRAM(3);//全屏刷为LCD_COLOUR_3
		//2.-----绘图-----
		Lcd_Set_Driver_Mode(write_inverse);//笔刷颜色设为反色
		Lcd_Draw_UTF8_String(10,8,"COLOUR 3/3");
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);
	
	

	//--------------------画线-----------------------
	{
		uint16_t x=0;
		lcd_driver_mode_t i1,i2;
		while(x <= SCREEN_WIDTH-1)
		{
			do
			{
				
				//0.---等待发送完毕---
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				//1.---清空缓存---
				Lcd_Clear_GRAM();
				//2.-----绘图-----
				switch(x%32)
				{
					case 0://i=write_0;break;//跟背景色重复
					case 8:i1=write_1;break;//笔刷颜色1
					case 16:i1=write_2;break;//笔刷颜色2
					case 24:i1=write_3;break;//笔刷颜色3
				}
				Lcd_Set_Driver_Mode(i1);//笔刷颜色
				Lcd_Draw_Line(0,0,x,SCREEN_HIGH-1);
				switch((x+8)%32)
				{
					case 0://i=write_0;break;//跟背景色重复
					case 8:i2=write_1;break;//笔刷颜色1
					case 16:i2=write_2;break;//笔刷颜色2
					case 24:i2=write_3;break;//笔刷颜色3
				}
				Lcd_Set_Driver_Mode(i2);//笔刷颜色
				Lcd_Draw_Line(SCREEN_WIDTH-1,0,SCREEN_WIDTH-1-x,SCREEN_HIGH-1);
				
			}while(LCD_Refresh());//直到每行都刷完
			x++;
		}
	}
	
	{
		lcd_driver_mode_t i;
		uint16_t y=0;
		while(y <= SCREEN_HIGH-1)
		{
			do
			{
				//0.---等待发送完毕---
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				//1.---清空缓存---
				Lcd_Clear_GRAM();
				//2.-----绘图-----
				switch(y%32)
				{
					case 0://i=write_0;break;//跟背景色重复
					case 8:i=write_1;break;//笔刷颜色1
					case 16:i=write_2;break;//笔刷颜色2
					case 24:i=write_3;break;//笔刷颜色3
				}
				Lcd_Set_Driver_Mode(i);//笔刷颜色
				Lcd_Draw_Line(0,0,SCREEN_WIDTH-1,SCREEN_HIGH-y);
				Lcd_Draw_Line(SCREEN_WIDTH-1,0,0,SCREEN_HIGH-y);
				
			}while(LCD_Refresh());//直到每行都刷完
			y++;
		}
	}
	//--------------------画圆-----------------------
	{
		uint8_t i=0;
		uint8_t C[]=
		{
			C_RU,//右上八分之一圆
			C_UR,//上右八分之一圆
			C_UL,//上左八分之一圆
			C_LU,//左上八分之一圆
			C_LD,//左下八分之一圆
			C_DL,//下左八分之一圆
			C_DR,//下右八分之一圆
			C_RD,//右下八分之一圆
			
			C_QRU,//右上角四分圆
			C_QLU,//左上角四分圆
			C_QLD,//左下角四分圆
			C_QRD,//右下角四分圆
			
			C_HR,//右半圆
			C_HU,//上半圆
			C_HL,//左半圆
			C_HD,//下半圆
			
			C_HR|C_UL,//右半圆+上左 (任意组合)
			C_HU|C_LD,//上半圆+左下
			C_HL|C_DR,//左半圆+下右
			C_HD|C_RU,//下半圆+右上

			C_ALL,//完整的园
		};
		while(i<=(sizeof(C)/sizeof(uint8_t)-1))
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Set_Driver_Mode(write_3);//笔刷颜色
				Lcd_Draw_Circel_part(32,20,16,(circle_part_t)C[i]);
				Lcd_Fill_Circel_part(SCREEN_WIDTH-32,16,16,(circle_part_t)C[i]);
			}while(LCD_Refresh());//直到每行都刷完
			delay_ms(50);
			i++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	
	//--------------------矩形-----------------------
	{
		uint16_t i=0;
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色
		while((i<SCREEN_HIGH) || (i<SCREEN_WIDTH))
		{
			uint16_t x,y;
			if(i < SCREEN_WIDTH){x=i;}else{x=SCREEN_WIDTH-1;}
			if(i < SCREEN_HIGH){y=i;}else{y=SCREEN_HIGH-1;}
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Draw_Box(0,0, x, y);
			}while(LCD_Refresh());
			i++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	{
		uint16_t i=0;
		Lcd_Set_Driver_Mode(write_2);//笔刷颜色
		while((i<SCREEN_HIGH) || (i<SCREEN_WIDTH))
		{
			uint16_t x,y;
			if(i < SCREEN_WIDTH){x=i;}else{x=SCREEN_WIDTH-1;}
			if(i < SCREEN_HIGH){y=i;}else{y=SCREEN_HIGH-1;}
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Fill_Box(0,0, x, y);
			}while(LCD_Refresh());
			i++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
//	
//	//--------------------圆角矩形-----------------------
	{
		Lcd_Set_Driver_Mode(write_1);//笔刷颜色
		uint16_t r=0;
		while((r<SCREEN_HIGH)&&(r<SCREEN_WIDTH))
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Draw_RBox(0,0,SCREEN_WIDTH-1, SCREEN_HIGH-1,r);
			}while(LCD_Refresh());
			r++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	{
		Lcd_Set_Driver_Mode(write_3);//笔刷颜色
		uint16_t r=0;
		while((r<SCREEN_HIGH)&&(r<SCREEN_WIDTH))
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Fill_RBox(0,0,SCREEN_WIDTH-1, SCREEN_HIGH-1,r);
			}while(LCD_Refresh());
			r++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
//	//--------------------十进制数字-----------------------
	{
		
		int16_t r=-100;
		while(r<100)
		{
			do
			{
				while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
				Lcd_Clear_GRAM();
				Lcd_Set_Driver_Mode(write_1);//笔刷颜色
				Lcd_Draw_int32(0,0,r);//十进制带符号
				Lcd_Set_Driver_Mode(write_2);//笔刷颜色
				Lcd_Draw_int32(0,12,r);//十进制带符号
				Lcd_Set_Driver_Mode(write_3);//笔刷颜色
				Lcd_Draw_int32(0,24,r);//十进制带符号
			}while(LCD_Refresh());
			//delay_ms(5);
			r++;
		}
	}
	delay_ms(DEMO_DELAY_MS);
	
	//--------------------刷点驱动切换-----------------------
	do
	{
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		
		Lcd_Set_Driver_Mode(write_1);//写1模式(普通) 白点模式
		Lcd_Clear_GRAM();
		
		//填充
		Lcd_Fill_RBox(0,0, SCREEN_WIDTH-1, SCREEN_HIGH-1,8);//倒圆方框

		//挖空
	  Lcd_Set_Driver_Mode(write_0);//写0模式(普通) 黑点模式
		Lcd_Fill_RBox(10,2, 50, 30,2);//倒圆方框
		
		//反写文字
	  Lcd_Set_Driver_Mode(write_inverse);//反写模式(普通)
		Lcd_Draw_UTF8_String(10,1,"Fill_RBox_ABCDEFGHIJK");

	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(800);
	
	//恢复正常的刷点模式
	Lcd_Set_Driver_Mode(write_1);//写1模式(普通) 白点模式


	//--------------------图片demo----------------------
	do
	{
		//0.---等待发送完毕---
		while(LCD_is_Busy()!=0);//DMA方式专用,其他模式可省略
		
		//1.---清空缓存---
		Lcd_Clear_GRAM();
		//2.-----绘图-----
		
		Lcd_Draw_Bitmap(
											0,    //x位置
											0,    //y位置
											128,  //图片宽
											64,   //图片高
		                  (uint8_t*)demo_bitmap_128x64 //图片数组
		                 );
		
	}while(LCD_Refresh());//直到每行都刷完
	delay_ms(DEMO_DELAY_MS);

	#undef DEMO_DELAY_MS
}
#endif


/*--------------------------------------------------------------
  * 本工程请使用UTF8文本编码
  * 修改lcd_driver_config.h和lcd_wegui_config.h即可快速上手点屏
----------------------------------------------------------------*/

int main(void)
{
	startup_init();
	
	delay_ms(100);//等待电源稳定
	
	//本框架"Driver"部分为高效率点阵OLED/RGB驱动,可单独移植使用移植,demo程序gui_1bit_demo();/gui_2bit_demo();
	lcd_driver_Init();
	
	//本框架Wegui部分为多级菜单图形动画ui,处理多级菜单,过度动画等
	lcd_wegui_init();

	sys1ms_stick = 0;
	while (1)
	{
		//--------------1.刷图DEMO---------------

		#if (LCD_COLOUR_BIT == 1)
			//gui_1bit_demo();//1位色刷图demo
		#else
			//gui_2bit_demo();//2位色刷图demo
		#endif
		
		//------------2.多级菜单DEMO-------------
		Wegui_loop_func();//Wegui循环驱动
		
		//--------------主循环-----------------
		if(sys1ms_stick)//1ms动作
		{
			//1.主题时钟APP demo用
			apptimer_ms+=sys1ms_stick;
			//2.闪灯 若程序阻塞,灯会闪变慢
			LED_Func();//暂不移植
			//3.检测外部可调电阻ADC,取其值用于控件菜单演示
			if(adc_en)
			{
				//ResADC值自增,显示到菜单
				if(ResADC < 4095){ResADC++;}
				else{ResADC = 0;}
				m_wDemo_wMessage_ADC_func();//刷新值转成字符串放到菜单
			}
			sys1ms_stick=0;
		}
	}
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
 /**
   * @brief  Reports the name of the source file and the source line number
   *         where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	while(1);
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif
