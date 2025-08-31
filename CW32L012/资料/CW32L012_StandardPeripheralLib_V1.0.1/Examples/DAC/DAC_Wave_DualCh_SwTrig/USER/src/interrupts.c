/**
  ******************************************************************************
  * @file    interrupts.c
  * @author  AE Team
  * @version 1.0
  * @date    2024-03-08
  * @brief   Interrupt Service Routines.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 SIYIMicro.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "interrupts.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* USER CODE END EV */


/******************************************************************************/
/*           Cortex-M0P Processor Interruption and Exception Handlers         */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn */

    /* USER CODE END NonMaskableInt_IRQn */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn */

    /* USER CODE END HardFault_IRQn */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn */

        /* USER CODE END W1_HardFault_IRQn */

    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn */

    /* USER CODE END SVCall_IRQn */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn */

    /* USER CODE END PendSV_IRQn */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{

    /* USER CODE BEGIN SysTick_IRQn */

    /* USER CODE END SysTick_IRQn */
}


/******************************************************************************/
/* SYM32L012 Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_sym32l012.s).                    */
/******************************************************************************/

/**
  * @brief This function handles WDT.
  */
void WDT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles LVD.
  */
void LVD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles RTC.
  */
void RTC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles FLASH and RAM.
  */
void FLASHRAM_IRQHandler(void)
{

    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles SYSCTRL.
  */
void SYSCTRL_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GPIOA.
  */
void GPIOA_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GPIOB.
  */
void GPIOB_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GPIOC.
  */
void GPIOC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GPIOF.
  */
void GPIOF_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles DMACH1 and DMACH2.
  */
void DMACH1_DMACH2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles DMACH3 and DMACH4.
  */
void DMACH3_DMACH4_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles ADC1.
  */
void ADC1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles ATIM.
  */
void ATIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles VC1 and VC3.
  */
void VC1_VC3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles VC2 and VC4.
  */
void VC2_VC4_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GTIM1.
  */
void GTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GTIM2.
  */
void GTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles GTIM3 and GTIM4.
  */
void GTIM3_GTIM4_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles LPTIM.
  */
void LPTIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles BTIM1.
  */
void BTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles BTIM2.
  */
void BTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles BTIM3 and HALLTIM.
  */
void BTIM3_HALLTIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles I2C1.
  */
void I2C1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles I2C2.
  */
void I2C2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles SPI1.
  */
void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles SPI2 and SPI3.
  */
void SPI23_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles UART1.
  */
void UART1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles UART2.
  */
void UART2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles UART3.
  */
void UART3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles ADC2 and DAC.
  */
void ADC2_DAC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
  * @brief This function handles CLKFAULT.
  */
void CLKFAULT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */


/************************ (C) COPYRIGHT SIYIMicro *****END OF FILE*************/
