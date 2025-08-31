/**
  ******************************************************************************
  * @file    main.c
  * @author  AE Team
  * @version 1.0
  * @date    2024-08-15
  * @brief   
  *
  * @note    硬件平台：CW32L012_StartKit_V1.0
  *          
  *
  *****************************************************************************/
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"
#include "cw32l012_dma.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** 
 **
 ******************************************************************************/
int32_t main(void)
{
    
    // 初始化时钟为96MHz/12 = 8MHz；
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);
    
    DMA_InitTypeDef DMA_InitStruct = {0};
    
    DMA_InitStruct.BlockCount = 1;
    DMA_InitStruct.TransferCount =  20;
    DMA_InitStruct.DataSize = DMA_DATA_SIZE_32BITS;
    DMA_InitStruct.DstAddress = 0x20000200;
    DMA_InitStruct.DstIncrement = DMA_ADDRESS_INCREMENT;
    DMA_InitStruct.RestartEnable = FALSE;
    DMA_InitStruct.SrcAddress = 0x00000000;
    DMA_InitStruct.SrcIncrement = DMA_ADDRESS_INCREMENT;
    DMA_InitStruct.TransferMode = DMA_MODE_BLOCK;
    DMA_InitStruct.TriggerSource = DMA_TRIGGER_SRC_UART1_RX;    //采用软件触发，硬件触发源无意义
    DMA_InitStruct.TriggerType = DMA_TRIGGER_SOFTWARE;
    
    // 初始化DMA
    DMA_Init(DMA_CHANNEL_1, &DMA_InitStruct);
    
    // 使能DMA
    DMA_Cmd(DMA_CHANNEL_1, ENABLE);
    
    // 软件启动传输
    DMA_StartTransfer(DMA_CHANNEL_1);
    
    // 检查传输完成
    while(DMA_STATUS_COMPLETE != DMA_CheckStatus(DMA_CHANNEL_1));
    
    // 清除传输完成中断标志
    DMA_ClearInterruptFlag(DMA_CHANNEL_1, DMA_INTERRUPT_TE|DMA_INTERRUPT_TC);
    
    while(1);    
    
    
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
 /**
   * @brief  Reports the name of the source file and the source line number
   *         where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif
