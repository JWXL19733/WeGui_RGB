/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if(LCD_PORT == _DMA_4SPI)


#include "cw32l012_lcd_dma_4spi_port.h"
#include "lcd_driver.h"
#include "stdint.h"

//---宏函数---


//---库函数版本---
//等待SPI发送缓冲器为空
//#define wait_lcd_spi_txtemp_free() do{while(SPI_GetFlagStatus(CW_SPI1,SPI_FLAG_TXE)==RESET);}while(0)
//向SPI发送缓冲器发送一个数据
//#define send_lcd_spi_dat(dat)      do{SPI_SendData(CW_SPI1, dat);}while(0)
//等待SPI发送器空闲(发完)
//#define send_lcd_spi_done()        do{while(SPI_GetFlagStatus(CW_SPI1,SPI_FLAG_BUSY));}while(0)


//---寄存器版本---
//等待SPI发送缓冲器为空
#define wait_lcd_spi_txtemp_free() do{while((CW_SPI1->ISR & SPI_FLAG_TXE)==RESET);}while(0)
//向SPI发送缓冲器发送一个数据
#define send_lcd_spi_dat(dat)      do{CW_SPI1->DR = dat;}while(0)
//等待SPI发送器空闲(发完)
#define send_lcd_spi_done()        do{while(CW_SPI1->ISR & SPI_FLAG_BUSY);}while(0)




volatile lcd_dma_step_t  DMA_State = DMA_FREE;
uint8_t DMA_reflash_step=0;

#if defined (LCD_USE_NORMAL_OLED) || defined (LCD_USE_GRAY_OLED)
//-------------------------------------------以下是OLED屏幕专用驱动接口----------------------------------------------


/*--------------------------------------------------------------
  * 名称: LCD_Port_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 屏幕接口初始化
  * 说明: 
----------------------------------------------------------------*/
void LCD_Port_Init(void)
{
	//OELD SPI SCL默认常高,第二个跳变数据沿有效
	//TFT SPI SCL默认常低,第一个跳变数据沿有效
	SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 双线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                     // 主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                 // 帧数据长度为8bit
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                        // 时钟空闲电平为高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                      // 第2个边沿采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                         // 片选信号由SSI寄存器控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BAUDRATE_PCLK_DIVx; // 波特率为PCLK的分频
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                // 最高有效位 MSB 收发在前
  SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                      // 低速SPI
	
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();LCD_CS_Set();
	LCD_BL_IO_Init();LCD_BL_On();
	
	__SYSCTRL_SPI1_CLK_ENABLE();
	SPI_Init(CW_SPI1, &SPI_InitStructure);
	SPI_DMACmd(CW_SPI1, SPI_DMAReq_Tx, ENABLE);
  SPI_Cmd(CW_SPI1, ENABLE);
	
	
	DMA_InitTypeDef DMA_InitStruct = {0};
    __SYSCTRL_DMA_CLK_ENABLE();
	
  DMA_InitStruct.BlockCount = 1;// 每个传输任务的数据块数量
  DMA_InitStruct.TransferCount =  5;// 待传输的任务数量
  DMA_InitStruct.DataSize = DMA_DATA_SIZE_8BITS;// 数据位宽
  DMA_InitStruct.DstAddress = (uint32_t)&CW_SPI1->DR;
  DMA_InitStruct.DstIncrement = DMA_ADDRESS_FIXED;// 地址固定
  DMA_InitStruct.RestartEnable = FALSE;// 自动重传使能（0：禁用，1：启用）
  DMA_InitStruct.SrcAddress = (uint32_t)lcd_driver.LCD_GRAM;
  DMA_InitStruct.SrcIncrement = DMA_ADDRESS_INCREMENT;// 地址自增
  DMA_InitStruct.TransferMode = DMA_MODE_BULK;
  DMA_InitStruct.TriggerSource = DMA_TRIGGER_SRC_SPI1_TX;//采用软件触发，硬件触发源无意义
  DMA_InitStruct.TriggerType = DMA_TRIGGER_SOFTWARE;// 软件触发
  DMA_Init(DMA_CHANNEL_1, &DMA_InitStruct);
  DMA_Cmd(DMA_CHANNEL_1, ENABLE);
	
	DMA_ITConfig(DMA_CHANNEL_1, DMA_INTERRUPT_TC, ENABLE);//开中断
	
	// 清除传输完成中断标志 (官方demo出错, 在此修复)
  //DMA_ClearInterruptFlag(DMA_CHANNEL_1, DMA_INTERRUPT_TE|DMA_INTERRUPT_TC);
	DMA_ClearInterruptFlag(DMA_CHANNEL_1, DMA_ICR_TC1_Msk);//(已纠正)
	
	NVIC_SetPriority(DMACH12_IRQn, 0);
	NVIC_EnableIRQ(DMACH12_IRQn);
	
	DMA_StartTransfer(DMA_CHANNEL_1);
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	__SYSCTRL_CRC_CLK_ENABLE();//动态刷新CRC校验用
	#endif

	DMA_State=DMA_FREE;
	
	LCD_RES_Clr();
	{
		volatile unsigned int i=10000;
		while(i--);
	}
	LCD_RES_Set();
}


/*--------------------------------------------------------------
  * 名称: LCD_delay_ms(volatile uint32_t ms)
  * 传入1: ms
  * 返回: 无
  * 功能: 软件延时
  * 说明: 非精准
----------------------------------------------------------------*/
void LCD_delay_ms(volatile uint32_t ms)
{
	//delay_ms(ms);
	
		volatile uint16_t i;
		while (ms--) 
		{
			i = 10000; //根据实际情况调整
			while (i--);
		}
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令(DC=0时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Clr();
	LCD_CS_Clr();
	{
		send_lcd_spi_dat(dat);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}
/*--------------------------------------------------------------
  * 名称: LCD_Send_1Dat(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个数据(DC=1时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Dat(uint8_t dat)
{
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Set();
	LCD_CS_Clr();
	{
		send_lcd_spi_dat(dat);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}
/*--------------------------------------------------------------
  * 名称: LCD_Send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个数据
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nDat(uint8_t *p,uint16_t num)
{
	while(DMA_State!=DMA_FREE){;}
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Set();
	LCD_CS_Clr();
		
	//1.关闭DMA(很多情况下关闭DMA才能调整DMA参数)
	*DMA_CSRy(DMA_CHANNEL_1) &= ~DMA_CSR1_EN_Msk;
	
	//2.设置DMA待发送的数据数量,连续发多个数据
	*DMA_CNTy(DMA_CHANNEL_1) = num;
		
	//3.设置DMA待发送的数据的起始地址指针
	*DMA_SRCADDRy(DMA_CHANNEL_1) = (uint32_t)p;
	
	//4.使能DMA
	*DMA_CSRy(DMA_CHANNEL_1) |= DMA_CSR1_EN_Msk;
		
	//5.开启传输
	DMA_StartTransfer(DMA_CHANNEL_1);
}


/*--------------------------------------------------------------
  * 名称: LCD_Send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nCmd(uint8_t *p,uint16_t num)
{
	uint16_t i=0;
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Clr();
	LCD_CS_Clr();
	while(num>i)	  
	{
		wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
		send_lcd_spi_dat(p[i++]);//向SPI发送缓冲器发送一个数据
	}
	wait_lcd_spi_txtemp_free();//等待SPI发送缓冲器为空
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_CS_Set();
}

#endif












/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/
/*--------------------------------------------------------------
  * 名称: LCD_DMAx_INT_Func(void)
  * 功能: DMA中断接口
----------------------------------------------------------------*/

//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if defined (LCD_USE_NORMAL_OLED)

#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全屏刷新----------
//--优点:全屏刷新
//--缺点:内容不变的区域也参与了刷新
uint8_t LCD_Refresh(void)
{
	while(DMA_State != DMA_FREE){;}
	send_lcd_spi_done();
	LCD_Set_Addr(0,0);
	while(DMA_State != DMA_FREE){;}
	DMA_State = DMA_REFLASH;
	DMA_reflash_step=1;
		
		
	//发送第一行,激活dma
	send_lcd_spi_done();//等待SPI发送器空闲(发完)
	LCD_DC_Set();
	LCD_CS_Clr();
	//1.关闭DMA(很多情况下关闭DMA才能调整DMA参数)
	*DMA_CSRy(DMA_CHANNEL_1) &= ~DMA_CSR1_EN_Msk;
	//2.设置DMA待发送的数据数量,连续发多个数据
	*DMA_CNTy(DMA_CHANNEL_1) = SCREEN_WIDTH;
	//3.设置DMA待发送的数据的起始地址指针
	*DMA_SRCADDRy(DMA_CHANNEL_1) = (uint32_t)&lcd_driver.LCD_GRAM[0][0][0];
	//4.使能DMA
	*DMA_CSRy(DMA_CHANNEL_1) |= DMA_CSR1_EN_Msk;
	//5.开启传输屏幕第一行数据
	DMA_StartTransfer(DMA_CHANNEL_1);
	

	//DMA中断里刷新余下行
	return 0;
}

#define LCD_DMA_SPIx_ISR DMACH12_IRQHandler
void LCD_DMA_SPIx_ISR()//放到DMA完成中断里
{
		lcd_dma_step_t save_DMA_State=DMA_State;
		save_DMA_State = DMA_State;
		DMA_State = DMA_FREE;//解决中断里因该变量引起死循环的问题
	
		//1.关闭DMA(很多情况下关闭DMA才能调整DMA参数)
		*DMA_CSRy(DMA_CHANNEL_1) &= ~DMA_CSR1_EN_Msk;
		//2.清除中断
		DMA_ClearInterruptFlag(DMA_CHANNEL_1, DMA_ICR_TC1_Msk);//(官方库有问题, 此处纠正)
	
		//等待SPI发完(DMA完毕不代表SPI完毕)
		send_lcd_spi_done();
		LCD_CS_Set();
		switch(save_DMA_State)
		{
			case DMA_FREE:break;
			case DMA_NORMAL_CMD:
			{
				save_DMA_State = DMA_FREE;
				LCD_CS_Set();
			}break;
			case DMA_REFLASH:
			{
				if(DMA_reflash_step >= GRAM_YPAGE_NUM)
				{
					LCD_CS_Set();
					DMA_reflash_step = 0;
					save_DMA_State = DMA_FREE;
					break;
				}
				LCD_Set_Addr(0,DMA_reflash_step);
				LCD_Send_nDat(&lcd_driver.LCD_GRAM[DMA_reflash_step++][0][0],SCREEN_WIDTH);
			}break;
		}
		DMA_State = save_DMA_State;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
	//动态刷新不支持dma方式
	#error("DYNA_UPDATE mode not support dma4spi driver yet!")
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)
	//页刷新不支持dma方式
	#error("PAGE_BUFF mode not support dma4spi driver yet!")
#elif (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)
	//页刷新不支持dma方式
	#error("PAGE_BUFF mode not support dma4spi driver yet!")
#endif

//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_GRAY_OLED)//灰度OLED
	//灰度OLED屏不支持dma4spi方式驱动 请更改屏幕驱动方式
	#error ("Gray OLED not support dma4spi driver yet!Use 4spi_driver please!")

//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)
	//彩屏TFT屏不支持dma4spi方式驱动 请更改屏幕驱动方式
	#error ("TFT not support dma4spi driver yet!");
#else
	#error ("Not support LCD!");
#endif



#endif
