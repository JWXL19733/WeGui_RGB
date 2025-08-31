/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-7-12
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
#define _MAIN

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "..\inc\main.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//#define HSI48MHZ_EN

#ifndef HSI48MHZ_EN
#define HSI8MHZ_EN
#endif

//#define BAUD_RATE_100K_EN
//#define BAUD_RATE_1000K_EN

//#define ADDR_SLAVE_10B_EN

#define I2C1_SCL_GPIO_PORT                     CW_GPIOB
#define I2C1_SCL_GPIO_PIN                      GPIO_PIN_6
#define I2C1_SDA_GPIO_PORT                     CW_GPIOB
#define I2C1_SDA_GPIO_PIN                      GPIO_PIN_7

#define I2C1_SCL_LOW()                         PB06_SETLOW()
#define I2C1_SDA_LOW()                         PB07_SETLOW()
#define I2C1_SCL_HIGH()                        PB06_SETHIGH()
#define I2C1_SDA_HIGH()                        PB07_SETHIGH()

#define I2C1_SCL2GPIO()                        PB06_AFx_GPIO()
#define I2C1_SDA2GPIO()                        PB07_AFx_GPIO()
#define GPIO2I2C1_SCL()                        PB06_AFx_I2C1SCL()
#define GPIO2I2C1_SDA()                        PB07_AFx_I2C1SDA()

#define DEV_ADDR_SLAVE_7B                      0x70                 //I2C从机设备地址：7bits.
#define DEV_ADDR_SLAVE_10B                     0x3F0                //I2C从机设备地址：10bits.
#define RAM_ADDR_OFFSET_SLAVE                  0x0800               //I2C从机RAM目标地址:0x20000000 + 0x0800.
#define RAM_ADDR_BASE_SLAVE                    0x20000000

#define BUFFER_SIZE_TX                         16UL


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8RegAddr[MAX_SIZE_SUBADDRESS];
uint16_t u16LenReceived, u16LenSent;
//...
size_t cntTx;
uint8_t addrRx;

#if defined ( __CC_ARM )
uint8_t u8DataReceived[2 + 16]  __attribute__((at(RAM_ADDR_BASE_SLAVE + RAM_ADDR_OFFSET_SLAVE)));
#elif defined  ( __ICCARM__ )
uint8_t u8DataReceived[2 + 16 + 2]  @ (RAM_ADDR_BASE_SLAVE + RAM_ADDR_OFFSET_SLAVE);
#elif defined   ( __GNUC__ )
uint8_t u8DataReceived[2 + 16]  __attribute__((section(".ARM.__at_(RAM_ADDR_BASE_SLAVE + RAM_ADDR_OFFSET_SLAVE)")));
#endif
/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);


/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
lpi2c_slave_handle_t slaveHandle;
lpi2c_slave_transfer_t slaveXfer;
uint8_t u8StateMathine;
volatile boolean_t g_ReceiveDone = FALSE;

void LPI2C_SlaveIRQHandler(I2C_TypeDef *I2Cx, uint8_t *pu8RegAddr, I2C_SlaveRegAddrTypeDef u8RegAddrType, uint8_t *pu8Data, uint16_t *pu16Len, uint16_t *pLenTx)
{
    uint32_t f_ISR = 0;
    uint32_t timeout = 0x10;

    // Step 1: Slave address is valid.
    // Step 2: Wait for slave register address & user data.

    do
    {
        I2C_SlaveReceiveDataISR(I2Cx, u8RegAddr, u8RegAddrType, pu8Data, pu16Len);
        f_ISR = I2C_SlaveGetFlag(I2Cx, I2C_IT_STOP_SLV | I2C_IT_RESTART_SLV);

    } while ((0 == f_ISR) && (--timeout > 0));

    if (0 == timeout)
    {
        return;
    }

    if ((f_ISR & I2C_IT_STOP_SLV) > 0)
    {
        *pLenTx = *pu16Len;
    }

    else if ((f_ISR & I2C_IT_RESTART_SLV) > 0)
    {
        // Slave transmit mode
        CW_I2C1->SICR = 0;
        I2C_SlaveSendDataToMaster(CW_I2C1, u8DataReceived, *pLenTx);
        CW_I2C1->SICR = 0;

        f_ISR = 0;
    }
    else
    {
    }
    CW_I2C1->SICR = 0;
}



void LPI2C_SlaveInit(void)
{
    I2C_SlaveConfigTypeDef I2C_InitStruct;

    I2C_SlaveDeinit(CW_I2C1);

    // I2C default values
    I2C_SlaveInitDefault(&I2C_InitStruct);

    I2C_InitStruct.I2C_Addr0 = DEV_ADDR_SLAVE_7B;
    I2C_InitStruct.I2C_RegAddr = I2C_REG_ADDR_16BIT;

#ifdef BAUD_RATE_100K_EN
    I2C_InitStruct.clockHoldTime_ns = 250U;
#elif defined(BAUD_RATE_1000K_EN)
    I2C_InitStruct.clockHoldTime_ns = 50U;
#else
    I2C_InitStruct.clockHoldTime_ns = 100U;
#endif

    // 初始化模块
#ifdef HSI48MHZ_EN
    I2C_SlaveInit(CW_I2C1, &I2C_InitStruct, 48000000UL);
#elif defined (HSI8MHZ_EN)
    I2C_SlaveInit(CW_I2C1, &I2C_InitStruct, 8000000UL);
#else
    //...
#endif

}

/**
 * @brief main entry point for the application.
 *
 * @return int32_t
 */
int32_t main(void)
{
    // 时钟初始化
    SYSCTRL_Configuration();

    // IO口初始化
    GPIO_Configuration();

    // I2C从设备初始化
    LPI2C_SlaveInit();

    //中断设置
    NVIC_Configuration();
    I2C_SlaveCmd(CW_I2C1, ENABLE); // I2C模块使能

    if (I2C_SlavePollingFlag(CW_I2C1, I2C_IT_BUSBUSY | I2C_IT_SLVBUSY_SLV) == I2C_Status_Success)
    {
        I2C_ExceptionHandling();
    }

    /* Enable LPI2C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
    LPI2C_SlaveEnableInterrupts(CW_I2C1, (uint32_t)kLPI2C_SlaveIrqFlags);

    I2C_SlaveCmd(CW_I2C1, ENABLE); // I2C模块使能

    while (1)
    {

        FirmwareDelay(0x100000);
        PB09_TOG();
    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
#ifdef HSI48MHZ_EN
    __SYSCTRL_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_2);
    CW_FLASH->CR2 |= (0x5A5A0000 | 0x18);    // Enable FLASH Prefetch and Cache.
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV2); // 48MHz
    __SYSCTRL_FLASH_CLK_DISABLE();
#elif defined(HSI8MHZ_EN)
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12); // 8MHz
#else
// Default HSI4MHz
#endif
    __SYSCTRL_I2C1_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __SYSCTRL_GPIOB_CLK_ENABLE();

    GPIO2I2C1_SCL();
    GPIO2I2C1_SDA();
    GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9 ;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    //LEDs are off.
    PB09_SETLOW();
    PB08_SETLOW();
}

void I2C_ExceptionHandling(void)
{
    //异常处理
    //while(1);
}

uint8_t I2C_SlaveSendDataToMaster(I2C_TypeDef *I2Cx, uint8_t *pu8Data, uint16_t u16Len)
{
    uint8_t u8State;
    uint16_t data0;

    // 使能应答标志
    I2C_SlaveTxNAck(I2Cx, DISABLE);

#if 1
    // 查询等待I2C_SISR.RESTART , I2C_SISR.ADDR 变为 1
    while (I2C_SlavePollingFlag(I2Cx, I2C_IT_RESTART_SLV | I2C_IT_ADDR_SLV) != I2C_Status_Success)
        ;

#else
    if (I2C_SlavePollingFlag(I2Cx, I2C_IT_RESTART_SLV | I2C_IT_ADDR_SLV) != I2C_Status_Success)
    {
        return I2C_Status_Timeout; // 超时错误
    }

#endif

    data0 = (uint16_t)I2Cx->SASR_f.RADDR; // 读取 I2C_SASR 寄存器，自动清除 I2C_SISR.ADDR 标志

    u8State = I2C_SlaveSend(I2Cx, pu8Data, u16Len);

    if (u8State != I2C_Status_Success)
    {
        return u8State;
    }

    // 返回 NACK,释放总线
    if (1 == I2Cx->SCR1_f.ACKSTALL)
    {
        I2C_SlaveTxNAck(I2Cx, ENABLE);
    }

    return I2C_Status_Success;
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_SetPriority(I2C1_IRQn, 3); //Lowest priority
    NVIC_ClearPendingIRQ(I2C1_IRQn);
    NVIC_EnableIRQ(I2C1_IRQn);
    __enable_irq();
}


uint8_t I2C_SlaveReceiveDataISR(I2C_TypeDef *I2Cx, uint8_t *pu8RegAddr, I2C_SlaveRegAddrTypeDef u8RegAddrType, uint8_t *pu8Data, uint16_t *pu16Len)
{
    uint8_t u8State;
    uint16_t data0;
    boolean_t modeRead;
    uint8_t i, j;

    //清除I2C从机标志位
    CW_I2C1->SICR = 0;
    // 使能应答标志
    I2C_SlaveTxNAck(I2Cx, DISABLE);

    data0 = (uint16_t)I2Cx->SASR_f.RADDR; // 读取 I2C_SASR 寄存器，自动清除 I2C_SISR.ADDR 标志
    modeRead = (boolean_t)(data0 & 0x01);
    data0 = (uint16_t)(data0 >> 1); // 读取从机地址

    //清除I2C从机标志位
    CW_I2C1->SICR = 0; //I2C_SlaveClearFlag(I2Cx, ~CLEAR_IT_MASK_SLV);

    u8State = I2C_SlaveReceive(I2Cx, pu8Data, pu16Len);

    if (u8State != I2C_Status_Success)
    {
        return u8State;
    }

    if (modeRead == FALSE)
    {
        /*I2C write-mode:
        Register address is at 1st or 1st & 2nd byte received;
        Other bytes received are user data.
        */
        // Save register address to local variable
        *pu8RegAddr = *pu8Data;

        switch (u8RegAddrType)
        {
        case I2C_REG_ADDR_8BIT:
            j = 1;
            break;
        case I2C_REG_ADDR_16BIT:
            j = 2;
            break;
        case I2C_REG_ADDR_24BIT:
            j = 3;
            break;
        case I2C_REG_ADDR_32BIT:
            j = 4;
            break;
        default:
            return I2C_Status_Error;
        }

        if (j > 1)
        {
            if (*pu16Len >= j) // 检查是否有足够的数据长度
            {
                i = j - 1;
                while (i--)
                {
                    ++pu8Data;
                    *(pu8RegAddr + 1) = *pu8Data;
                }
            }
            else
            {
                return I2C_Status_Error; // 数据长度不足
            }
        }
    }
    else
    {
        // I2C read-mode
        uint8_t f_Stop = 0;
        uint32_t addrReg = (((uint32_t) * pu8RegAddr) << 8) + (*(pu8RegAddr + 1)) + RAM_ADDR_BASE_SLAVE;
        uint8_t *pData = (uint8_t *)&addrReg;

        do
        {
            // 等待TXE标志位被置位，表示发送缓冲区空闲
            if (I2C_SlavePollingFlag(I2Cx, I2C_IT_TXE_SLV) != I2C_Status_Success)
            {
                return I2C_Status_Timeout; // 超时错误
            }

            // 发送数据
            I2Cx->STDR = *pData++;

            if (I2C_SlaveGetFlag(I2Cx, I2C_IT_STOP_SLV))
            {
                f_Stop = 1;
            }

        }
        while (f_Stop == 0);

        // 返回 NACK,释放总线
        if (1 == I2Cx->SCR1_f.ACKSTALL)
        {
            I2C_SlaveTxNAck(I2Cx, ENABLE);
        }

    }

    return I2C_Status_Success;
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
	  __NOP();
	  while(1);
    /* USER CODE END 6 */
}

#endif

