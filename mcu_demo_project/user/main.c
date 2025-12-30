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
#include "stm32f10x.h"
#include "stm32f10x_it.h"

//#include "lcd_driver.h"
//#include "lcd_wegui_driver.h"
//#include "wegui_menu_demo.h"
//#include "driver_demo.h" 

//------------------------------------1.直驱driver移植帮助---------------------------------------

//1.1.包含lcd_driver.h文件
#error   ("1.1. Include 'lcd_driver.h' file");
//#include "lcd_driver.h"

//1.2执行lcd_driver_init();初始化驱动
#error   ("1.2. Use 'lcd_driver_init()' Initialize driver");

//1.3.包含driver_demo.h文件
#error   ("1.3. Include 'driver_demo.h' file");
//#include "driver_demo.h"

//1.4在main while中调用driver_demo()函数查看移植效果
#error   ("1.4.Call the 'driver_demo()' in main while");

//1.5.编辑'lcd_driver_config.h'
#error   ("1.5. Edid 'lcd_driver_config.h' file");

//1.6.使用lcd_port_template.c文件完成屏幕接口移植
#error   ("1.6. Complete lcd port using 'lcd_port_template.c' file");

//1.7编译下载测试是否有正常显示图形
#error   ("1.7. Build and test");

//------------------------------------1.直驱driver移植帮助---------------------------------------


//-------------------------------------2.多级菜单移植帮助---------------------------------------
////2.0完成1.1~1.7的移植和测试
//#error   ("2.0.Complete 1.1~1.7");

//////2.1.编辑'lcd_wegui_config.h'
//#error   ("2.1. Edid 'lcd_wegui_config.h' file");

////2.2.包含lcd_wegui_driver.h驱动文件
//#error   ("2.2.Include 'lcd_wegui_driver.h' file");
////#include "lcd_wegui_driver.h" 

////2.3.wegui_menu_demo.h文件
//#error   ("2.3.Include 'wegui_menu_demo.h' file");
////#include "wegui_menu_demo.h"

////2.4.请初始化一个1ms滴答中断
//#error   ("2.4.Initialize 1ms stick interrupt");

////2.5.将wegui_1ms_stick()函数放到1ms滴答中断里
//#error   ("2.5.Put the 'wegui_1ms_stick()' into the 1ms stick interrupt func");

////2.6执行lcd_wegui_init()初始化多级菜单demo驱动
//#error   ("2.6.Use 'lcd_wegui_init()' Initialize driver");

////2.7执行lcd_wegui_init()初始化多级菜单demo驱动 可以从wegui_menu_demo.h中找到m_min菜单
//#error   ("2.7.Use 'wegui.menu=&m_min;' Initialize a startup menu");
////wegui.menu = &m_main;

////2.8.main while中调用wegui_loop_func()函数
//#error ("2.7.Call the 'wegui_loop_func()' in main while");

////2.9编译下载
//#error ("2.9 Build and test");
//-------------------------------------2.多级菜单移植帮助---------------------------------------



void startup_init(void)
{
	//tip 2.3.初始化一个1ms滴答中断 例:
	//SysTick_Config(SystemCoreClock / 1000);
}

////tip 2.4.将该函数放到1ms滴答中断里 例:
//void SysTick_Handler(void)
//{
//	wegui_1ms_stick();//tip 2.4
//}

/*--------------------------------------------------------------
  * 修改lcd_driver_config.h和lcd_wegui_config.h即可快速上手点屏
----------------------------------------------------------------*/
int main(void)
{
	//自定义开机初始化
	startup_init();
	
	//本框架"driver"部分为高效率点阵OLED/RGB驱动,可单独移植使用移植
	//lcd_driver_init();//tip:1.2
	
	//本框架wegui部分为多级菜单图形动画ui,处理多级菜单,过度动画等,
	//lcd_wegui_init();//tip:2.3
	//wegui.menu = &m_main;//开机初始菜单menu
	
	while (1)
	{
		//------------1.直驱刷图DEMO-------------
		//driver_demo();//driver直驱demo tip:1.4
		
		//------------2.多级菜单DEMO-------------
		//wegui_loop_func();//wegui主循环 tip:2.4
	}
}
