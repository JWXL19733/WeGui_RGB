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
#include "cw32l012_gtim.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc;
uint32_t valueAdcAcc;
volatile uint8_t gFlagIrq;
uint16_t gCntEoc = 0;
uint8_t cntSample;
float fTsDegree, fVoltage;

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
 * @brief ADC 配置
 *
 */
void ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    __SYSCTRL_ADC_CLK_ENABLE();

    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;
    ADC_InitStructure.ADC_BaseSampTime = ADC_BaseSampTime6Clk;    // 默认
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to1;
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputTs;        // 温度
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime166Clk;       // 采Ts至少需要40us，ADCCLK = 40us*Fadc = 40*4 =160
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputVref1P2;        // 内部1.2V参考
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime166Clk;      // 采样持续时间至少需要40us
    

    ADC_Init(CW_ADC1, &ADC_InitStructure);
    // 采集温度需要开启内置温度传感器
    ADC_SetTs(ADC_TsEnable);

    //ADC外部中断触发源配置
    ADC_ExtTrigCfg(CW_ADC1, ADC_TRIG_GTIM1TRGO, ENABLE);
    ADC_Enable(CW_ADC1);
    
     //  PB05 作为采样时序指示
    PB05_DIGTAL_ENABLE();
    PB05_DIR_OUTPUT();
    PB05_PUSHPULL_ENABLE();
    PB05_AFx_ADC1SAM();   
    
}

/**
 * @brief GTIM初始化
 *
 */
void GTIM_Init(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = { 0 };

    //打开GTIM时
    __SYSCTRL_GTIM1_CLK_ENABLE();

    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_DIS;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 4000 - 1;  //实际分频为 Prescaler+1 = 4000, 频率为1kHz
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = 1000 - 1;  // 1秒
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;

    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    // 配置GTIM的TRGO信号
    GTIM_MasterTRGOCfg(CW_GTIM1, GTIM_MASTER_TRGO_UPDATE);
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    // 初始化板上资源
    Bsp_Init(4000000ul);

    // ADC 初始化
    ADC_Configuration();

    // GTIM初始化
    GTIM_Init();

    // 启动GTIM，开始计时
    GTIM_Cmd(CW_GTIM1, ENABLE);

    while (1)
    {
        if (CW_ADC1->ISR_f.EOS)    // 采样转换完成
        {
            Bsp_Led_Tog(&LED1);
            CW_ADC1->ICR = 0;
            ADC_GetSqr1Result(CW_ADC1, &valueAdc);
            fVoltage = ADC_BgrResult2Avcc(valueAdc);
            ADC_GetSqr0Result(CW_ADC1, &valueAdc);
            fTsDegree = ADC_GetTs(fVoltage, valueAdc);
            printf("The Temperature is %2f\r\n", fTsDegree);
            printf("The Reference voltage is %2f\r\n", fVoltage);
        }
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
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

