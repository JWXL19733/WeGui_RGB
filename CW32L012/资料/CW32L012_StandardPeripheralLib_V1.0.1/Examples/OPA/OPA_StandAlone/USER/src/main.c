/**
  ******************************************************************************
  * @file    main.c
  * @author  AE Team
  * @version 1.0
  * @date    2024-08-15
  * @brief   本样例演示OPA的独立运放功能
  *          由于开发板没有OPA的外围电路，因此只做代码配置演示，不做功能演示。
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
    // 初始化时钟为96MHz/12 = 8MHz；
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);   
    
    //初始化OPA
    OPA_Config();
    
    // 启动OPA
    OPA_Start(CW_OPA1);
    while(1)
    {
    }
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
    AFx_OPA1INN2_PA07();                                // 设置OPA1INN2_PA07为模拟端口
    AFx_OPA1OUT_PB00();                                 // 设置OPA1OUT_PB00为模拟端口    
    
    OPA_InitStruct.InputP = OPA_INPUT_INP3;             // 正端输入源来自INP3
    OPA_InitStruct.InputN = OPA_INPUT_INN2;             // 负端输入源不连接IO
    OPA_InitStruct.Bias     = OPA_BIAS_8UA_4US;         // BIAS电流为8uA, AZ需要4us    
    OPA_InitStruct.WorkMode = OPA_WORKMODE_STANDALONE;  // 独立运放模式，由外部电阻决定电路类型
    
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
