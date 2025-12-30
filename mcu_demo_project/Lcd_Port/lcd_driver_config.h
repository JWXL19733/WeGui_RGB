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
#ifndef LCD_DRIVER_CONFIG_H
#define LCD_DRIVER_CONFIG_H

/*--------------------------------------------------------------
  * wegui : V0.5
  * Author: KOUFU
  * https://space.bilibili.com/526926544
  * https://github.com/KOUFU-DIY/WeGui_RGB
----------------------------------------------------------------*/

/*--------------------------------------------------------------
  * 版本更新 : WeGui RGB V0.5.7
  * 1.修改移植接口,现接口更简易更简单,性能降低但可移植性提高
	* 2.新增RLE压缩bitmap刷图驱动,gui支持使用RLE压缩图形,用法参考driver_demo();暂时使用LCD_mcuTool取模,后续集成到上位机上
	* 3.修改了字体取模,后续支持外挂字体,旧版本字体失效,需要更新上位机重新取
	* 4.优化了bitmap驱动,提升了帧率
----------------------------------------------------------------*/

/*--------------------------------------------------------------
  * 本程序使用了大量的"UTF8"支持多国语言的编码
  * 1.Edit->Configuration->Encoding->"Encod in UTF8"
	* 若使用v5版本编译器需要增加编译设置(v6无需请勿添加)
  * 2.Project -> Oprions for Target-> C/C++ -> 
  * Misc Contiols -> 填入"--no-multibyte-chars"
----------------------------------------------------------------*/

//-------------------------1.5.1选择一个屏幕通讯接口-----------------------------
#define _LCD_NO_PORT    (0)
#define _LCD_PORT_DEMO  (1)//屏幕demo移植接口
#define LCD_PORT        _LCD_PORT_DEMO //选择一个屏幕接口

#if (LCD_PORT == _LCD_PORT_DEMO)
	#include "lcd_port_template.h"//demo头文件
#endif

//----------------------------1.5.2设定屏幕分辨率--------------------------------
#define SCREEN_WIDTH 128  //建议取8的倍数
#define SCREEN_HIGH  64  //建议取8的倍数

//-----------------------1.5.3设定屏幕区域显示偏移设置--------------------------
#define SCREEN_X_OFFSET 0 //x左右方向偏移像素
#define SCREEN_Y_OFFSET 0 //y上下方向偏移像素

//---------------------------1.5.4选择刷屏方式--------------------------------
#define _FULL_BUFF_FULL_UPDATE (0) //全屏缓存 全屏刷新(建议测试性能用)
#define _FULL_BUFF_DYNA_UPDATE (1) //全屏缓存 动态刷新(最高的刷新速度,建议使用)
#define _PAGE_BUFF_FULL_UPDATE (2) //页缓存   全屏刷新(更低的内存占用,测试用)
#define _PAGE_BUFF_DYNA_UPDATE (3) //页缓存   动态刷新(更低的内存占用,建议使用)
#define LCD_MODE    _FULL_BUFF_FULL_UPDATE //选择一个刷屏模式

//-------------------------1.5.5设置择刷屏缓存大小--------------------------------
#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)) 
	#define GRAM_YPAGE_NUM ((SCREEN_HIGH+7)/8)//全屏缓存,固定大小 禁止修改
#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	#define GRAM_YPAGE_NUM (1)//页缓存 自定义大小 最小取1 最大取(((SCREEN_HIGH+7)/8)) 根据ram占用来平衡性能与占用
#endif

//-------------------------1.5.6设置动态刷新页细分数量--------------------------------
//每页连续横向扫描n个像素校验一个crc
#if ((LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)||(LCD_MODE == _FULL_BUFF_DYNA_UPDATE))
	#define PAGE_CRC_NUM  (8)     //动态刷新每页细分校验的分区 默认1~8即可
#endif

//-------------------------1.5.7设置单片机开机字体--------------------------------
//&mcu_fonts_ascii_songti_6X12;
//&mcu_fonts_ascii_songti_8X16;
//&mcu_fonts_ascii_songti_12X24;
#define STARTUP_FONTS_ASCII (&mcu_fonts_ascii_songti_6X12)//开机ascii字体 建议两者高度一致

//&mcu_fonts_utf8_songti_12X12;
//&mcu_fonts_utf8_songti_16X16;
//&mcu_fonts_utf8_songti_24X24;
#define STARTUP_FONTS_UTF8  (&mcu_fonts_utf8_songti_12X12)//开机utf8字体 建议两者高度一致

//-------------------------1.5.8选择一个外挂FLASH接口--------------------------------
//请勿与LCD接口冲突
#define _FLASH_NO_PORT    (0)//关闭FLASH接口
#define _FLASH_PORT_DEMO  (1)//对应flash_port_template.c
#define FLASH_PORT        _FLASH_NO_PORT//选择一个外挂FLASH接口

#if (FLASH_PORT == _FLASH_PORT_DEMO)
	#include "flash_port_template.h"//demo头文件
#endif

//----------------------1.5.9选择一个外挂FLASH型号内置驱动-----------------------------
#define _FLASH_NO_IC   (0)//无
#define _FLASH_W25Qxx  (1)//W25Qxx
#define FLASH_IC     _FLASH_NO_IC//选择一个外挂FLASH型号

#if (FLASH_IC == _FLASH_W25Qxx)
	#include "w25qxx.h"
	#define flash_ic_init()                  do{w25qxx_init();}while(0)
	#define flash_read_addr_ndat(addr,p,len) do{w25qxx_read_data(addr,p,len);}while(0)
#endif

//-------------------------1.5.10选择内置的屏幕驱动IC-----------------------------
#define _SH1106    (1)//普通点阵OLED
#define _SH1108    (2)//普通点阵OLED
#define _SH1107    (3)//普通点阵OLED
#define _SH1115    (4)//普通点阵OLED
#define _SSD1306   (5)//普通点阵OLED (移植简单)
#define _SSD1309   (6)//普通点阵OLED 
#define _SSD1312   (7)//普通点阵OLED
#define _SSD1315   (8)//普通点阵OLED
//#define _SSD1327   (9)//灰度OLED (暂时取消)
#define _ST7735   (10)//TFT彩屏 RGB565 (移植简单)
#define _ST7789V3 (11)//TFT彩屏 RGB565
#define _ST7789VW (12)//TFT彩屏 RGB565
#define _ST7796S  (13)//TFT彩屏 RGB565
#define _GC9A01   (14)//TFT彩屏 RGB565
#define LCD_IC    _ST7735 //选择一个屏幕IC型号

//-----------------------1.5.11选择屏幕IC对应的屏幕类型---------------------------
#define LCD_UNKNOW (0) //未知
#define LCD_OLED   (1) //普通点阵OLED
#define LCD_GRAY   (2) //灰度OLED
#define LCD_RGB565 (3) //TFT彩屏 RGB565
#define LCD_TYPE    LCD_RGB565 //选择一个屏幕类型

//-----------------------1.5.12设置色彩深度---------------------------
#if ((LCD_TYPE == LCD_GRAY) || (LCD_TYPE == LCD_OLED))
	#define LCD_COLOUR_BIT (1) //不是彩屏 固定1位色 不可修改
#elif(LCD_TYPE == LCD_RGB565)
	//彩屏
	//支持1位(0~1,共2色) //同一界面允许同时显示1位色(2种颜色) 0B   1B
	//支持2位(0~3,共4色) //同一界面允许同时显示2位色(4种颜色) 00B  01B  10B  11B
	//支持3位(0~7,共8色) //同一界面允许同时显示3位色(8种颜色) 000B 001B 010B 011B 100B 101B 110B 111B
	#define LCD_COLOUR_BIT (1) //自定义设置1~3色位
#endif


#if (LCD_IC == _SH1106)
	#define lcd_ic_init() do{SH1106_Init();}while(0)
	#define lcd_set_addr_x(x) do{SH1106_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SH1106_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SH1106_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{SH1106_Set_Contrast(x);}while(0)
	#include "sh1106.h"
#elif (LCD_IC == _SH1108)
	#define lcd_ic_init() do{SH1108_Init();}while(0)
	#define lcd_set_addr_x(x) do{SH1108_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SH1108_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SH1108_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{SH1108_Set_Contrast(x);}while(0)
	#include "sh1108.h"
#elif (LCD_IC == _SH1107)
	#define lcd_ic_init() do{SH1107_Init();}while(0)
	#define lcd_set_addr_x(x) do{SH1107_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SH1107_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SH1107_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{lcd_send_1Cmd(0x81);lcd_send_1Cmd(x);}while(0)
	#include "SH1107.h"
#elif (LCD_IC == _SH1115)
	#define lcd_ic_init() do{SH1115_Init();}while(0)
	#define lcd_set_addr_x(x) do{SH1115_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SH1115_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SH1115_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{SH1115_The_Contrast_Control_Mode_Set(x);}while(0)
	#include "sh1115.h"
#elif (LCD_IC == _SSD1306)
	#define lcd_ic_init() do{SSD1306_Init();}while(0)
	#define lcd_set_addr_x(x) do{SSD1306_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SSD1306_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SSD1306_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{lcd_send_1Cmd(0x81);lcd_send_1Cmd(x);}while(0)
	#include "ssd1306.h"
#elif (LCD_IC == _SSD1309)
	#define lcd_ic_init() do{SSD1309_Init();}while(0)
	#define lcd_set_addr_x(x) do{SSD1309_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SSD1309_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SSD1309_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{lcd_send_1Cmd(0x81);lcd_send_1Cmd(x);}while(0)
	#include "ssd1309.h"
#elif (LCD_IC == _SSD1312)
	#define lcd_ic_init() do{SSD1312_Init();}while(0)
	#define lcd_set_addr_x(x) do{SSD1312_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SSD1312_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SSD1312_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{lcd_send_1Cmd(0x81);lcd_send_1Cmd(x);}while(0)
	#include "ssd1312.h"
#elif (LCD_IC == _SSD1315)
	#define lcd_ic_init() do{SSD1315_Init();}while(0)
	#define lcd_set_addr_x(x) do{SSD1315_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_ypage(page) do{SSD1315_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define lcd_set_addr(x,page) do{SSD1315_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define lcd_set_bright(x) do{SSD1315_Set_Contrast_Control(x);}while(0)
	#include "ssd1315.h"
#elif (LCD_IC == _SSD1327)
	#define lcd_ic_init() do{SSD1327_Init();}while(0)
	#define lcd_set_addr_x(x0,x1) do{SSD1327_Set_Addr_x(x0+SCREEN_X_OFFSET,x1+SCREEN_X_OFFSET);}while(0)
	#define lcd_set_addr_y(y0,y1) do{SSD1327_Set_Addr_y(y0+SCREEN_Y_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_addr(x0,y0,x1,y1) do{SSD1327_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_bright(x) do{/*SSD1327_Set_Contrast_Control(x);*/}while(0)
	#include "ssd1327.h"
#elif (LCD_IC == _ST7735)
	#define lcd_ic_init() do{ST7735_Init();}while(0)
	#define lcd_set_addr(x0,y0,x1,y1) do{ST7735_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_bright(x) do{}while(0)
	#include "st7735.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7789V3)
	#define lcd_ic_init() do{ST7789V3_Init();}while(0)
	#define lcd_set_addr(x0,y0,x1,y1) do{ST7789V3_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_bright(x) do{}while(0)
	#include "st7789v3.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7789VW)
	#define lcd_ic_init() do{ST7789VW_Init();}while(0)
	#define lcd_set_addr(x0,y0,x1,y1) do{ST7789VW_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_bright(x) do{}while(0)
	#include "st7789vw.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7796S)
	#define lcd_ic_init() do{ST7796S_Init();}while(0)
	#define lcd_set_addr(x0,y0,x1,y1) do{ST7796S_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_bright(x) do{}while(0)
	#include "st7796s.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _GC9A01)
	#define lcd_ic_init() do{GC9A01_Init();}while(0)
	#define lcd_set_addr(x0,y0,x1,y1) do{GC9A01_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define lcd_set_bright(x) do{}while(0)
	#include "gc9a01.h"
	#include "TFT_Color.h"
#else
	#warning("No screen ic init!")
#endif



//------------编译-----------
#define DYNA_CRC_ONCE_XNUM  ((SCREEN_WIDTH+PAGE_CRC_NUM-1)/PAGE_CRC_NUM) //一次性校验的像素数量
	
#endif

	
