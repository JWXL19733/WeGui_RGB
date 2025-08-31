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
#include "cw32l012_halltim.h"
#include "cw32l012_btim.h"

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
char RamBuf[100] __attribute__((at(0x20000200)));


int main(void) 
{   
    HALLTIM_InitTypeDef HALLTIM_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();
    
    PA03_AFx_HALLTIMCH1();
    PA04_AFx_HALLTIMCH2();
    PA05_AFx_HALLTIMCH3();
    PB00_AFx_HALLTIMOUT();
    PB01_AFx_HALLTIMTRGO();    
    
    GPIO_InitStruct.Pins = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pins = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);  
    
    HALLTIM_InitStruct.filter_enable = TRUE;
    HALLTIM_InitStruct.auto_reload_value = 9999; // 设置自动重装载值为10000-1
    HALLTIM_InitStruct.filter_length = 10;       // 设置滤波宽度为10个时钟周期
    HALLTIM_InitStruct.clock_division = HALLTIM_DIV_PCLK_DIV4;    // 设置时钟分频为PCLK/4
    HALLTIM_InitStruct.trigger_output = HALLTIM_MMS_MATCH;    // 设置触发输出模式为CCR匹配脉冲
    
    // 初始化HALLTIM模块
    HALLTIM_Init(&HALLTIM_InitStruct);
    
    // 设置比较值为500
    HALLTIM_SetCompareValue(5000);
    
    // 清除所有状态标志
    HALLTIM_ClearStatus(HALLTIM_ISR_MATCH | HALLTIM_ISR_OV | HALLTIM_ISR_CHG);

    // 启用HALLTIM模块
    HALLTIM_Cmd(ENABLE);    

    // 生成软件触发信号
    HALLTIM_GenerateSoftTrigger();

    // 等待一段时间，让HALLTIM模块运行
    for (volatile int i = 0; i < 1000000; i++);

    // 获取计数值
    uint32_t counter_value = HALLTIM_GetCounterValue();
    sprintf(RamBuf,"counter_value:%u\n", counter_value);

    // 获取脉冲宽度
    uint32_t pulse_width = HALLTIM_GetPulseWidth();
    sprintf(&RamBuf[25],"pulse_width:%u\n", pulse_width);

    // 获取未滤波的HALL信号状态
    uint32_t raw_state = HALLTIM_GetRawStateCH1();
    sprintf(&RamBuf[50],"raw_state_ch1:0x%X\n", raw_state);

    raw_state = HALLTIM_GetRawStateCH2();
    sprintf(&RamBuf[50],"raw_state_ch2:0x%X\n", raw_state);

    raw_state = HALLTIM_GetRawStateCH3();
    sprintf(&RamBuf[50],"raw_state_ch3:0x%X\n", raw_state);

    // 获取滤波后的HALL信号状态
    uint32_t filtered_state = HALLTIM_GetFilteredStateCH1();
    sprintf(&RamBuf[70],"filtered_state_ch1:0x%X\n", filtered_state);

    filtered_state = HALLTIM_GetFilteredStateCH2();
    sprintf(&RamBuf[70],"filtered_state_ch2:0x%X\n", filtered_state);

    filtered_state = HALLTIM_GetFilteredStateCH3();
    sprintf(&RamBuf[70],"filtered_state_ch3:0x%X\n", filtered_state);

    // 清除所有状态标志
    HALLTIM_ClearStatus(HALLTIM_ISR_MATCH | HALLTIM_ISR_OV | HALLTIM_ISR_CHG);
    
    while(1) 
    {       
        
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
