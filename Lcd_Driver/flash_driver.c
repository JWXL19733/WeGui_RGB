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

#include "flash_driver.h"
#if (FLASH_PORT != _F_NO_PORT)

#if ((LCD_TYPE == LCD_OLED) || (LCD_TYPE == LCD_GRAY))
__weak void flash_draw_img(uint16_t x, uint16_t y, uint32_t flash_addr) 
{
	
}

#elif (LCD_TYPE == LCD_RGB565)
/*--------------------------------------------------------------
  * 名称: flash_draw_IMG_RGB565(uint16_t x, uint16_t y, uint32_t flash_addr) 
  * 传入1:x输出坐标
  * 传入2:y输出坐标
  * 传入3:flash_addr图片数据的flash地址
  * 返回: 无
  * 功能: 从FLASH读取IMG_RGB565格式图片投放到屏幕坐标(x,y)上
  * 说明: 该函数使用weak类型 方便修改成更高执行效率的函数 
----------------------------------------------------------------*/
__attribute__((weak)) void flash_draw_IMG_RGB565(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, uint32_t flash_addr)
{
		#define IMG_FLASH_SIZE 128 //设置读取缓冲大小
		uint8_t flash_dat_buff[IMG_FLASH_SIZE];
		uint32_t total_size   = (uint32_t) sizex * sizey * 2;
		uint32_t aligned_size = total_size / IMG_FLASH_SIZE;//falsh缓冲区对齐的部分
		uint16_t remaining    = total_size % IMG_FLASH_SIZE;//不对齐剩余的部分
	
		LCD_Set_Addr(x, y, x + sizex - 1, y + sizey - 1);
		while (aligned_size--) 
		{
				flash_read_addr_ndat(flash_addr, flash_dat_buff, IMG_FLASH_SIZE);
				LCD_Send_nDat(flash_dat_buff, IMG_FLASH_SIZE);
				flash_addr += IMG_FLASH_SIZE;
		}
		if (remaining > 0) 
		{
				flash_read_addr_ndat(flash_addr, flash_dat_buff, remaining);
				LCD_Send_nDat(flash_dat_buff, remaining);
		}
		#undef IMG_FLASH_SIZE
}

/*--------------------------------------------------------------
  * 名称: flash_draw_IMG_RGB332(uint16_t x, uint16_t y, uint32_t flash_addr) 
  * 传入1:x输出坐标
  * 传入2:y输出坐标
  * 传入3:flash_addr图片数据的flash地址
  * 返回: 无
  * 功能: 从FLASH读取IMG_RGB332格式图片投放到屏幕坐标(x,y)上
  * 说明: 该函数使用weak类型 方便修改成更高执行效率的函数 
----------------------------------------------------------------*/
__attribute__((weak)) void flash_draw_IMG_RGB332(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, uint32_t flash_addr)
{
		//RGB332转RGB565快速转换表
		//const uint16_t RGB332to565Table[256] = 
		//{
		//	0x0000, 0x000a, 0x0015, 0x001f, 0x0120, 0x012a, 0x0135, 0x013f, 0x0240, 0x024a, 0x0255, 0x025f, 0x0360, 0x036a, 0x0375, 0x037f, 
		//	0x0480, 0x048a, 0x0495, 0x049f, 0x05a0, 0x05aa, 0x05b5, 0x05bf, 0x06c0, 0x06ca, 0x06d5, 0x06df, 0x07e0, 0x07ea, 0x07f5, 0x07ff, 
		//	0x2000, 0x200a, 0x2015, 0x201f, 0x2120, 0x212a, 0x2135, 0x213f, 0x2240, 0x224a, 0x2255, 0x225f, 0x2360, 0x236a, 0x2375, 0x237f, 
		//	0x2480, 0x248a, 0x2495, 0x249f, 0x25a0, 0x25aa, 0x25b5, 0x25bf, 0x26c0, 0x26ca, 0x26d5, 0x26df, 0x27e0, 0x27ea, 0x27f5, 0x27ff, 
		//	0x4800, 0x480a, 0x4815, 0x481f, 0x4920, 0x492a, 0x4935, 0x493f, 0x4a40, 0x4a4a, 0x4a55, 0x4a5f, 0x4b60, 0x4b6a, 0x4b75, 0x4b7f, 
		//	0x4c80, 0x4c8a, 0x4c95, 0x4c9f, 0x4da0, 0x4daa, 0x4db5, 0x4dbf, 0x4ec0, 0x4eca, 0x4ed5, 0x4edf, 0x4fe0, 0x4fea, 0x4ff5, 0x4fff, 
		//	0x6800, 0x680a, 0x6815, 0x681f, 0x6920, 0x692a, 0x6935, 0x693f, 0x6a40, 0x6a4a, 0x6a55, 0x6a5f, 0x6b60, 0x6b6a, 0x6b75, 0x6b7f, 
		//	0x6c80, 0x6c8a, 0x6c95, 0x6c9f, 0x6da0, 0x6daa, 0x6db5, 0x6dbf, 0x6ec0, 0x6eca, 0x6ed5, 0x6edf, 0x6fe0, 0x6fea, 0x6ff5, 0x6fff, 
		//	0x9000, 0x900a, 0x9015, 0x901f, 0x9120, 0x912a, 0x9135, 0x913f, 0x9240, 0x924a, 0x9255, 0x925f, 0x9360, 0x936a, 0x9375, 0x937f, 
		//	0x9480, 0x948a, 0x9495, 0x949f, 0x95a0, 0x95aa, 0x95b5, 0x95bf, 0x96c0, 0x96ca, 0x96d5, 0x96df, 0x97e0, 0x97ea, 0x97f5, 0x97ff, 
		//	0xb000, 0xb00a, 0xb015, 0xb01f, 0xb120, 0xb12a, 0xb135, 0xb13f, 0xb240, 0xb24a, 0xb255, 0xb25f, 0xb360, 0xb36a, 0xb375, 0xb37f, 
		//	0xb480, 0xb48a, 0xb495, 0xb49f, 0xb5a0, 0xb5aa, 0xb5b5, 0xb5bf, 0xb6c0, 0xb6ca, 0xb6d5, 0xb6df, 0xb7e0, 0xb7ea, 0xb7f5, 0xb7ff, 
		//	0xd800, 0xd80a, 0xd815, 0xd81f, 0xd920, 0xd92a, 0xd935, 0xd93f, 0xda40, 0xda4a, 0xda55, 0xda5f, 0xdb60, 0xdb6a, 0xdb75, 0xdb7f, 
		//	0xdc80, 0xdc8a, 0xdc95, 0xdc9f, 0xdda0, 0xddaa, 0xddb5, 0xddbf, 0xdec0, 0xdeca, 0xded5, 0xdedf, 0xdfe0, 0xdfea, 0xdff5, 0xdfff, 
		//	0xf800, 0xf80a, 0xf815, 0xf81f, 0xf920, 0xf92a, 0xf935, 0xf93f, 0xfa40, 0xfa4a, 0xfa55, 0xfa5f, 0xfb60, 0xfb6a, 0xfb75, 0xfb7f, 
		//	0xfc80, 0xfc8a, 0xfc95, 0xfc9f, 0xfda0, 0xfdaa, 0xfdb5, 0xfdbf, 0xfec0, 0xfeca, 0xfed5, 0xfedf, 0xffe0, 0xffea, 0xfff5, 0xffff 
		//};
		#define IMG_FLASH_SIZE 128 //设置读取缓冲大小
		uint8_t flash_dat_buff[IMG_FLASH_SIZE];
		uint32_t total_size   = (uint32_t) sizex * sizey * 1;
		uint32_t aligned_size = total_size / IMG_FLASH_SIZE;//falsh缓冲区对齐的部分
		uint16_t remaining    = total_size % IMG_FLASH_SIZE;//不对齐剩余的部分
		
		LCD_Set_Addr(x, y, x + sizex - 1, y + sizey - 1);
		while (aligned_size--) 
		{
				uint16_t i;
				uint8_t * p = flash_dat_buff;
				flash_read_addr_ndat(flash_addr, flash_dat_buff, IMG_FLASH_SIZE);
				flash_addr += IMG_FLASH_SIZE;
				for (i = 0; i < IMG_FLASH_SIZE; i++) 
				{
						uint8_t rgb332 = * p++;
						LCD_Send_1Dat(((rgb332 & 0xE0) | ((rgb332 >> 2) & 0x1F))); 
						LCD_Send_1Dat(((rgb332 << 3) & 0xF8) | ((rgb332 << 2) & 0x04) | (rgb332 & 0x03)); 
				}
		}
		if (remaining > 0) 
		{
				uint16_t i;
				uint8_t * p = flash_dat_buff;
				flash_read_addr_ndat(flash_addr, flash_dat_buff, remaining);
				for (i = 0; i < remaining; i++) 
				{
						uint8_t rgb332 = * p++;
						LCD_Send_1Dat(((rgb332 & 0xE0) | ((rgb332 >> 2) & 0x1F))); 
						LCD_Send_1Dat(((rgb332 << 3) & 0xF8) | ((rgb332 << 2) & 0x04) | (rgb332 & 0x03)); 
				}
		}
		#undef IMG_FLASH_SIZE
}

/*--------------------------------------------------------------
  * 名称: flash_draw_IMG_RGB565_ZIP_ORLE2(uint16_t x, uint16_t y, uint32_t flash_addr) 
  * 传入1:x输出坐标
  * 传入2:y输出坐标
  * 传入3:flash_addr图片数据的flash地址
  * 返回: 无
  * 功能: 从FLASH读取RGB565_ZIP_ORLE2格式图片投放到屏幕坐标(x,y)上
  * 说明: 该函数使用weak类型 方便修改成更高执行效率的函数 
----------------------------------------------------------------*/
__attribute__((weak)) void flash_draw_IMG_RGB565_ZIP_ORLE2(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, uint32_t flash_addr)
{
		#define IMG_FLASH_SIZE 128 //设置读取缓冲大小
		uint8_t flash_dat_buff[IMG_FLASH_SIZE];
		uint16_t i = 0;
		uint8_t *p;
		uint8_t dat[3];
		LCD_Set_Addr(x, y, x + sizex - 1, y + sizey - 1);
		while (1) 
		{
				if (i <= 0) 
				{
						flash_read_addr_ndat(flash_addr, flash_dat_buff, (IMG_FLASH_SIZE / 3 * 3));
						flash_addr += (IMG_FLASH_SIZE / 3 * 3);
						p = & flash_dat_buff[0];
						i = (IMG_FLASH_SIZE / 3);
				}
				i--;
				dat[0] = * p++;
				if (dat[0] == 0x00) {break;} 
				dat[1] = * p++;
				dat[2] = * p++;
				while (dat[0]--) 
				{
						LCD_Send_1Dat(dat[1]); 
						LCD_Send_1Dat(dat[2]); 
				}
		}
		#undef IMG_FLASH_SIZE
}

/*--------------------------------------------------------------
  * 名称: flash_draw_IMG_RGB332_ZIP_ORLE1(uint16_t x, uint16_t y, uint32_t flash_addr) 
  * 传入1:x输出坐标
  * 传入2:y输出坐标
  * 传入3:flash_addr图片数据的flash地址
  * 返回: 无
  * 功能: 从FLASH读取RGB332_ZIP_ORLE1格式图片投放到屏幕坐标(x,y)上
  * 说明: 该函数使用weak类型 方便修改成更高执行效率的函数 
----------------------------------------------------------------*/
__attribute__((weak)) void flash_draw_IMG_RGB332_ZIP_ORLE1(uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, uint32_t flash_addr)
{
		#define IMG_FLASH_SIZE 128 //读取缓冲大小
		uint8_t flash_dat_buff[IMG_FLASH_SIZE];
		uint16_t i = 0;
    uint8_t * p;
		uint8_t num,rgb332;
		LCD_Set_Addr(x, y, x + sizex - 1, y + sizey - 1);
    while (1) 
		{
        if (i <= 0) 
				{
            flash_read_addr_ndat(flash_addr, flash_dat_buff, (IMG_FLASH_SIZE / 2 * 2));
            flash_addr += (IMG_FLASH_SIZE / 2 * 2);
            p = & flash_dat_buff[0];
            i = (IMG_FLASH_SIZE / 2);
        }
        i--;
        num = * p++;
        if (num == 0x00) 
				{
            break;
        } 
        rgb332 = * p++;
        while (num--) 
				{
            LCD_Send_1Dat(((rgb332 & 0xE0) | ((rgb332 >> 2) & 0x1F))); 
            LCD_Send_1Dat(((rgb332 << 3) & 0xF8) | ((rgb332 << 2) & 0x04) | (rgb332 & 0x03)); 
        }
    }
		#undef IMG_FLASH_SIZE
}

/*--------------------------------------------------------------
  * 名称: flash_draw_img(uint16_t x, uint16_t y, uint32_t flash_addr) 
  * 传入1:x输出坐标
  * 传入2:y输出坐标
  * 传入3:flash_addr带头文件的图片flash地址
  * 返回: 无
  * 功能: 从FLASH读取"带文件头"的任意格式图片投放到屏幕坐标(x,y)上
  * 说明: 该函数使用weak类型 方便修改成更高执行效率的函数 
----------------------------------------------------------------*/
__attribute__((weak)) void flash_draw_img(uint16_t x, uint16_t y, uint32_t flash_addr) 
{
		uint8_t  flash_dat_buff[6];	//头缓冲区
		uint8_t  head_len         ;	//文件头:长度
		uint16_t sizex            ;	//文件头:图片宽
		uint16_t sizey            ;	//文件头:图片高
		uint8_t  img_type         ;	//文件头:图片压缩类型
	
		/* 读取文件头 */
    flash_read_addr_ndat(flash_addr, flash_dat_buff, 6);
    head_len   = flash_dat_buff[0];
    sizex      = ((uint16_t) flash_dat_buff[1] << 8) | flash_dat_buff[2];
    sizey      = ((uint16_t) flash_dat_buff[3] << 8) | flash_dat_buff[4];
    img_type   = flash_dat_buff[5];
    flash_addr = flash_addr + head_len;
    
		/* 解析图片 */
    switch (img_type) 
		{
				/* 无压缩 */
        case IMG_RGB565: flash_draw_IMG_RGB565(x,y,sizex,sizey,flash_addr);break;
        case IMG_RGB888: break;
        case IMG_RGB555: break;
        case IMG_RGB444: break;
        case IMG_RGB332: flash_draw_IMG_RGB332(x,y,sizex,sizey,flash_addr);break;
			
				/* 压缩 */
        case IMG_RGB565_ZIP_ORLE2: flash_draw_IMG_RGB565_ZIP_ORLE2(x,y,sizex,sizey,flash_addr);break;
        case IMG_RGB888_ZIP_ORLE3: break;
        case IMG_RGB555_ZIP_ORLE2: break;
        case IMG_RGB444_ZIP_ORLE2: break;
        case IMG_RGB332_ZIP_ORLE1: flash_draw_IMG_RGB332_ZIP_ORLE1(x,y,sizex,sizey,flash_addr);break;
    }
}

#endif
#endif 
