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
  * 版本更新 : WeGui RGB V0.5.6
  * 1.增加外挂FLASH支持
	* * 1)外挂flash下载请使用exflash_download工程
	* * 2)图片取模使用WeGui工具上位机
	* 2.增加"视频播放器APP", 在主页打开
	* 3.增加TFT彩屏主题修改, 设置->UI&主题
	* 4.开机字体更改到.h修改
	* 5.上电逻辑优化

----------------------------------------------------------------*/

/*--------------------------------------------------------------
  * 若keil版本过高,请自行安装v5版本编译器
  * 本程序使用了大量的"UTF8"支持多国语言的编码
  * 需要进行两步设置
  * 1.Edit->Configuration->Encoding->"Encod in UTF8"
  * 2.Project -> Oprions for Target-> C/C++ -> 
  * Misc Contiols -> 填入"--no-multibyte-chars"
----------------------------------------------------------------*/
//提示: 
//IIC相关功能暂不支持使用ARMV6编译,请使用ARMV5编译器!!
//软件IIC默认使用适中的延迟速率,需要更快的刷新速度,需要到对应port文件里修改, 查找I2C_SCL_Rise_Delay等并进行修改延迟值
//使用模拟IIC可精确控制上升和下降时间,调整合适的延迟时间,刷新率可以比硬件IIC更快

//该工程支持四种工作模式, 内存充足情况下建议使用(2), 内存不足建议使用(4), 工作方式在下方通过宏定义调整切换
//(1)"全缓存全屏刷新",最原始的刷新模式
//(2)"全缓存动态刷新",最快的刷新模式,全屏动画时与(1)的刷屏速度基本一致
//(3)"页缓存全屏刷新",节省内存的原始刷新模式
//(4)"页缓存动态刷新",节省内存且刷得快得模式, 全屏动画时与(3)的刷屏速度基本一致

//字库及其他图片资源在lcd_res,可自行使用配套工具修改裁剪多国语言字库和图片取模

/*------------------------------------------------------------------------------------
  * ----默认驱动接口----        ---默认按键接口---        |板载闪烁LED| C13 |
  * |--SPI--|  |--IIC--|        |UP   | B13 | 上 |        |UART_RX    |  A3 |
  * |BL   B0|  |RES  A6|        |DOWN | B14 | 下 |        |UART_TX    |  A2 |
  * |CS   A4|  |SDA B11|        |LEFT | B15 | 左 |        -----外挂flash----- 
  * |DC  B11|  |SCL B10|        |RIGHT| B12 | 右 |        |CS  | B12 |
  * |RES B10|  |3V3 3V3|        --默认编码器接口--        |DO  | B14 |
  * |SDA  A7|  |GND GND|        |  A  | A8  |A相 |        |CLK | B13 |
  * |SCL  A5|                   |  B  | A10 |B相 |        |DI  | B15 |
  * |3V3 3V3|                   |  K  | A9  |按键|        ----433/315模块----
  * |GND GND|                                             |DAT |  A0 |
-------------------------------------------------------------------------------------*/

//-------------------------1.选择一个屏幕通讯接口-----------------------------
#define _SOFT_3SPI  (0)//软件三线SPI驱动   对应文件stm32f103_lcd_soft_3spi_port.c
#define _SOFT_4SPI  (1)//软件四线SPI驱动   对应文件stm32f103_lcd_soft_4spi_port.c
#define _HARD_4SPI  (2)//硬件四线SPI驱动   对应文件stm32f103_lcd_hard_4spi_port.c
#define _DMA_4SPI   (3)//DMA四线SPI驱动   对应文件stm32f103_lcd_dma_4spi_port.c
#define _SOFT_IIC   (4)//软件IIC驱动(推荐) 对应文件stm32f103_lcd_soft_iic_port.c 
//#define _HARD_IIC   (5)//硬件IIC驱动(不推荐) 对应文件stm32f103_lcd_hard_iic_port.c (有硬件bug不建议使用)
#define LCD_PORT    _HARD_4SPI                                                                                                                             //选择一个接口

//----------------------------2.1.设定屏幕分辨率--------------------------------
#define SCREEN_WIDTH 240  //建议取8的倍数
#define SCREEN_HIGH  320  //建议取8的倍数

//-----------------------2.2.设定屏幕区域显示偏移设置--------------------------
//oled屏幕优先修改驱动初始化
#define SCREEN_X_OFFSET     0 //x左右方向偏移像素
#define SCREEN_Y_OFFSET     0 //y上下方向偏移像素

//-----------------------------2.3.选择屏幕IC---------------------------------
#define _SH1106    (0)//普通点阵OLED
#define _SH1108    (1)//普通点阵OLED
#define _SH1107    (2)//普通点阵OLED
#define _SH1115    (3)//普通点阵OLED
#define _SSD1306   (4)//普通点阵OLED (最常用)
#define _SSD1309   (5)//普通点阵OLED 
#define _SSD1312   (6)//普通点阵OLED
#define _SSD1315   (7)//普通点阵OLED
#define _SSD1327   (8)//灰度OLED
#define _ST7735    (9)//TFT彩屏 RGB565
#define _ST7789V3 (10)//TFT彩屏 RGB565
#define _ST7789VW (11)//TFT彩屏 RGB565
#define _ST7796S  (12)//TFT彩屏 RGB565
#define _GC9A01   (13)//TFT彩屏 RGB565
#define LCD_IC    _ST7789V3 //选择一个屏幕IC型号

//---------------------------3.1.设定I2C驱动--------------------------------
#if ((LCD_PORT == _HARD_IIC) || (LCD_PORT == _SOFT_IIC))
	#define OLED_IIC_7ADDR 0x3C //7位0x3C => 8位0x78 (大部分默认)
	//#define OLED_IIC_7ADDR 0x3D //7位0x3D => 8位0x7A
#endif 

//-------------------------3.2.设定硬件SPI驱动-------------------------------
#if ((LCD_PORT == _HARD_4SPI) || (LCD_PORT == _DMA_4SPI))
//STM32F103手册指定SPI最高设置18MHz 但也支持超频
#define RCC_HCLK_Divx            RCC_HCLK_Div1 //HCLK时钟分频1,2,4,8,16
#define LCD_SPI_BaudRatePrescaler_2  SPI_BaudRatePrescaler_2 //SPI分频2,4,8,16,32,64,128,256
#endif 

//---------------------------4.1.选择刷屏方式--------------------------------
#define _FULL_BUFF_FULL_UPDATE (0) //全屏缓存 全屏刷新(建议测试性能用)
#define _FULL_BUFF_DYNA_UPDATE (1) //全屏缓存 动态刷新(最高的刷新速度,建议使用)
#define _PAGE_BUFF_FULL_UPDATE (2) //页缓存 全屏刷新(更低的内存占用,测试用)
#define _PAGE_BUFF_DYNA_UPDATE (3) //页缓存 动态刷新(更低的内存占用,建议使用)
#define LCD_MODE    _PAGE_BUFF_DYNA_UPDATE //选择一个刷屏模式

//-------------------------4.2.设置择刷屏缓存页大小--------------------------------
//仅页缓存模式需要设置 全屏缓存此设置无效
#if ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	#define GRAM_YPAGE_NUM (4)//自定义 最小取1 最大取(((SCREEN_HIGH+7)/8)) 根据ram占用来平衡性能与占用
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+1)/2)//设置二分之一屏缓存
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+3)/4)//设置四分之一屏缓存
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+7)/8)//设置八分之一屏缓存
	//#define GRAM_YPAGE_NUM (((SCREEN_HIGH+7)/8))//设置全屏缓存(请直接使用_FULL_BUFF_DYNA_UPDATE模式获取更高的性能)
#endif

//-------------------------4.3.设置彩屏动态刷新页细分数量--------------------------------
//仅适配了TFT彩屏 //每页连续横向扫描PAGE_CRC_NUM个像素校验一个crc
#if ((LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)||(LCD_MODE == _FULL_BUFF_DYNA_UPDATE))
	#define PAGE_CRC_NUM  (8)     //动态刷新每页细分校验的分区 默认1 仅TFT可用,其他类型屏幕默认取1
#endif

//---------------------------5.设定灰度屏--------------------------------
//仅"灰度OLED屏"需要设置
//注意目前驱动限制,灰度屏幕宽高须为8的倍数,否则屏幕可能会有溢出

//1.选择一个灰度屏扫描方向[需要与初始化(A0指令)匹配]
//#define GRAY_DRIVER_0DEG  //方向1
#define GRAY_DRIVER_90DEG //方向2

//2.设置画笔灰度亮度[1:15]
#define GRAY_COLOUR  15 //画笔灰度(亮度)

//-------------------------6.1.选择一个外挂FLASH接口--------------------------------
//请勿与LCD接口冲突(默认不共用一个spi,不冲突)
#define _F_NO_PORT      (0)//没有外挂FLASH接口 
#define _F_SOFT_STDSPI  (1)//软件标准SPI
#define _F_HARD_STDSPI  (2)//硬件标准SPI
#define _F_DMA_STDSPI   (3)//DMA标准SPI
#define FLASH_PORT      _F_DMA_STDSPI//选择一个外挂FLASH接口

//-------------------------6.2.选择一个外挂FLASH型号--------------------------------
#define _FLASH_NONE    (0)//没有FLASH
#define _FLASH_W25Qxx  (1)//W25Qxx
#define FLASH_MODEL     _FLASH_W25Qxx//选择一个外挂FLASH型号

//-------------------------6.3.配置硬件FLASH参数--------------------------------
//#define RCC_HCLK_Divx            RCC_HCLK_Div2 //HCLK时钟分频1,2,4,8,16 (与LCD共用)
#define FLASH_SPI_BaudRatePrescaler_x SPI_BaudRatePrescaler_2 //SPI分频2,4,8,16,32,64,128,256



//-------------------------7.默认字体设置--------------------------------
//extern const fonts_t fonts_ascii_songti_6X12;
//extern const fonts_t fonts_ascii_songti_8X16;
//extern const fonts_t fonts_ascii_songti_12X24;
#define STARTUP_FONTS_ASCII (fonts_ascii_songti_8X16)//开机ascii字体 建议两者高度一致

//extern const fonts_t fonts_utf8_songti_12X12;
//extern const fonts_t fonts_utf8_songti_16X16;
//extern const fonts_t fonts_utf8_songti_24X24;
#define STARTUP_FONTS_UTF8  (fonts_utf8_songti_16X16)//开机utf8字体 建议两者高度一致



//-------------------------8.函数RAM零等待加速--------------------------------
//使用该功能需要修改sct文件, 将*.o(RAMCODE)放进RAM区,编译才有效
//经过"RAMCODE"修饰的代码, 将会放到RAM运行, 因此会占用RAM空间
//实测stm32f103打开后加速不明显, 因此已无需再打开ram加速
//此处保留, 作为移植使用
//打开宏编译即使用加速,注释掉则不加速(默认不加速)
//#define RAM_SPEEDUP_FUNC_0 __attribute__((section ("RAMCODE"))) //LCD_Refresh相关函数的RAM加速
//#define RAM_SPEEDUP_FUNC_1 __attribute__((section ("RAMCODE"))) //1位色相关函数RAM加速
//#define RAM_SPEEDUP_FUNC_2 __attribute__((section ("RAMCODE"))) //2位色相关函数RAM加速
//#define RAM_SPEEDUP_FUNC_3 __attribute__((section ("RAMCODE"))) //3位色相关函数RAM加速


//------------编译-----------
//全屏缓存,固定大小
#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)) 
	#define GRAM_YPAGE_NUM ((SCREEN_HIGH+7)/8)
//页缓存,判断页大小
#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	#if (GRAM_YPAGE_NUM >= ((SCREEN_HIGH+7)/8))
		#warning("Buff enough!")
		//显存过剩, 请在上方调小 GRAM_YPAGE_NUM 
		//或者 LCD_MODE 使用全屏刷新 _FULL_BUFF_DYNA_UPDATE
	#endif
#endif


#if (LCD_PORT == _SOFT_3SPI)    //软件三线SPI
	#include "stm32f103_lcd_soft_3spi_port.h"
#elif (LCD_PORT == _SOFT_4SPI) //软件四线SPI 
	#include "stm32f103_lcd_soft_4spi_port.h"
#elif (LCD_PORT == _HARD_4SPI) //硬件四线SPI 
	#include "stm32f103_lcd_hard_4spi_port.h"
#elif (LCD_PORT == _DMA_4SPI) //DMA四线SPI 
	#include "stm32f103_lcd_dma_4spi_port.h"
#elif (LCD_PORT == _SOFT_IIC)  //软件IIC   
	#include "stm32f103_lcd_soft_iic_port.h"
#elif (LCD_PORT == _HARD_IIC)  //硬件IIC(不推荐)  
	#include "stm32f103_lcd_hard_iic_port.h"
#elif (LCD_PORT == _DMA_IIC)  //DMA_IIC驱动 (暂不支持)
	//#include "stm32f103_oled_dma_iic_port.h"
	#error ("stm32f103 dma iic driver is not supported!")
#else
	#error ("no lcd driver.h")
#endif

#if (FLASH_PORT == _F_NO_PORT)
	//没有flash接口
#elif (FLASH_PORT == _F_SOFT_STDSPI)
	#include "stm32f103_flash_soft_stdspi_port.h"
#elif (FLASH_PORT == _F_HARD_STDSPI)
	#include "stm32f103_flash_hard_stdspi_port.h"
#elif (FLASH_PORT == _F_DMA_STDSPI)
	#include "stm32f103_flash_dma_stdspi_port.h"
#else
	#error ("no flash.h")
#endif

#if ((FLASH_PORT == _F_NO_PORT) && (FLASH_MODEL == _FLASH_NONE))
	//没有flash
	#define flash_ic_init()                  do{}while(0)
	#define flash_read_addr_ndat(addr,p,len) do{}while(0)
#elif (FLASH_MODEL == _FLASH_W25Qxx)
	#include "w25qxx.h"
	#define flash_ic_init()                  do{w25qxx_init();}while(0)
	#define flash_read_addr_ndat(addr,p,len) do{w25qxx_read_data(addr,p,len);}while(0)
#else
	#error ("no flash driver.h")
#endif




#define LCD_UNKNOW (0)//未知
#define LCD_OLED   (1)//普通屏幕
#define LCD_GRAY   (2)//灰度屏幕
#define LCD_RGB565 (3)//彩色屏幕RGB565

#if (LCD_IC == _SH1106)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SH1106_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1106_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1106_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1106_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SH1106_Set_Contrast(x);}while(0)
	#include "sh1106.h"
#elif (LCD_IC == _SH1108)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SH1108_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1108_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1108_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1108_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SH1108_Set_Contrast(x);}while(0)
	#include "sh1108.h"
#elif (LCD_IC == _SH1107)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SH1107_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1107_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1107_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1107_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "SH1107.h"
#elif (LCD_IC == _SH1115)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SH1115_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1115_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1115_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1115_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SH1115_The_Contrast_Control_Mode_Set(x);}while(0)
	#include "sh1115.h"
#elif (LCD_IC == _SSD1306)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SSD1306_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1306_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1306_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1306_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "ssd1306.h"
#elif (LCD_IC == _SSD1309)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SSD1309_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1309_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1309_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1309_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "ssd1309.h"
#elif (LCD_IC == _SSD1312)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SSD1312_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1312_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1312_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1312_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "ssd1312.h"
#elif (LCD_IC == _SSD1315)
	#define LCD_TYPE  LCD_OLED//使用OLED屏幕
	#define LCD_IC_Init() do{SSD1315_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1315_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1315_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1315_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SSD1315_Set_Contrast_Control(x);}while(0)
	#include "ssd1315.h"
#elif (LCD_IC == _SSD1327)
	#define LCD_TYPE  LCD_GRAY//使用灰度屏幕
	#define LCD_IC_Init() do{SSD1327_Init();}while(0)
	#define LCD_Set_Addr_x(x0,x1) do{SSD1327_Set_Addr_x(x0+SCREEN_X_OFFSET,x1+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_y(y0,y1) do{SSD1327_Set_Addr_y(y0+SCREEN_Y_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{SSD1327_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{/*SSD1327_Set_Contrast_Control(x);*/}while(0)
	#include "ssd1327.h"
#elif (LCD_IC == _ST7735)
	#define LCD_TYPE  LCD_RGB565//使用RGB屏幕565色域
	#define LCD_IC_Init() do{ST7735_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7735_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7735.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7789V3)
	#define LCD_TYPE  LCD_RGB565//使用RGB屏幕565色域
	#define LCD_IC_Init() do{ST7789V3_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7789V3_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7789v3.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7789VW)
	#define LCD_TYPE  LCD_RGB565//使用RGB屏幕565色域
	#define LCD_IC_Init() do{ST7789VW_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7789VW_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7789vw.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7796S)
	#define LCD_TYPE  LCD_RGB565//使用RGB屏幕565色域
	#define LCD_IC_Init() do{ST7796S_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7796S_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7796s.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _GC9A01)
	#define LCD_TYPE  LCD_RGB565//使用RGB屏幕565色域
	#define LCD_IC_Init() do{GC9A01_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{GC9A01_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "gc9a01.h"
	#include "TFT_Color.h"
#else
	#warning("No screen ic init!")
#endif



#if(LCD_TYPE != LCD_RGB565)
	#define LCD_COLOUR_BIT (1)
	#ifdef PAGE_CRC_NUM
		#undef  PAGE_CRC_NUM
		#define PAGE_CRC_NUM (1)
	#endif
#endif

#if !defined PAGE_CRC_NUM
		#define PAGE_CRC_NUM (1)
#endif
#define DYNA_CRC_ONCE_XNUM  ((SCREEN_WIDTH+PAGE_CRC_NUM-1)/PAGE_CRC_NUM) //一次校验的数量
	
#endif

	
