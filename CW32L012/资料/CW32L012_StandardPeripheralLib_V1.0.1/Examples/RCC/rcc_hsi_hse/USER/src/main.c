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
#include "cw32l012_sysctrl.h"
#include "cw32l012_gpio.h"
#include "cw32l012_flash.h"
#include "cw32l012_systick.h"


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
 ** This sample HSI switch to HSE, HSE switch to HSI.
 **
 ******************************************************************************/
int32_t main(void)
{
    uint8_t res = 0;
    GPIO_InitTypeDef GPIO_InitStruct;

    // HSE PIN config
    GPIO_InitStruct.Pins = HSE_PIN_IN | HSE_PIN_OUT;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_Init(HSE_PIN_PORT, &GPIO_InitStruct);

    // PCLK PIN Config
    GPIO_InitStruct.Pins = PCLK_OUT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(PCLK_OUT_PIN_PORT, &GPIO_InitStruct);
    GPIO_SetAlternateFunction(PCLK_OUT_PIN_PORT, PCLK_OUT_PIN, PCLK_OUT_PIN_AF);

    // MCO PIN Config
    GPIO_InitStruct.Pins = MCO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(MCO_PIN_PORT, &GPIO_InitStruct);
    GPIO_SetAlternateFunction(MCO_PIN_PORT, MCO_PIN, MCO_PIN_AF);

    // LED init
    Bsp_Led_Init(&LED1);
    Bsp_Led_Init(&LED2);

    while (1)
    {
        //============================================================================
        //以下从HSI切换到HSE
        SYSCTRL_HSE_Enable( SYSCTRL_HSE_MODE_OSC, 16000000, SYSCTRL_HSE_DRIVER_LEVEL5, 0); //开外部高速时钟，实际频率需要根据实际晶体频率重新配置
        res = SYSCTRL_SysClk_Switch( SYSCTRL_SYSCLKSRC_HSE );                         //切换系统时钟到外部高速时钟

        if ( res == 0x00 )                                                    //切换系统时钟成功
        {
            SYSCTRL_HSI_Disable();
            CW_SYSCTRL->MCO_f.SOURCE = 5;//MCO时钟源为HSE，通过MCO_PIN输出来观察HSE
            InitTick(16000000);
            res = 10;

            while (res--)
            {
                Bsp_Led_On(&LED1);
                SysTickDelay(100);
                Bsp_Led_Off(&LED1);
                SysTickDelay(100);
            }

        }
        else
        {
            while (1)
            {
                Bsp_Led_On(&LED1);
                SysTickDelay(1000);
                Bsp_Led_Off(&LED1);
                SysTickDelay(1000);
            }
        }


        //============================================================================
        //以下从HSE切换到HSI
        SYSCTRL_HSI_Enable( SYSCTRL_HSIOSC_DIV24 );             //开启内部高速时钟HSI = HSIOSC / 24
        res = SYSCTRL_SysClk_Switch( SYSCTRL_SYSCLKSRC_HSI );   //切换系统时钟到内部高速时时钟

        if ( res == 0x00 )                                      //切换系统时钟成功
        {
            SYSCTRL_HSE_Disable();
            CW_SYSCTRL->MCO_f.SOURCE = 3;//MCO时钟源为HSIOSC，通过PB03的MCO输出来观察HSIOSC            
            InitTick(4000000);
            res = 10;

            while (res--)
            {
                Bsp_Led_On(&LED1);
                SysTickDelay(400);
                Bsp_Led_Off(&LED1);
                SysTickDelay(400);
            }
        }
        else
        {
            while (1)
            {
                Bsp_Led_On(&LED1);
                SysTickDelay(1000);
                Bsp_Led_Off(&LED1);
                SysTickDelay(1000);
            }
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
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

