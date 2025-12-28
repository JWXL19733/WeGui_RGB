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
#include "flash_port_template.h"

/*--------------------------------------------------------------
  * 名称: flash_cs_clr(void)
  * 功能: FLASH片选使能 weak类型 需要改写
----------------------------------------------------------------*/
__attribute__((weak)) void flash_cs_clr(void)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: flash_cs_set(void)
  * 功能: FLASH片选失能 weak类型 需要改写
----------------------------------------------------------------*/
__attribute__((weak)) void flash_cs_set(void)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: flash_send_1Byte(uint8_t dat)
  * 传入: dat
  * 功能: SPI发送1个字节数据 weak类型 需要改写
----------------------------------------------------------------*/
__attribute__((weak)) void flash_send_1Byte(uint8_t dat)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: flash_send_nByte(uint8_t *p,uint32_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向flash发送num个数据
  * 说明: weak类型 需要改写
----------------------------------------------------------------*/
__attribute__((weak)) void flash_send_nByte(uint8_t *p,uint32_t num)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: flash_read_1Byte()
  * 功能: SPI发送1个字节数据 weak类型 需要改写
----------------------------------------------------------------*/
__attribute__((weak)) uint8_t flash_read_1Byte()
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: flash_read_nByte(uint8_t *p,uint32_t num)
  * 传入1: *p数组指针
  * 传入2: num读取数量
  * 返回: 无
  * 功能: 向flash读取num个数据到p
  * 说明: 
----------------------------------------------------------------*/
__attribute__((weak)) void flash_read_nByte(uint8_t *p,uint32_t num)
{
	while(1)//需要移植 参考stm32f103例程
	{
		;
	}
}

/*--------------------------------------------------------------
  * 名称: flash_port_init()
  * 功能: flash接口初始化 weak类型 需要改写
----------------------------------------------------------------*/
__attribute__((weak)) void flash_port_init()
{
	//while(1)//需要移植 参考stm32f103例程
	//{
	//	;
	//}
	//flash_ic_init();
}

