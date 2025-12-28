/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if (LCD_PORT == _PC)

#include "stdint.h"
#include "wchar.h"
#include "stdio.h"
#include "pc_lcd_port.h"
#include "lcd_driver.h"
#include "SDL.h"


SDL_Window* window;
SDL_Renderer* renderer;
uint8_t sdl_quit=0;

/*--------------------------------------------------------------
  * 名称: lcd_bl_on()
  * 说明: 打开屏幕背光
----------------------------------------------------------------*/
void lcd_bl_on(void)
{
}

/*--------------------------------------------------------------
  * 名称: lcd_bl_off()
  * 说明: 关闭屏幕背光
----------------------------------------------------------------*/
void lcd_bl_off(void)
{
}

/*--------------------------------------------------------------
  * 名称: uint8_t()
  * 传入: 无
  * 返回: 0屏幕接口空闲 1屏幕接口忙碌
  * 说明: DMA或SPI忙碌
----------------------------------------------------------------*/
uint8_t lcd_is_busy()
{
    return 0;
}

void lcd_port_init()
{
    sdl_quit = 0;

    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
    }
    // 创建窗口
    window = SDL_CreateWindow("WeGui simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*WIN_ZOOM, SCREEN_HIGH*WIN_ZOOM, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    //1.清空渲染器
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);//笔刷着色
	SDL_RenderClear(renderer);
	//2.渲染器发送到屏幕
    SDL_RenderPresent(renderer);

}



/*--------------------------------------------------------------
  * 名称: void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: OLED屏幕从x,page位置开始刷屏
  * 说明: OLED屏幕移植接口 默认weak类型 需要移植改写
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_OLED)
void lcd_oled_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
}
#endif

/*--------------------------------------------------------------
  * 名称: void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: 灰度OLED屏幕从x,page位置开始刷屏
  * 说明: 灰度OLED屏幕移植接口 默认weak类型 需要移植改写
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_GRAY)
void lcd_gray_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
}
#endif

/*--------------------------------------------------------------
  * 名称: void rgb565_flush(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *gram)
  * 传入1:x0刷新起始横坐标
	* 传入2:x1刷新结束横坐标
  * 传入3:page当前刷新的页坐标
  * 传入4:*gram点阵数据指针 往下8点对齐逐行扫描
  * 功能: TFT_RGB565屏幕从x,page位置开始刷屏
  * 说明: RGB565屏幕移植接口 默认weak类型 需要移植改写
----------------------------------------------------------------*/
#if (LCD_TYPE == LCD_RGB565)
void lcd_rgb565_port(uint16_t x0,uint16_t x1,uint16_t page,uint8_t *page_gram)
{
}
#endif

/*--------------------------------------------------------------
  * 名称: uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
  * 传入1:*gram待校验数组指针
	* 传入2:len待校验数组长度
	* 返回: crc校验值
  * 说明: weak类型 需要移植,否则无法使用动态刷新
----------------------------------------------------------------*/
#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//动态刷新相关
uint16_t lcd_gram_crc_port(uint8_t *gram,uint16_t len)
{
    static uint16_t i;
    return i++;
}
#endif


uint8_t LCD_Refresh(void)
{
	uint16_t x,y;
	//1.清空渲染器
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);//笔刷着色
    SDL_RenderClear(renderer);


	for(y=0;y<SCREEN_HIGH;y++)
	{
		uint8_t ypage=y/8;
		uint8_t mask=0x01<<(y%8);
		uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][0][0];
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			uint8_t c=0;
			#if (LCD_COLOUR_BIT>=1)//---1位色---
				if(*gram++&mask){c+=1;}
			#endif
			#if (LCD_COLOUR_BIT>=2)//---2位色---
				if(*gram++&mask){c+=2;}
			#endif
			#if (LCD_COLOUR_BIT>=3)//---3位色---
				if(*gram++&mask){c+=4;}
			#endif

			//2.渲染器画点
			SDL_SetRenderDrawColor(renderer,
                                    (lcd_driver.colour[c] & 0xf800)    >> 8,
                                    (lcd_driver.colour[c] & 0x07e0)  >> 3,
                                    (lcd_driver.colour[c] & 0x001f)   << 3,
                                    0xff);
            for(uint8_t zy=0;zy<WIN_ZOOM;zy++)//像素放大
            {
                for(uint8_t zx=0;zx<WIN_ZOOM;zx++)
                {
                    SDL_RenderDrawPoint(renderer, x*WIN_ZOOM+zx,y*WIN_ZOOM+zy);
                }
            }
		}
	}
	//3.渲染器发送到屏幕
    SDL_RenderPresent(renderer);
	return 0;
}


#endif
