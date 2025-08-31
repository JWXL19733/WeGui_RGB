/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/
#include "lcd_driver_config.h"

#if (LCD_IC == _ST7735)
#include "st7735.h"



void ST7735_Set_Addr(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	
	//1.等待SPI空闲
	//while(b_dma_busy!=0){};//等待DMA完毕,空闲
	//Wait_Soft_spi_Done();
		
	//发送数据
	uint8_t i[]={0x2a,x1>>8,x1&0xff,(x2)>>8,(x2)&0xff};
	uint8_t j[]={0x2b,y1>>8,(y1)&0xff,(y2)>>8,y2&0xff};
	const uint8_t k[]={0x2c};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
	LCD_Send_nCmd((uint8_t*)j,sizeof(j)/sizeof(uint8_t));
	LCD_Send_nCmd((uint8_t*)k,sizeof(k)/sizeof(uint8_t));

}



void ST7735_Soft_Reset()
{
	const uint8_t i[]={0x01};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Sleep_In()
{
	const uint8_t i[]={0x10};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Sleep_Out()
{
	const uint8_t i[]={0x11};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Partial_Mode_On()
{
	const uint8_t i[]={0x12};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Partial_Mode_Off()
{
	const uint8_t i[]={0x13};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Inversion_Off()
{
	const uint8_t i[]={0x20};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Inversion_On()
{
	const uint8_t i[]={0x21};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Display_Off()
{
	const uint8_t i[]={0x28};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Display_On()
{
	const uint8_t i[]={0x29};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_TE_Line_Off()
{
	const uint8_t i[]={0x34};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_TE_Line_On()
{
	#define TELOM 0//[0:1]When TELOM =’0’: The Tearing Efonly
	const uint8_t i[]={0x35,TELOM};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Idle_Off()
{
	const uint8_t i[]={0x38};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Idle_On()
{
	const uint8_t i[]={0x39};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Set_RGB565_Mode()
{
	uint8_t i[]={0x3A,0x05};//65k(RGB565)
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Set_RGB444_Mode()
{
	uint8_t i[]={0x3A,0x03};//4k(RGB444)
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}
void ST7735_Set_RGB666_Mode()
{
	uint8_t i[]={0x3A,0x06};//262k(RGB666)
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
}

void ST7735_Clear()//清除IC显示缓存
{
	uint16_t i=0;
	ST7735_Set_Addr(0,0,162-1,162-1);
	while(i++<162*162)
	{
		LCD_Send_1Dat(0x33);LCD_Send_1Dat(0x33);//测试
		//LCD_Send_1Dat(0x00);LCD_Send_1Dat(0x00);
	}
}



void ST7735_Init()
{
	ST7735_Soft_Reset();
	LCD_RES_Clr();
	LCD_delay_ms(100);
	LCD_RES_Set();
	LCD_delay_ms(100);
	
  LCD_delay_ms(100);
	
	LCD_Send_1Cmd(0x11);//Sleep exit 
	LCD_delay_ms(120);
		
	//ST7735R Frame Rate
	LCD_Send_1Cmd(0xB1); 
	LCD_Send_1Dat(0x01); 
	LCD_Send_1Dat(0x2C); 
	LCD_Send_1Dat(0x2D); 

	LCD_Send_1Cmd(0xB2); 
	LCD_Send_1Dat(0x01); 
	LCD_Send_1Dat(0x2C); 
	LCD_Send_1Dat(0x2D); 

	LCD_Send_1Cmd(0xB3); 
	LCD_Send_1Dat(0x01); 
	LCD_Send_1Dat(0x2C); 
	LCD_Send_1Dat(0x2D); 
	LCD_Send_1Dat(0x01); 
	LCD_Send_1Dat(0x2C); 
	LCD_Send_1Dat(0x2D); 
	
	LCD_Send_1Cmd(0xB4); //Column inversion 
	LCD_Send_1Dat(0x07); 
	
	//ST7735R Power Sequence
	LCD_Send_1Cmd(0xC0); 
	LCD_Send_1Dat(0xA2); 
	LCD_Send_1Dat(0x02); 
	LCD_Send_1Dat(0x84); 
	LCD_Send_1Cmd(0xC1); 
	LCD_Send_1Dat(0xC5); 

	LCD_Send_1Cmd(0xC2); 
	LCD_Send_1Dat(0x0A); 
	LCD_Send_1Dat(0x00); 

	LCD_Send_1Cmd(0xC3); 
	LCD_Send_1Dat(0x8A); 
	LCD_Send_1Dat(0x2A); 
	LCD_Send_1Cmd(0xC4); 
	LCD_Send_1Dat(0x8A); 
	LCD_Send_1Dat(0xEE); 
	
	LCD_Send_1Cmd(0xC5); //VCOM 
	LCD_Send_1Dat(0x0E); 
	
	//方向选择其一
	LCD_Send_1Cmd(0x36);
	LCD_Send_1Dat(0xC8);//方向1
	//LCD_Send_1Dat(0x08);//方向2
	//LCD_Send_1Dat(0x78);//方向3
	//LCD_Send_1Dat(0xA8);//方向4
	
	//ST7735R Gamma Sequence
	LCD_Send_1Cmd(0xe0); 
	LCD_Send_1Dat(0x0f); 
	LCD_Send_1Dat(0x1a); 
	LCD_Send_1Dat(0x0f); 
	LCD_Send_1Dat(0x18); 
	LCD_Send_1Dat(0x2f); 
	LCD_Send_1Dat(0x28); 
	LCD_Send_1Dat(0x20); 
	LCD_Send_1Dat(0x22); 
	LCD_Send_1Dat(0x1f); 
	LCD_Send_1Dat(0x1b); 
	LCD_Send_1Dat(0x23); 
	LCD_Send_1Dat(0x37); 
	LCD_Send_1Dat(0x00); 	
	LCD_Send_1Dat(0x07); 
	LCD_Send_1Dat(0x02); 
	LCD_Send_1Dat(0x10); 

	LCD_Send_1Cmd(0xe1); 
	LCD_Send_1Dat(0x0f); 
	LCD_Send_1Dat(0x1b); 
	LCD_Send_1Dat(0x0f); 
	LCD_Send_1Dat(0x17); 
	LCD_Send_1Dat(0x33); 
	LCD_Send_1Dat(0x2c); 
	LCD_Send_1Dat(0x29); 
	LCD_Send_1Dat(0x2e); 
	LCD_Send_1Dat(0x30); 
	LCD_Send_1Dat(0x30); 
	LCD_Send_1Dat(0x39); 
	LCD_Send_1Dat(0x3f); 
	LCD_Send_1Dat(0x00); 
	LCD_Send_1Dat(0x07); 
	LCD_Send_1Dat(0x03); 
	LCD_Send_1Dat(0x10);  
	
	LCD_Send_1Cmd(0x2a);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x7f);

	LCD_Send_1Cmd(0x2b);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x9f);
	
	LCD_Send_1Cmd(0xF0); //Enable test command  
	LCD_Send_1Dat(0x01); 
	LCD_Send_1Cmd(0xF6); //Disable ram power save mode 
	LCD_Send_1Dat(0x00); 
	
	LCD_Send_1Cmd(0x3A); //65k mode 
	LCD_Send_1Dat(0x05); 

	ST7735_Clear();//清除IC显示缓存
	LCD_Send_1Cmd(0x29);//Display on	 
}


#endif
