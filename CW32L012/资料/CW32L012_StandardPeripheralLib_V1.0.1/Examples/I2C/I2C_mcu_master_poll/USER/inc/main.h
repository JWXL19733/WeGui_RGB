/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "cw32l012.h"
#include "system_cw32l012.h"
#include "interrupts_cw32l012.h"
#include "base_types.h"

#include "cw32l012_lpi2c.h"
#include "cw32l012_gpio.h"
#include "cw32l012_sysctrl.h"
#include "cw32l012_flash.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */


/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define LEN_DATA_RAM (16)
/* USER CODE END EM */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct _ramTesting_handle
{
    I2C_TypeDef *base;
    lpi2c_master_transfer_t xfer;
    uint8_t ramRx_buf[2 + LEN_DATA_RAM];
} ramTesting_handle_t;
/* USER CODE END ET */


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */


/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
static void I2C_Stop_SW(void);
void I2C_ExceptionHandling(void);
void I2C_InitBsp(void);
status_t RAM_TestingInit(ramTesting_handle_t *handle, I2C_TypeDef *base);
/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
