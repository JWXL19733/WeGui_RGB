/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
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
#define LED_GPIO_PORT CW_GPIOB
#define LED_GPIO_PINS  GPIO_PIN_9
#define KEY_GPIO_PORT CW_GPIOA
#define KEY_GPIO_PINS  GPIO_PIN_0
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
    uint16_t tmp;

    GPIO_InitTypeDef GPIO_InitStruct;

    __SYSCTRL_GPIOA_CLK_ENABLE(); 
    __SYSCTRL_GPIOB_CLK_ENABLE();
    
    InitTick(4000000);

    GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStruct.Pins = KEY_GPIO_PINS;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = LED_GPIO_PINS;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
    
    SYSCTRL_LSI_Enable();    // 使用LSI提供GPIO中断滤波的时钟
    GPIO_ConfigFilter(KEY_GPIO_PORT,  KEY_GPIO_PINS, GPIO_FLTCLK_LSI);

    GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PINS, GPIO_Pin_SET);    // LED1 turn on
    
    while (GPIO_ReadPin(KEY_GPIO_PORT, KEY_GPIO_PINS) == GPIO_Pin_SET);
    GPIOA_INTFLAG_CLR(KEY_GPIO_PINS);
    NVIC_EnableIRQ(GPIOA_IRQn);
	GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PINS, GPIO_Pin_RESET);    // LED1 turn off
    tmp = 0x00;
    while (1)
    {
        SYSCTRL_GotoDeepSleep();
        for(tmp = 0; tmp < 10; tmp++)
        {
            GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PINS);
            SysTickDelay(200);    // 200ms延时
        }
    }
}

void GPIOA_IRQHandlerCallBack(void)
{
    if ((KEY_GPIO_PORT->ISR & KEY_GPIO_PINS) != 0)
    {
        KEY_GPIO_PORT->ICR &=  ~KEY_GPIO_PINS;
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif
