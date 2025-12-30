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
#ifndef _FLASH_PORT_TEMPLATE_
#define _FLASH_PORT_TEMPLATE_

#include "stdint.h"
#include "lcd_driver_config.h"

//此处可包含GPIO SPI DMA等头文件,按需包含
//禁止使用包含main.h或其他容易造成循环包含的文件

/*--------------------------------------------------------------
  * 名称: flash_cs_clr(void)
  * 功能: FLASH片选使能
----------------------------------------------------------------*/
void flash_cs_clr(void);

/*--------------------------------------------------------------
  * 名称: flash_cs_set(void)
  * 功能: FLASH片选失能
----------------------------------------------------------------*/
void flash_cs_set(void);
	
/*--------------------------------------------------------------
  * 名称: flash_send_1Byte(uint8_t dat)
  * 传入: dat
  * 返回: 无
  * 功能: SPI发送1个字节数据
  * 说明: 
----------------------------------------------------------------*/
void flash_send_1Byte(uint8_t dat);
	
/*--------------------------------------------------------------
  * 名称: flash_send_nByte(uint8_t *p,uint32_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向flash发送num个数据
  * 说明: 
----------------------------------------------------------------*/
void flash_send_nByte(uint8_t *p,uint32_t num);
	
/*--------------------------------------------------------------
  * 名称: flash_read_1Byte()
  * 功能: SPI发送1个字节数据
----------------------------------------------------------------*/
uint8_t flash_read_1Byte(void);

/*--------------------------------------------------------------
  * 名称: flash_read_nByte(uint8_t *p,uint32_t num)
  * 传入1: *p数组指针
  * 传入2: num读取数量
  * 功能: 向flash读取num个数据到p
----------------------------------------------------------------*/
void flash_read_nByte(uint8_t *p,uint32_t num);
	
/*--------------------------------------------------------------
  * 名称: flash_port_init()
  * 功能: flash接口初始化
----------------------------------------------------------------*/
void flash_port_init(void);
	
#endif
