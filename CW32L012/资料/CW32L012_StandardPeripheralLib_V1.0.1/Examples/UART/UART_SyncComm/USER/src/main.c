/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2022
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
 ******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"

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
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_SendBuf_Polling(UART_TypeDef *UARTx, uint8_t *TxBuf, uint8_t TxCnt);
uint8_t UART_RecvBuf_Polling(UART_TypeDef *UARTx, uint8_t *RxBuf);


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t  TxBuffer[] = {0x90, 0xFF, 0xFF, 0x00, 0xFF, 0xFF};
uint8_t  RxBuffer[60];
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
 ******************************************************************************/
int32_t main(void)
{
    uint8_t cmd = 0x9F;    //JEDEC ID
    UART_SyncModeInitTypeDef UART_SyncModeInitStruct = {0};
    //配置RCC
    RCC_Configuration();

    Bsp_Init(96000000);
    
    InitTick(96000000);      // tick 1ms
    //配置GPIO
    GPIO_Configuration();

    //配置UART
    UART_SyncModeInitStruct.UART_ClockSource = UART_Source_PCLK;
    UART_SyncModeInitStruct.UART_ClockFreq = 96000000;
    UART_SyncModeInitStruct.UART_BaudRate = 1000000;    // 注：BaudRate = UCLK/(2*BRRI), BRRI<0x100
    UART_SyncModeInitStruct.UART_TransmitOrder = UART_Transmit_Order_MSB;
    UART_SyncMode_Init(CW_UART1, &UART_SyncModeInitStruct);

    PB02_SETLOW();    // CS
    UART_SyncMode_SendDataPolling(CW_UART1, &cmd, 1);
    UART_SyncMode_ReceiveDataPolling(CW_UART1, RxBuffer, 3);
    PB02_SETHIGH();
    
    printf("The JEDEC ID is :0x%2X 0x%2X 0x%2X\r\n", RxBuffer[0], RxBuffer[1],RxBuffer[2]);


    while (1)
    {
        Bsp_Led_Tog(&LED1);
        SysTickDelay(500);    
    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSIOSC/1 = HCLK = PCLK == 96MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI96MHZ);

    //外设时钟使能
    __SYSCTRL_UART1_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __SYSCTRL_GPIOB_CLK_ENABLE();

    //UART TX RX 复用
    PB00_AFx_UART1RXD();
    PB01_AFx_UART1TXD();

    GPIO_InitStructure.Pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    PB02_SETHIGH();
}



/**
 * @brief 配置NVIC
 *
 */
void NVIC_Configuration(void)
{

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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

