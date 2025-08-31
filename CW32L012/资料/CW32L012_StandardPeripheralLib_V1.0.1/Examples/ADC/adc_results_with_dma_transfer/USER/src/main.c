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
#include "cw32l012_dma.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc1, valueAdc2;

volatile uint8_t gFlagIrq;

uint32_t RamBuff[8];
volatile uint32_t RegisterValue;


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
static void RCC_Config(void);
static void ADC_PortInit(void);
static void ADC_Config(void);
static void DMA_Config(void);

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    RCC_Config();
    //使用sysTick,产生1ms中断计时
    InitTick(16000000);

    Bsp_Uart_Init(16000000, 1000000);   // 将打印串口的波特率改为1Mbps

    ADC_Config();
    DMA_Config();
    ADC_SoftwareStartConvCmd(CW_ADC1, ENABLE);

    while (1)
    {
        if (CW_DMA->ISR_f.TC2)
        {
            // 传输完成，暂停串口的DMA功能，待ADC转换完成后触发DMA功能再次打开
            CW_UART3->CR2 = 0;
            CW_DMA->ICR = 0;
        }
    }
}

static void RCC_Config(void)
{
    SYSCTRL_HSI_Enable(HSIOSC_TO_HSI16MHZ);    // HSI分频到16Mhz
}

/**
 * @brief ADC I/O初始化
 *
 */
static void ADC_PortInit(void)
{
    //打开GPIO时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();

    //set PA00 as AIN0 INPUT
    PA00_ANALOG_ENABLE();
    //set PA01 as AIN1 INPUT
    PA01_ANALOG_ENABLE();
    //set PA02 as AIN2 INPUT
    PA02_ANALOG_ENABLE();
    //set PA03 as AIN2 INPUT
    PA03_ANALOG_ENABLE();
    //set PA04 as AIN2 INPUT
    PA04_ANALOG_ENABLE();
    //set PA05 as AIN5 INPUT
    PA05_ANALOG_ENABLE();
    //set PA06 as AIN6 INPUT
    PA06_ANALOG_ENABLE();
    //set PA07 as AIN7 INPUT
    PA07_ANALOG_ENABLE();

    // set PB05 as ADC1_SAM
    PB05_AFx_ADC1SAM();
    PB05_DIGTAL_ENABLE();
    PB05_DIR_OUTPUT();
    PB05_PUSHPULL_ENABLE();
}

static void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    ADC_DMA_TriggerTypeDef ADC_DmaTriggerStruct;


    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div8;
    ADC_InitStructure.ADC_BaseSampTime = ADC_BaseSampTime10Clk;
    ADC_InitStructure.ADC_SlaveMod = ADC_SlaveMode_Disable;
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Continuous;    // 连续采样
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to7;     // 8通道序列采样
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputCH0;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN1.ADC_InputChannel = ADC_InputCH1;
    ADC_InitStructure.ADC_IN1.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN2.ADC_InputChannel = ADC_InputCH2;
    ADC_InitStructure.ADC_IN2.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN3.ADC_InputChannel = ADC_InputCH3;
    ADC_InitStructure.ADC_IN3.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN4.ADC_InputChannel = ADC_InputCH4;
    ADC_InitStructure.ADC_IN4.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN5.ADC_InputChannel = ADC_InputCH5;
    ADC_InitStructure.ADC_IN5.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN6.ADC_InputChannel = ADC_InputCH6;
    ADC_InitStructure.ADC_IN6.ADC_SampTime = ADC_SampTime518Clk;
    ADC_InitStructure.ADC_IN7.ADC_InputChannel = ADC_InputCH7;
    ADC_InitStructure.ADC_IN7.ADC_SampTime = ADC_SampTime518Clk;
    ADC_Init(CW_ADC1, &ADC_InitStructure);
    ADC_ClearITPendingAll(CW_ADC1);

    //配置ADC测试IO口
    ADC_PortInit();

    // 配置DMA使能
    ADC_DmaTriggerStruct.dma_eoc_enable = FALSE;
    ADC_DmaTriggerStruct.dma_eos_enable = TRUE;
    ADC_DMACmd(CW_ADC1, &ADC_DmaTriggerStruct);

    //ADC使能
    ADC_Enable(CW_ADC1);
}


static void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure = {0};
    
    RegisterValue = 0x80;   // 此值有DMA传输UART3的CR2寄存器，使能UART的DMA tx
    
    //打开DMA时钟
    __SYSCTRL_DMA_CLK_ENABLE();

    DMA_InitStructure.DataSize = DMA_DATA_SIZE_32BITS;
    DMA_InitStructure.SrcIncrement = DMA_ADDRESS_FIXED;
    DMA_InitStructure.DstIncrement = DMA_ADDRESS_FIXED;
    DMA_InitStructure.TransferMode = DMA_MODE_BLOCK;
    DMA_InitStructure.TransferCount = 1;
    DMA_InitStructure.BlockCount = 1;    //必须设置为1
    DMA_InitStructure.SrcAddress = (uint32_t) & (RegisterValue);
    DMA_InitStructure.DstAddress = (uint32_t) & (CW_UART3->CR2);    // 使能UART的DMA tx
    DMA_InitStructure.TriggerType = DMA_TRIGGER_HARDWARE;
    DMA_InitStructure.TriggerSource = DMA_TRIGGER_SRC_ADC1_SEQ;
    DMA_InitStructure.RestartEnable = TRUE;

    DMA_Init(DMA_CHANNEL_1, &DMA_InitStructure);
    DMA_Cmd(DMA_CHANNEL_1, ENABLE);

    DMA_InitStructure.DataSize = DMA_DATA_SIZE_8BITS;
    DMA_InitStructure.SrcIncrement = DMA_ADDRESS_INCREMENT;
    DMA_InitStructure.DstIncrement = DMA_ADDRESS_FIXED;
    DMA_InitStructure.TransferMode = DMA_MODE_BLOCK;
    DMA_InitStructure.TransferCount = 32;
    DMA_InitStructure.BlockCount = 1;    //必须设置为1
    DMA_InitStructure.SrcAddress = (uint32_t) & (CW_ADC1->RESULT0);
    DMA_InitStructure.DstAddress = (uint32_t) & (CW_UART3->TDR);    // UART通过DMA传输ADC的转换结果
    DMA_InitStructure.TriggerType = DMA_TRIGGER_HARDWARE;
    DMA_InitStructure.TriggerSource = DMA_TRIGGER_SRC_UART3_TX;
    DMA_InitStructure.RestartEnable = TRUE;

    DMA_Init(DMA_CHANNEL_2, &DMA_InitStructure);
    DMA_Cmd(DMA_CHANNEL_2, ENABLE);
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

