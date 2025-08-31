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
uint16_t valueAdc[2];

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
void GTIM_OutputConfig(void);
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

    //set PA00 as AIN0 INPUT
	//set PA01 as AIN1 INPUT
    PA00_ANALOG_ENABLE();
	PA01_ANALOG_ENABLE();
    
    // set PB05 as ADC1_SAM
    PB05_AFx_ADC1SAM();
    PB05_DIGTAL_ENABLE();
    PB05_DIR_OUTPUT();
    PB05_PUSHPULL_ENABLE();
}
/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};

    // 系统时钟配置为96MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI96MHZ);
    
    //使用sysTick,产生1ms中断计时
    InitTick(96000000);
    
    // 调试串口初始化
    Bsp_Init(96000000);

    //配置ADC测试IO口
    ADC_PortInit();

    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div2;//ADCLK = PCLK/2 = 48MHz
    ADC_InitStructure.ADC_SlaveMod = ADC_SlaveMode_Disable;
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;    // 单次采样
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to1;     // 双通道
	ADC_InitStructure.ADC_BaseSampTime = ADC_BaseSampTime10Clk;
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputCH0;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime102Clk;//10个BaseSampTime+ 102个SampTime+ 15个固定转换周期，共127个ADCCLK周期，48MHz ADCCLK条件下需要2.6us
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputCH1;
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime70Clk;//10个BaseSampTime+ 70个SampTime+ 15个固定转换周期，共95个ADCCLK周期，48MHz ADCCLK条件下需要1.98us
	
    ADC_Init(CW_ADC1, &ADC_InitStructure);

    ADC_ClearITPendingAll(CW_ADC1);

#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_INTERRUPTION)
    ADC_ITConfig(CW_ADC1, ADC_IT_EOS, ENABLE);
    NVIC_EnableIRQ(ADC1_IRQn);
#endif

    //ADC使能
    ADC_Enable(CW_ADC1);
	ADC_ExtTrigCfg(CW_ADC1,ADC_TRIG_GTIM1TRGO,ENABLE);//ADC外部触发源是GTIM1的TRGO信号
	
	GTIM_OutputConfig();

    while (1)
    {
#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_POLLING)

        while (!(CW_ADC1->ISR & ADCx_ISR_EOS_Msk));
        ADC_ClearITPendingBit(CW_ADC1,ADC_IT_EOS);
        valueAdc[0] = ADC_GetConversionValue(CW_ADC1,0);
		valueAdc[1] = ADC_GetConversionValue(CW_ADC1,1);
		
		Bsp_Led_Tog(&LED1);//每1秒LED1翻转一次
        printf("The ADC sample PA00 , result is : %d\r\n", valueAdc[0]);
		printf("The ADC sample PA01 , result is : %d\r\n", valueAdc[1]);
#endif

#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_INTERRUPTION)

        if (gFlagIrq & ADCx_ISR_EOS_Msk)
        {
            valueAdc[0] = ADC_GetConversionValue(CW_ADC1, 0);
			valueAdc[1] = ADC_GetConversionValue(CW_ADC1,1);
            gFlagIrq = 0;
			
			Bsp_Led_Tog(&LED1);//每1秒LED1翻转一次
            printf("The ADC sample PA00 , result is : %d\r\n", valueAdc[0]);
		    printf("The ADC sample PA01 , result is : %d\r\n", valueAdc[1]);
        }

#endif
		
    }
}

void GTIM_OutputConfig(void)
{
	//PB04---GTIM1_CH1
	__SYSCTRL_GPIOB_CLK_ENABLE();
	PB04_DIGTAL_ENABLE();
	PB04_DIR_OUTPUT();
	PB04_PUSHPULL_ENABLE();
	PB04_AFx_GTIM1CH1();
	
	//GTIM1
    GTIM_InitTypeDef GTIM_InitStruct = {0};
    GTIM_OCModeCfgTypeDef GTIM_OCModeCfgStruct = {DISABLE, DISABLE, 0};

    __SYSCTRL_GTIM1_CLK_ENABLE();

    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;//边沿计数
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 9600 - 1;//96M/9600 = 10KHz
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = 10000 - 1;//溢出时间 = 10000*0.1ms = 1s
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

    GTIM_OCModeCfgStruct.FastMode = DISABLE;
    GTIM_OCModeCfgStruct.OCMode = GTIM_OC_MODE_PWM2;
    GTIM_OCModeCfgStruct.OCPolarity = GTIM_OC_POLAR_NONINVERT;
    GTIM_OCModeCfgStruct.PreloadState = DISABLE;
    GTIM_OC1ModeCfg(CW_GTIM1, &GTIM_OCModeCfgStruct);
    
    GTIM_SetCompare1(CW_GTIM1, 5000);
    GTIM_OC1Cmd(CW_GTIM1, ENABLE);
	
    GTIM_Cmd(CW_GTIM1, ENABLE);
	GTIM_MasterTRGOCfg(CW_GTIM1,GTIM_MASTER_TRGO_OC1REF);//GTIM1的TRGO信号源是OC1REF的上升沿时刻
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

