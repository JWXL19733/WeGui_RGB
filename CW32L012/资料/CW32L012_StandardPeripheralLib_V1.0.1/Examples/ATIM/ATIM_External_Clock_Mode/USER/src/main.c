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
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void ATIM_Configuration(void);
void NVIC_Configuration(void);
void ATIM_IRQHandlerCallBack(void);

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
 * @brief  ATIM边沿对齐模式，向上计数，ATIMETR作为时钟源
 *
 * @return int32_t
 */
int32_t main(void)
{
    /* System Clocks Configuration */
    SYSCTRL_Configuration();

    Bsp_Init(8000000);
    Bsp_Led_On(&LED1);     // 默认LED1灭

    InitTick(8000000);  
    
    GPIO_Configuration();

    ATIM_Configuration();
    NVIC_Configuration();

    while (1)
    {
        //中断服务程序中翻转LED1的电平
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);    //  配置系统时钟为8MHz
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV8);   //  PCLK= 8MHz/8 = 1MHz
    __SYSCTRL_ATIM_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{   
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.Pins = GPIO_PIN_0;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

    PB00_AFx_ATIMETR();   //PB00复用为ATIMETR
}

void ATIM_Configuration(void)
{
    ATIM_InitTypeDef ATIM_InitStruct = {DISABLE, 0};
    ATIM_ETRInitTypeDef ATIM_ETRInitStruct;

    ATIM_InitStruct.BufferState = ENABLE;               //使能缓存寄存器
    ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_ALIGN_MODE_EDGE;    //边沿对齐
    ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;        //向上计数；
    ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    //连续运行模式
    ATIM_InitStruct.Prescaler = 0;    // 1分频
    ATIM_InitStruct.ReloadValue = 9;               // 自动重载值10
    ATIM_InitStruct.RepetitionCounter = 0;           // 重复周期0
    ATIM_Init(&ATIM_InitStruct);

    ATIM_ETRInitStruct.Source = ATIM_ETRSOURCE_ETR_PIN;
    ATIM_ETRInitStruct.Polarity = ATIM_ETR_POLARITY_NEGATIVE;
    ATIM_ETRInitStruct.Prescaler = ATIM_ETR_PRESCALE_NONE;
    ATIM_ETRInitStruct.Filter = ATIM_ETR_FILTER_DIV32_N8;
    ATIM_ETRInit(&ATIM_ETRInitStruct);

    ATIM_SetFilterClockDiv(FILTER_CLOCK_DIV4);
    ATIM_EnableExternalClock(ENABLE);
    
    
    ATIM_ITConfig(ATIM_IT_UIE,ENABLE);
    ATIM_UpdateEventSelect(ATIM_UPDATE_EVENT_NORMAL);
    ATIM_Cmd(ENABLE);    
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(ATIM_IRQn);
    __enable_irq();
}

void ATIM_IRQHandlerCallBack(void)
{
    if (CW_ATIM->ISR_f.UIF)
    {
        CW_ATIM->ICR_f.UIF = 0;
        Bsp_Led_Tog(&LED1);
    }
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

