/**
 * @file main.c
 * @brief 本文件为设备驱动库模块的主程序文件。
 * @details
 * @author WHXY
 * @date 2021-03-12
 * @version 1.0
 *
 * @copyright
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
 */

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"
#include "cw32l012_vc.h"
#include "cw32l012_dac.h"

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
static void SYSCTRL_Configuration(void);
//static void GPIO_Configuration(void);
static void LED_Init(void);
static void NVIC_Configuration(void);
static void VC_PortInit(void);
static void VC_Configuration(void);
static void DAC_Configuration(void);

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 **
 **
 ******************************************************************************/
/**
 * @brief 主函数，程序的入口点。
 *
 * 该函数完成系统初始化工作，包括系统时钟配置、滴答定时器初始化、LED初始化、
 * VC模块配置、DAC模块配置以及中断向量控制器配置。随后进入一个无限循环，
 * 不断更新DAC通道1的数据并触发软件触发，同时将PB09引脚置低。
 *
 * @return int32_t 程序正常运行时不会返回。
 */
int32_t main(void)
{
    uint8_t outputValue;
    // 调用系统时钟配置函数，将系统时钟配置为8MHz
    SYSCTRL_Configuration();

    // 初始化滴答定时器，传入系统时钟频率8000000Hz
    InitTick(8000000);
    // 调用LED初始化函数，对LED进行初始化操作
    LED_Init();

    // 调用VC模块配置函数，对VC模块进行配置
    VC_Configuration();
    // 调用DAC模块配置函数，对DAC模块进行配置
    DAC_Configuration();
    // 调用中断向量控制器配置函数，对中断向量控制器进行配置
    NVIC_Configuration();

    while (1)
    {
        for (outputValue = 0; ; outputValue++)
        {
            DAC_SetChannel1Data(DAC_ALIGN_8B_R, outputValue);
            DAC_SoftwareTrigger(DAC_CHANNEL_1);
            // 延时50个滴答定时器周期
            SysTickDelay(50);
            PB09_SETLOW();
        }

    }
}

static void SYSCTRL_Configuration(void)
{
    // 系统时钟配置为8MHz
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI8MHZ);
    /* 1. 设置HCLK和PCLK的分频系数　*/
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);
    SYSCTRL_SystemCoreClockUpdate(8000000);
    /* 2. 配置外设时钟 */
    __SYSCTRL_VC_CLK_ENABLE(); //使能VC模块

    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();

}

//static void GPIO_Configuration(void)
//{
//    GPIO_InitTypeDef GPIO_InitStruct;
//}

static void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(VC13_IRQn);
    __enable_irq();
}

void VC13_IRQHandlerCallBack(void)
{
    // 清除中断标志
    VC_ClearInterruptFlag(CW_VC1);
    PB09_SETHIGH();
}

/**
 * @brief 初始化VC模块的输入输出引脚
 *
 * 此函数用于配置GPIO引脚，将PA00设置为VC1的输出引脚，
 * 并将PA02设置为VC1的正输入引脚。
 */
void VC_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __SYSCTRL_GPIOA_CLK_ENABLE();

    //set PA00 as VC1 output
    GPIO_InitStruct.Pins = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

    PA00_AFx_VC1OUT();

    //set PA02 as VC1 Positive input
    PA02_ANALOG_ENABLE();
}

static void VC_Configuration(void)
{
    VC_InitTypeDef    VC_InitStruct;

    // 配置VC的端口
    VC_PortInit();

    // 初始化 VC 模块
    VC_InitStruct.div  = VC_DIV_0;    // 此例程改参数未使用
    VC_InitStruct.filterConfig.FilterClock = VC_FILTER_CLOCK_PCLK;
    VC_InitStruct.filterConfig.FilterTime  = VC_FILTER_16X_6SAMPLES;
    VC_InitStruct.hysteresisEnable = TRUE;
    VC_InitStruct.inputNegative = VC_INN_DAC;    // DAC1的输出作为参考
    VC_InitStruct.inputPositive = VC_INP_IN1;    // PA02
    VC_InitStruct.interruptMode = VC_INT_FALLING;
    VC_InitStruct.polarityInvert = FALSE;
    VC_InitStruct.RefVoltage = VC_REF_EXT;
    VC_InitStruct.responseSpeed = VC_RESPONSE_SLOW;

    VC_Init(CW_VC1, &VC_InitStruct);

    VC_Cmd(CW_VC1, ENABLE);
}

static void DAC_Configuration(void)
{
    DAC_InitTypeDef DAC_InitStruct;

    DAC_InitStruct.DAC_ChannelState      = DAC_CHANNEL_ENABLE;
    DAC_InitStruct.DAC_DMAState          = DAC_DMA_DISABLE;
    DAC_InitStruct.DAC_DMAUnderflowState = DAC_DMA_UNDERFLOW_DISABLE;
    DAC_InitStruct.DAC_TriangleAmplitude = DAC_TriangleAmplitude_255;
    DAC_InitStruct.DAC_TriggerSource     = DAC_TRIG_SOFTWARE;
    DAC_InitStruct.DAC_TriggerState      = DAC_TRIGGER_DISABLE;
    DAC_InitStruct.DAC_WaveformMode      = DAC_WAVE_TRIANGLE;
    DAC_Init(DAC_CHANNEL_1, &DAC_InitStruct);

    DAC_Cmd(DAC_CHANNEL_1, ENABLE);
    DAC_ChannelOutCmd(DAC_CHANNEL_1, DISABLE);    // DAC的输出在内部提供给VC，如果需要观测波形可打开对外输出。
    DAC_SetChannel1Data(DAC_ALIGN_8B_R, 0x00);
}

/**
 * @brief LED I/O初始化
 *
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    //打开GPIO时钟

    __SYSCTRL_GPIOB_CLK_ENABLE();
    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Pins = GPIO_PIN_9; //LED1为PB09
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    //LEDs are off.
    PB09_SETLOW();
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
