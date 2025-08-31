/**
  ******************************************************************************
  * @file    interrupts.h
  * @author  AE Team
  * @version 1.0
  * @date    2024-03-08
  * @brief   This file contains the headers of the interrupt handlers.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l012.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void WDT_IRQHandler(void);
extern void LVD_IRQHandler(void);
extern void RTC_IRQHandler(void);
extern void FLASHRAM_IRQHandler(void);
extern void SYSCTRL_IRQHandler(void);
extern void GPIOA_IRQHandler(void);
extern void GPIOB_IRQHandler(void);
extern void GPIOC_IRQHandler(void);
extern void GPIOF_IRQHandler(void);
extern void DMACH1_DMACH2_IRQHandler(void);
extern void DMACH3_DMACH4_IRQHandler(void);
extern void CORDIC_IRQHandler(void);
extern void ADC1_IRQHandler(void);
extern void ATIM_IRQHandler(void);
extern void VC1_VC3_IRQHandler(void);
extern void VC2_VC4_IRQHandler(void);
extern void GTIM1_IRQHandler(void);
extern void GTIM2_IRQHandler(void);
extern void GTIM3_GTIM4_IRQHandler(void);
extern void LPTIM_IRQHandler(void);
extern void BTIM1_IRQHandler(void);
extern void BTIM2_IRQHandler(void);
extern void BTIM3_HALLTIM_IRQHandler(void);
extern void I2C1_IRQHandler(void);
extern void I2C2_IRQHandler(void);
extern void SPI1_IRQHandler(void);
extern void SPI23_IRQHandler(void);
extern void UART1_IRQHandler(void);
extern void UART2_IRQHandler(void);
extern void UART3_IRQHandler(void);
extern void ADC2_DAC_IRQHandler(void);
extern void CLKFAULT_IRQHandler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */


#ifdef __cplusplus
}
#endif

#endif /* __INTERRUPTS_H */


/************************ (C) COPYRIGHT SIYIMicro *****END OF FILE*************/
