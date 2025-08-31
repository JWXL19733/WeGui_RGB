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
#include "..\inc\main.h"

#define HSI48MHZ_EN

#ifndef HSI48MHZ_EN
#define HSI8MHZ_EN
#endif

//#define BAUD_RATE_100K_EN

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

// EEPROM内部地址
#define START_ADDR_EE                          0   // 写入或者读取的EEPROM的地址
#define BUFFER_SIZE                            8
#define TEST_CNT                               16 // 写读次数

uint8_t g_TxBuffer[BUFFER_SIZE] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};
uint8_t g_RxBuffer[BUFFER_SIZE];
lpi2c_master_handle_t masterHandle = {0};
lpi2c_master_transfer_t masterXfer = {0};
uint8_t u8StateMathine;
volatile boolean_t g_TransferDone = FALSE;


void LPI2C_MasterIRQHandler(void)
{
    LPI2C_MasterTransferHandleIRQ(CW_I2C1, &masterHandle);
}

void LPI2C_MasterCallback(I2C_TypeDef *I2Cx, lpi2c_master_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_Success == status)
    {
        g_TransferDone = TRUE;
    }
    else
    {
        // 错误处理
    }
}

void LPI2C_MasterInit(void)
{
    I2C_MasterConfigTypeDef I2C_InitStruct;

    I2C_MasterDeinit(CW_I2C1);

    // I2C default values
    I2C_MasterInitDefault(&I2C_InitStruct);
#ifdef BAUD_RATE_100K_EN
    I2C_InitStruct.I2C_BaudRate_Hz = 100000UL;
#endif

    // 初始化模块
#ifdef HSI48MHZ_EN
    I2C_MasterInit(CW_I2C1, &I2C_InitStruct, 48000000UL);
#elif defined (HSI8MHZ_EN)
    I2C_MasterInit(CW_I2C1, &I2C_InitStruct, 8000000UL);
#else
#endif

}

void LPI2C_MasterTransferIT(void)
{
    u8StateMathine = kIdleState;

    masterXfer.data = &g_TxBuffer[0];
    masterXfer.dataSize = BUFFER_SIZE;
    masterXfer.direction = I2C_MODE_WRITE;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;
    masterXfer.slaveAddress = I2C_SLAVEADDRESS_EEPROM;
    masterXfer.subaddress = START_ADDR_EE;
    masterXfer.subaddressSize = 1;

    masterHandle.state = kIdleState;
    masterHandle.remainingBytes = BUFFER_SIZE;
    masterHandle.buf = &u8StateMathine;
    masterHandle.userData = NULL;
    masterHandle.commandBuffer[0] = (I2C_SLAVEADDRESS_EEPROM << 1) | I2C_MODE_WRITE;
    masterHandle.commandBuffer[1] = START_ADDR_EE;
    masterHandle.commandBuffer[2] = NULL;
    masterHandle.commandBuffer[3] = NULL;
    masterHandle.commandBuffer[4] = NULL;
    masterHandle.commandBuffer[5] = NULL;
    masterHandle.completionCallback = LPI2C_MasterCallback;
    masterHandle.transfer = masterXfer;

    LPI2C_MasterTransferCreateHandle(CW_I2C1, &masterHandle, LPI2C_MasterCallback, NULL);
    LPI2C_MasterTransferNonBlocking(CW_I2C1, &masterHandle, &masterXfer);
}

void LPI2C_MasterReceiveIT(void)
{
    u8StateMathine = kIdleState;

    masterXfer.data = &g_RxBuffer[0];
    masterXfer.dataSize = BUFFER_SIZE;
    masterXfer.direction = I2C_MODE_READ;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;
    masterXfer.slaveAddress = I2C_SLAVEADDRESS_EEPROM;
    masterXfer.subaddress = START_ADDR_EE;
    masterXfer.subaddressSize = 1;

    masterHandle.state = kIdleState;
    masterHandle.remainingBytes = BUFFER_SIZE;
    masterHandle.buf = &u8StateMathine;
    masterHandle.userData = NULL;
    masterHandle.commandBuffer[0] = (I2C_SLAVEADDRESS_EEPROM << 1) | I2C_MODE_WRITE;
    masterHandle.commandBuffer[1] = START_ADDR_EE;
    masterHandle.commandBuffer[2] = NULL;
    masterHandle.commandBuffer[3] = NULL;
    masterHandle.commandBuffer[4] = NULL;
    masterHandle.commandBuffer[5] = NULL;
    masterHandle.completionCallback = LPI2C_MasterCallback;
    masterHandle.transfer = masterXfer;

    LPI2C_MasterTransferCreateHandle(CW_I2C1, &masterHandle, LPI2C_MasterCallback, NULL);
    LPI2C_MasterTransferNonBlocking(CW_I2C1, &masterHandle, &masterXfer);
}

/**
* @brief main函数
*
* 本函数实现了I2C通信的初始化及数据读写操作。
*
* @return int32_t 返回0表示程序正常结束
*/
int32_t main(void)
{
    uint16_t tempCnt = 0;

    // 时钟初始化
    SYSCTRL_Configuration();

    // IO口初始化
    GPIO_Configuration();

    // I2C主设备初始化
    LPI2C_MasterInit();

    // 中断设置
    NVIC_Configuration();
    I2C_MasterCmd(CW_I2C1, ENABLE); // I2C模块使能
    if (I2C_MasterPollingFlag(CW_I2C1, I2C_IT_BUSBUSY) == I2C_Status_Success)
    {
        I2C_ExceptionHandling();
    }

    while (1)
    {
        LPI2C_MasterTransferIT(); // I2C数据写操作
        while (!g_TransferDone)
        {
            ;
        }
        // Data transfer is ok, then read eeprom data.
#ifdef HSI48MHZ_EN
        FirmwareDelay(30000); //@48MHz
#else
        FirmwareDelay(5000); //@8MHz
#endif

        g_TransferDone = 0;
        LPI2C_MasterReceiveIT(); // I2C数据读操作
        while (!g_TransferDone)
        {
            ;
        }
        // 检查Write/Read数据一致性
        if (0 == memcmp(g_TxBuffer, g_RxBuffer, BUFFER_SIZE))
        {
            // LED1亮:PASSED
            PB09_SETHIGH();
        }
        else
        {
            // LED1灭:FAILED
            PB09_SETLOW();
        }

        ++tempCnt;
        for (uint8_t i = 0; i < 8; ++i)
        {
            g_TxBuffer[i] = tempCnt + i;
        }
        if (tempCnt >= TEST_CNT)
        {
            break;
        }

#ifdef HSI48MHZ_EN
        FirmwareDelay(30000); //@48MHz
#else
        FirmwareDelay(5000); //@8MHz
#endif
    }

    while (1)
    {
    }
}

/**
* @brief  Configures the different system clocks.
* @param  None
* @retval None
*/
void SYSCTRL_Configuration(void)
{
#ifndef HSI48MHZ_EN
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12); // 8MHz
#else
    __SYSCTRL_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_2);
    CW_FLASH->CR2 |= (0x5A5A0000 | 0x18); //Enable FLASH Prefetch and Cache.
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV2); // 48MHz
    __SYSCTRL_FLASH_CLK_DISABLE();
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
    GPIO_InitStructure.Pins = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    //LED灭
    PB09_SETLOW();
}

static void I2C_Stop_SW(void)
{
    I2C1_SCL_LOW();   // SCL = 0
    I2C1_SDA_LOW();   // SDA = 0
    FirmwareDelay(4); // delay 4us@8MHz
    I2C1_SCL_HIGH();  // SCL = 1
    FirmwareDelay(4); // delay 4us@8MHz
    I2C1_SDA_HIGH();  // SDA = 1
}

void I2C_ExceptionHandling(void)
{
    I2C1_SCL2GPIO();
    I2C1_SDA2GPIO();
    I2C_Stop_SW();
    FirmwareDelay(1000); // delay 1ms@8MHz
    GPIO2I2C1_SCL();
    GPIO2I2C1_SDA();
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



/******************************************************************************
* EOF (not truncated)
******************************************************************************/
#ifdef USE_FULL_ASSERT
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
    __NOP();
    while(1);    
    /* USER CODE END 6 */
}
#endif
