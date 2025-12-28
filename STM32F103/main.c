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
#include "main.h"
#include "driver_demo.h"

void delay_ms(uint32_t ms)
{
	sys1ms_delay = ms;
	while(sys1ms_delay);
}

void startup_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//--释放JTAG 接口作为GPIO--
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	//--板载指示灯--
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	
	//--系统滴答1ms定时--
	sys1ms_stick = 0;
	SysTick_Config(SystemCoreClock / 1000);
}


//--板载指示灯--
void led_func()
{
	if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)!=0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
	else
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
}

void debug()
{
	wegui_uart_tx_1dat(0x55);
	wegui_uart_tx_1dat(0xAA);
}

void wegui_hello_word()
{
	char* string = "Hello WeGui!";
	wegui_push_message_tip(2, string, 8000);//推送提示信息, (推送y位置, 提示内容字符串, 展示时间ms)
}


///*--------------------------------------------------------------
//  * 名称: flash_draw_IMG_RGB565_ZIP_ORLE2(uint16_t x, uint16_t y, uint32_t flash_addr) 
//  * 传入1:x输出坐标
//  * 传入2:y输出坐标
//  * 传入3:flash_addr图片数据的flash地址
//  * 功能: 从FLASH读取RGB565_ZIP_ORLE2格式图片投放到屏幕坐标(x,y)上
//  * 说明: 该源函数是weak类型, 兼容性好但效率不高, 改写自flash_driver.c
//----------------------------------------------------------------*/
//#if ((LCD_PORT == _HARD_4SPI) && (LCD_TYPE == LCD_RGB565))
//void flash_draw_IMG_RGB565_ZIP_ORLE2(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, uint32_t flash_addr)
//{
//		#define IMG_FLASH_SIZE 512 //设置读取缓冲大小

//		//#if (FLASH_PORT != _F_DMA_STDSPI)//FLASH和LCD使用串行快速读写
//		#if (1)
//		{
//			uint8_t flash_dat_buff[IMG_FLASH_SIZE];
//			uint16_t i = 0;
//			uint8_t *p;
//			uint8_t dat[3];
//			lcd_set_addr(x, y, x + sizex - 1, y + sizey - 1);
//			while (1) 
//			{
//					if (i <= 0) 
//					{
//							flash_read_addr_ndat(flash_addr, flash_dat_buff, (IMG_FLASH_SIZE / 3 * 3));
//							flash_addr += (IMG_FLASH_SIZE / 3 * 3);
//							p = & flash_dat_buff[0];
//							i = (IMG_FLASH_SIZE / 3);
//					}
//					i--;
//					dat[0] = * p++;
//					if (dat[0] == 0x00) {break;} 
//					dat[1] = * p++;
//					dat[2] = * p++;
//					LCD_DC_Set();
//					LCD_CS_Clr();
//					while (dat[0]--) 
//					{
//						wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
//						send_lcd_spi_dat(dat[1]);//向SPI发送缓冲器发送一个数据
//						wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
//						send_lcd_spi_dat(dat[2]);//向SPI发送缓冲器发送一个数据
//					}
//					wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
//					send_lcd_spi_done();//等待SPI发送器空闲(发完)
//					LCD_CS_Set();
//			}
//		}
//		#else //DMA FLASH和LCD并行快速读写
//		{
//			
//		}
//		#endif
//		#undef IMG_FLASH_SIZE
//}
//#endif

/*--------------------------------------------------------------
  * 修改lcd_driver_config.h和lcd_wegui_config.h即可快速上手点屏
----------------------------------------------------------------*/
int main(void)
{
	startup_init();
	
	//本框架"driver"部分为高效率点阵OLED/RGB驱动,可单独移植使用移植
	lcd_driver_Init();//demo程序driver_demo();
	
	//本框架wegui部分为多级菜单图形动画ui,处理多级菜单,过度动画等,
	//lcd_wegui_init();//demo程序wegui_loop_func();
	
	//wegui_hello_word();//开机欢迎弹窗(菜单demo)
	sys1ms_stick = 0;
	while (1)
	{
		//------------1.直驱刷图DEMO-------------
		driver_demo();
		
		//------------2.多级菜单DEMO-------------
		//wegui_loop_func();//wegui主循环

		//-----------3.自定义测试----------
		//if(sys1ms_stick>=100)//1ms动作
		//{
		//	//debug();
		//	//1.闪灯 若程序阻塞,灯会闪变慢
		//	led_func();
		//	//2.demo_bool使能演示,在demo菜单里修改
		//	if(demo_bool)
		//	{
		//		//demo_value自增,在demo菜单里显示
		//		if(demo_value < 4095){demo_value++;}
		//		else{demo_value = 0;}
		//	}
		//	sys1ms_stick-=100;
		//}
	}
}
