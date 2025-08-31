/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2021
 *
 */
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
#include "..\inc\main.h"
#include "cw32l012_systick.h"
#include "cw32l012_sysctrl.h"
#include "cw32l012_gpio.h"
#include "cw32l012_adc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc1, valueAdc2;

volatile uint8_t gFlagIrq;


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
 * @brief ADC I/O初始化
 *
 */
void ADC_PortInit(void)
{
    //打开GPIO时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
    //打开ADC时钟
    __SYSCTRL_ADC_CLK_ENABLE();

    //set PA00 as ADC1 AIN0 INPUT
    PA00_ANALOG_ENABLE();
    // set PA05 as ADC 2 AIN0 INPUT
    PA05_ANALOG_ENABLE();
    
    // set PB05 as ADC1_SAM
    PB05_AFx_ADC1SAM();
    PB05_DIGTAL_ENABLE();
    PB05_DIR_OUTPUT();
    PB05_PUSHPULL_ENABLE();
    
    // set PA04 as ADC2_SAM
    PA04_AFx_ADC2SAM();
    PA04_DIGTAL_ENABLE();
    PA04_DIR_OUTPUT();
    PA04_PUSHPULL_ENABLE();
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    
    //使用sysTick,产生1ms中断计时
    InitTick(4000000);
    
    Bsp_Init(4000000);

    //配置ADC测试IO口
    ADC_PortInit();

    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;
    ADC_InitStructure.ADC_SlaveMod = ADC_SlaveMode_Disable;
    ADC_InitStructure.ADC_BaseSampTime = ADC_BaseSampTime3Clk;
#if (ADC_CONVERT_MODE == ADC_CONVERT_ONCE)
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;    // 单次采样
#else
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Continuous;    // 连续采样
#endif
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to0;     // 单通道
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputCH0;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime6Clk;     // 3个基本周期 +6个采样周期+ 15个固定转换周期，共24个周期，48MHz ADCCLK条件下可达2MSa/s

    ADC_Init(CW_ADC1, &ADC_InitStructure);
    
    // 配置ADC2为从模式
    ADC_InitStructure.ADC_SlaveMod = ADC_SlaveMode_Enable;
    ADC_Init(CW_ADC2, &ADC_InitStructure);

    ADC_ClearITPendingAll(CW_ADC1);

#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_INTERRUPTION)
    ADC_ITConfig(CW_ADC1, ADC_IT_EOC, ENABLE);
    NVIC_EnableIRQ(ADC1_IRQn);
#endif

    //ADC使能
    ADC_Enable(CW_ADC1);
    ADC_Enable(CW_ADC2);
    ADC_SoftwareStartConvCmd(CW_ADC1, ENABLE);

    while (1)
    {
#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_POLLING)

        while (!(CW_ADC1->ISR & ADCx_ISR_EOC_Msk));
        ADC_ClearITPendingBit(CW_ADC1,ADC_IT_EOC);
        valueAdc1 = ADC_GetConversionValue(CW_ADC1,0);
        valueAdc2 = ADC_GetConversionValue(CW_ADC2,0);
#endif

#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_INTERRUPTION)

        if (gFlagIrq & ADCx_ISR_EOC_Msk)
        {
            valueAdc1 = ADC_GetConversionValue(CW_ADC1, 0);
            valueAdc2 = ADC_GetConversionValue(CW_ADC2,0);
            gFlagIrq = 0;
        }

#endif

        Bsp_Led_Tog(&LED1);
        printf("The ADC1 Result:0x%3x[%d]\r\n", valueAdc1, valueAdc1);
        printf("The ADC2 Result:0x%3x[%d]\r\n", valueAdc2, valueAdc2);
        SysTickDelay(300);
        
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
  * @return None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

