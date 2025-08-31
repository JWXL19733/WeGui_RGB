/**
  ******************************************************************************
  * @file    main.c
  * @author  AE Team
  * @version 1.0
  * @date    2024-08-15
  * @brief   本样例演示 OPA 被 软件控制 校正的功能。
  *          OPA1 工作于 PGAx8 模式，ADC1 采样 BGR1.2V、OPA1_INP3 和 OPA1_OUT,
  *          软件控制OPA1校正。
  *
  *          向OPA1_INP3(PB01)输入小于 Vcc/2 的电压，从OPA1_OUT(PB00)输出OPA1放大后的电压。
  *          ADC1 采样并转换完成的结果存入RAM中
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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define TIMES    16

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
static void OPA_Config(void);
static void ADC_Config(void);
static void Delay(uint16_t nCount);
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
    uint32_t ResultBgr = 0;
    uint32_t ResultOpaIn = 0;
    uint32_t ResultOpaOut = 0;
    char RAM_DisplayBuff[512];
    
    memset(RAM_DisplayBuff, 0 , 512);
   
    // 初始化时钟为96MHz/12 = 8MHz；
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);

    // 配置ADC
    ADC_Config();    
    
    //初始化OPA
    OPA_Config();
    
    // 启动OPA
    OPA_Start(CW_OPA1);
    while(1)
    {
        OPA_AutoZeroSoft_Start(CW_OPA1);    // 启动软件校正
        FirmwareDelay(100);                 // 保持软件校正时间
        OPA_AutoZeroSoft_Stop(CW_OPA1);     // 停止软件校正
        
        ADC_SoftwareStartConvCmd(CW_ADC1, ENABLE);  // 启动ADC
        for (int i = 0; i < TIMES; i++)
        {
            while (ADC_GetITStatus(CW_ADC1, ADC_IT_EOS) == 0); // 等待EOS序列转换完成
            ADC_ClearITPendingAll(CW_ADC1);                    // 清除EOS序列转换完成

            ResultBgr += ADC_GetConversionValue(CW_ADC1, ADC_RESULT_0);              // 读出BGR1.2V的Code
            ResultOpaIn += ADC_GetConversionValue(CW_ADC1,ADC_RESULT_2);            // 读出OPA1_IN3的Code
            ResultOpaOut += ADC_GetConversionValue(CW_ADC1, ADC_RESULT_1);           // 读出OPA1_OUT的Code
        }
         ADC_SoftwareStartConvCmd(CW_ADC1, DISABLE);

        ResultBgr    /= TIMES;
        ResultOpaIn  /= TIMES;
        ResultOpaOut /= TIMES;

        float Avcc  = 1253 * 4.095f / ResultBgr;           // 获取AVCC的电压
        float OpaIn = Avcc * ResultOpaIn / 4095;                      // 计算出OPA_IN的电压
        float OpaOut = Avcc * ResultOpaOut / 4095;                    // 计算出OPA_OUT的电压

        // 打印ADC转换的结果
        sprintf(RAM_DisplayBuff,"BgrCode = 0x%04X  OpaInCode = 0x%04X  OpaInCode = 0x%04X\r\n", ResultBgr, ResultOpaIn, ResultOpaOut);
        sprintf(RAM_DisplayBuff+0x50,"AvccVol = %4.3fV  OpaInVol  = %4.3fV  OpaOutVol = %4.3fV\r\n", Avcc     , OpaIn,       OpaOut);        
        FirmwareDelay(500000);
        
        
       
    }
    
    
}

static void ADC_Config(void)
{
    ADC_InitTypeDef ADC_InitStruct={0};
    
    //是能ADC的APB时钟
    __SYSCTRL_ADC_CLK_ENABLE();
    
    ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div8;        // ADCCLK = 8MHz/8 = 1MHz
    ADC_InitStruct.ADC_ConvertMode = ADC_ConvertMode_Continuous;    // 持续转换模式
    ADC_InitStruct.ADC_SlaveMod = ADC_SlaveMode_Disable;        // 关闭从模式
    ADC_InitStruct.ADC_SQREns = ADC_SqrEns0to2;            // 配置转换序列通道0~2
    ADC_InitStruct.ADC_IN0.ADC_InputChannel = ADC_InputVref1P2;     // 内部1.2V
    ADC_InitStruct.ADC_IN0.ADC_SampTime = ADC_SampTime518Clk;       // 1.2V采样时间要大于40us
    ADC_InitStruct.ADC_IN1.ADC_InputChannel = ADC_InputCH8;     // 配置序列通道1的模拟输入源为ADC_IN8(PB00)，OPA1OUT 
    ADC_InitStruct.ADC_IN1.ADC_SampTime = ADC_SampTime30Clk;       
    ADC_InitStruct.ADC_IN2.ADC_InputChannel = ADC_InputCH9;     // 配置序列通道2的模拟输入源为ADC_IN9(PB01) OPA1INP3
    ADC_InitStruct.ADC_IN2.ADC_SampTime = ADC_SampTime30Clk;       
        
    ADC_Init(CW_ADC1,&ADC_InitStruct);
    ADC_Enable(CW_ADC1);
}

static void OPA_Config(void)
{    
    OPA_InitTypeDef OPA_InitStruct ={0};
    
    //使能OPA的时钟
    __SYSCTRL_OPA_CLK_ENABLE();
    
    //使能GPIO的时钟
    __SYSCTRL_GPIOA_CLK_ENABLE();
    __SYSCTRL_GPIOB_CLK_ENABLE();

    AFx_OPA1INP3_PB01();                                // 设置OPA1INP3_PB01为模式端口
    AFx_OPA1OUT_PB00();                                 // 设置OPA1OUT_PB00为模拟端口    
    
    OPA_InitStruct.InputP = OPA_INPUT_INP3;             // 正端输入源来自INP3
    OPA_InitStruct.InputN = OPA_INPUT_NONE;             // 负端输入源不连接IO
    OPA_InitStruct.Bias     = OPA_BIAS_8UA_4US;         // BIAS电流为8uA, AZ需要4us
    OPA_InitStruct.PgaGain  = OPA_PGA_GAIN2;            // 同相放大2倍
    OPA_InitStruct.WorkMode = OPA_WORKMODE_PGA;         // PGA模式
    
    OPA_Init(CW_OPA1, &OPA_InitStruct);
}

/**
* @brief 简单延时
 *
 * @param nCount
 */
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
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
