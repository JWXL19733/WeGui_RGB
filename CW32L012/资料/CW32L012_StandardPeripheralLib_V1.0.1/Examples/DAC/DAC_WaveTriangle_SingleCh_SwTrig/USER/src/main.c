/**
  ******************************************************************************
  * @file    main.c
  * @author  AE Team
  * @version 1.0
  * @date    2024-08-15
  * @brief   本样例演示通过软件触发的方式使 DAC 单个通道输出三角波。
  *          使用示波器观察DAC_CH1(PB00)或DAC_CH1(PB01)
  *
  * @note    硬件平台：CW32L012_StartKit_V1.0
  *          接线说明：None
  *
  ******************************************************************************
  * @attention
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
  ******************************************************************************
  */


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "main.h"
#include "cw32l012_sysctrl.h"
#include "cw32l012_gpio.h"
#include "cw32l012_dac.h"

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/


/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
static void DAC_Config(void);

/*******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 **
 ******************************************************************************/
int32_t main(void)
{
    // 初始化系统时钟
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV24);    // 96MHz/24 = 4MHz

    // 初始化DAC
    DAC_Config();

    while (1)
    {
        DAC_SoftwareTrigger(DAC_CHANNEL_1);
//        DAC_SoftwareTrigger(DAC_CHANNEL_2);
        FirmwareDelay(10);  // 等待输出稳定
        
    }
}


/*******************************************************************************
 * @brief  DAC 配置
 * @param  None
 * @retval None
 */
static void DAC_Config(void)
{
    DAC_InitTypeDef DAC_InitStruct;
    
    __SYSCTRL_DAC_CLK_ENABLE();    // 开DAC外设时钟
    __SYSCTRL_GPIOB_CLK_ENABLE();  // 开GPIOB外设时钟
    
    AFx_DACOUT1_PB00();    // PB00为DAC通道1的输出
//    AFx_DACOUT2_PB01();    // PB01为DAC通道2的输出
    
    DAC_DeInit();
    
    DAC_InitStruct.DAC_ChannelState = DAC_CHANNEL_ENABLE;
    DAC_InitStruct.DAC_DMAState = DAC_DMA_DISABLE;       // DMA关闭
    DAC_InitStruct.DAC_DMAUnderflowState = DAC_DMA_UNDERFLOW_DISABLE;  // DMA中断关闭
    DAC_InitStruct.DAC_TriangleAmplitude = DAC_TriangleAmplitude_4095; //
    DAC_InitStruct.DAC_TriggerSource = DAC_TRIG_SOFTWARE;    // 软件触发
    DAC_InitStruct.DAC_TriggerState = DAC_TRIGGER_ENABLE;
    DAC_InitStruct.DAC_WaveformMode = DAC_WAVE_TRIANGLE;
    
    DAC_Init(DAC_CHANNEL_1, &DAC_InitStruct);
//    DAC_Init(DAC_CHANNEL_2, &DAC_InitStruct);     
    
    // 通道1输出到PB00使能
    DAC_ChannelOutCmd(DAC_CHANNEL_1, ENABLE);
    
    // 通道2输出到PB01使能
//    DAC_ChannelOutCmd(DAC_CHANNEL_2, ENABLE);
    
}


/*******************************************************************************
 ** EOF (not truncated)
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
    /* USER CODE BEGIN */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END */
}
#endif /* USE_FULL_ASSERT */


/************************ (C) COPYRIGHT SIYIMicro *****END OF FILE*************/
