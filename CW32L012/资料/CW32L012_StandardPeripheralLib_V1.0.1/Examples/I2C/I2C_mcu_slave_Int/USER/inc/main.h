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
/* USER CODE BEGIN EFP */
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void I2C_ExceptionHandling(void);
uint8_t I2C_SlaveReceiveDataFromMaster(I2C_TypeDef *I2Cx, uint8_t *pu8RegAddr, I2C_SlaveRegAddrTypeDef u8RegAddrType,uint8_t *pu8Data, uint16_t *pu16Len);
uint8_t I2C_SlaveSendDataToMaster(I2C_TypeDef *I2Cx, uint8_t *pu8Data, uint16_t u16Len);
//...
void LPI2C_SlaveIRQHandler(I2C_TypeDef *I2Cx, uint8_t *pu8RegAddr, I2C_SlaveRegAddrTypeDef u8RegAddrType, uint8_t *pu8Data, uint16_t *pu16Len, uint16_t *pLenTx);
uint8_t I2C_SlaveReceiveDataISR(I2C_TypeDef *I2Cx, uint8_t *pu8RegAddr, I2C_SlaveRegAddrTypeDef u8RegAddrType, uint8_t *pu8Data, uint16_t *pu16Len);

/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
