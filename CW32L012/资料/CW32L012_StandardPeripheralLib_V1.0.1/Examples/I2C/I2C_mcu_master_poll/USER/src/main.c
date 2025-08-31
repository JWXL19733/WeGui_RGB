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
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint8_t u8DataSent[LEN_DATA_RAM] = {0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0};
uint8_t u8DataReceived[2 + LEN_DATA_RAM] = {0x00};
//...
static ramTesting_handle_t ramHandle;
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
//#define HSI48MHZ_EN

#ifndef HSI48MHZ_EN
#define HSI8MHZ_EN
#endif

//#define BAUD_RATE_100K_EN
//#define BAUD_RATE_1000K_EN

#define I2C1_SCL_GPIO_PORT                     CW_GPIOB
#define I2C1_SCL_GPIO_PIN                      GPIO_PIN_10
#define I2C1_SDA_GPIO_PORT                     CW_GPIOB
#define I2C1_SDA_GPIO_PIN                      GPIO_PIN_11

#define I2C1_SCL_LOW()                         PB10_SETLOW()
#define I2C1_SDA_LOW()                         PB11_SETLOW()
#define I2C1_SCL_HIGH()                        PB10_SETHIGH()
#define I2C1_SDA_HIGH()                        PB11_SETHIGH()

#define I2C1_SCL2GPIO()                        PB10_AFx_GPIO()
#define I2C1_SDA2GPIO()                        PB11_AFx_GPIO()
#define GPIO2I2C1_SCL()                        PB10_AFx_I2C1SCL()
#define GPIO2I2C1_SDA()                        PB11_AFx_I2C1SDA()

#define DEV_ADDR_SLAVE_7B                      0x70                 //I2C从机设备地址：7bits.
#define DEV_ADDR_SLAVE_10B                     0x3F0                //I2C从机设备地址：10bits.
#define RAM_ADDR_OFFSET_SLAVE                  0x0800               //I2C从机RAM目标地址:0x20000000 + 0x0800.


/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief main函数
 *
 * 初始化系统时钟、I2C的IO口和I2C模块，并通过I2C模块发送和接收数据。
 *
 * @return int32_t 返回0表示程序正常结束。
 */
int32_t main(void)
{
    uint16_t cntTemp = 0 ;
    uint8_t dataTemp[sizeof(u8DataSent) + 2];



	
	
    //时钟初始化
    SYSCTRL_Configuration();
		//SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1); // 8MHz
//__SYSCTRL_I2C1_CLK_ENABLE();

    // I2C的IO口初始化
   // GPIO_Configuration();
//	__SYSCTRL_GPIOB_CLK_ENABLE();
//	PB10_AFx_I2C2SCL();
//	PB11_AFx_I2C2SDA();
//	GPIO_InitTypeDef GPIO_InitStruct={0};
//	GPIO_InitStruct.Pins = GPIO_PIN_10|GPIO_PIN_11;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
 GPIO_InitTypeDef GPIO_InitStructure = {0};

    __SYSCTRL_GPIOB_CLK_ENABLE();

    GPIO2I2C1_SCL();
    GPIO2I2C1_SDA();
    GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);

    I2C_DeInit(CW_I2C1);

    //I2C_InitBsp();

	I2C_MasterConfigTypeDef I2C_InitStruct;
  I2C_MasterInitDefault(&I2C_InitStruct);
	I2C_MasterInit(CW_I2C1, &I2C_InitStruct, 96000000UL);
	I2C_MasterCmd(CW_I2C1, ENABLE); // I2C模块使能

    I2C_MasterCmd(CW_I2C1, ENABLE); // I2C模块使能

    if (I2C_MasterPollingFlag(CW_I2C1, I2C_IT_BUSBUSY) == I2C_Status_Success)
    {
        I2C_ExceptionHandling();
    }

    dataTemp[0] = (uint8_t)((RAM_ADDR_OFFSET_SLAVE & 0xFF00) >> 8); // 写数据高字节
    dataTemp[1] = (uint8_t)(RAM_ADDR_OFFSET_SLAVE & 0x00FF);        // 写数据低字节

    while (1)
    {
        memcpy(dataTemp + 2, u8DataSent, sizeof(u8DataSent));
#if 0
        /*I2C Polling method 1*/
        // 主机发送2 + X字节数据
        if(I2C_Status_Success == I2C_MasterSendDataToSlave(CW_I2C1, DEV_ADDR_SLAVE_7B, dataTemp, sizeof(dataTemp)))
        {
#ifdef HSI48MHZ_EN
            FirmwareDelay(1000); //@48MHz
#else
            FirmwareDelay(100); //@8MHz
#endif

            if(I2C_Status_Success != I2C_MasterReceiveDataFromSlave(CW_I2C1, DEV_ADDR_SLAVE_7B, dataTemp, I2C_REG_ADDR_16BIT, u8DataReceived, sizeof(u8DataReceived))) // 主机读取2+X字节数据
            {
                I2C_MasterSTOP(CW_I2C1);
            }
        }
        else
        {
            I2C_MasterSTOP(CW_I2C1);
        }

        I2C_MasterClearFlag(CW_I2C1, ~CLEAR_IT_MASK); // 清除I2C_MISR中断标志

        //检查Write/Read数据一致性
        if(0 == memcmp(dataTemp, u8DataReceived, sizeof(u8DataReceived)))
#else
        /*I2C Polling method 2*/
        /* Initialize RAM Testing */
        status_t status = RAM_TestingInit(&ramHandle, CW_I2C1);

        //检查Write/Read数据一致性
        if (0 == memcmp(dataTemp, &ramHandle.ramRx_buf[0], sizeof(ramHandle.ramRx_buf)))
#endif
        {
            //LED1亮/LED2灭  //PASSED
            PB09_SETHIGH();
            PB08_SETLOW();
        }
        else
        {
            //LED1灭/LED2亮  //FAILED
            PB09_SETLOW();
            PB08_SETHIGH();

            I2C_MasterSoftReset(CW_I2C1);
            // I2C初始化
            I2C_InitBsp();
            I2C_MasterCmd(CW_I2C1, ENABLE); // I2C模块使能

            if (I2C_MasterPollingFlag(CW_I2C1, I2C_IT_BUSBUSY) == I2C_Status_Success)
            {
                I2C_ExceptionHandling();
            }
        }

        ++cntTemp;
        for (uint8_t i = 0; i < sizeof(u8DataSent); ++i)
        {
            u8DataSent[i] = cntTemp + i;
        }

#ifdef HSI48MHZ_EN
        FirmwareDelay(3000);
#else
        FirmwareDelay(500);
#endif

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
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1); // 8MHz
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

//    /* Configure the GPIO_LED pin */
//    GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9 ;
//    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

//    //LEDs are off.
//    PB09_SETLOW();
//    PB08_SETLOW();
}

static void I2C_Stop_SW(void)
{
    I2C1_SCL_LOW();
    I2C1_SDA_LOW();
#ifdef HSI48MHZ_EN
    FirmwareDelay(24); // delay 4us@48MHz
    I2C1_SCL_HIGH();
    FirmwareDelay(24);
#else
    FirmwareDelay(4);  // delay 4us@8MHz
    I2C1_SCL_HIGH();
    FirmwareDelay(4);
#endif
    I2C1_SDA_HIGH();
}

void I2C_ExceptionHandling(void)
{
    I2C1_SCL2GPIO();
    I2C1_SDA2GPIO();
    I2C_Stop_SW();
#ifdef HSI48MHZ_EN
    FirmwareDelay(6000); // delay 1ms@48MHz
#else
    FirmwareDelay(1000); // delay 1ms@8MHz
#endif
    GPIO2I2C1_SCL();
    GPIO2I2C1_SDA();
}

void I2C_InitBsp(void)
{
    I2C_MasterConfigTypeDef I2C_InitStruct;
    // I2C初始化为默认值
    I2C_MasterInitDefault(&I2C_InitStruct);

#ifdef BAUD_RATE_100K_EN
    I2C_InitStruct.I2C_BaudRate_Hz = 100000UL;
#elif defined (BAUD_RATE_1000K_EN)
    I2C_InitStruct.I2C_BaudRate_Hz = 1000000UL;
#else
#endif

// 初始化模块
#ifdef HSI48MHZ_EN
    I2C_MasterInit(CW_I2C1, &I2C_InitStruct, 48000000UL);
#elif defined(HSI8MHZ_EN)
    I2C_MasterInit(CW_I2C1, &I2C_InitStruct, 96000000UL);
#else
#endif
}

/**
 * @brief 初始化RAM测试模块
 *
 * 该函数用于初始化RAM测试模块，配置I2C通信参数，并准备向从设备写入和读取数据。
 *
 * @param handle RAM测试模块的句柄，用于存储测试过程中需要的数据
 * @param base I2C通信的基地址
 *
 * @return 返回状态码，成功返回kStatus_Success，失败返回错误代码
 */
status_t RAM_TestingInit(ramTesting_handle_t *handle, I2C_TypeDef *base)
{
    lpi2c_master_transfer_t *xfer = &(handle->xfer);
    status_t status;

    assert_param(handle);
    assert_param(base);

    if (!handle || !base)
    {
        return kStatus_InvalidArgument;
    }

    handle->base = base;

    /* clear transfer structure and buffer */
    memset(xfer, 0, sizeof(*xfer));
    memset(handle->ramRx_buf, 0, LEN_DATA_RAM);

    /* set device mode to normal operation */
    xfer->slaveAddress   = DEV_ADDR_SLAVE_7B;
    xfer->direction      = I2C_MODE_WRITE;
    xfer->subaddress     = RAM_ADDR_OFFSET_SLAVE;
    xfer->subaddressSize = 2;
    xfer->data           = &u8DataSent;
    xfer->dataSize       = LEN_DATA_RAM;
    xfer->flags          = kLPI2C_TransferDefaultFlag;
    //Write data to slave RAM.
    status = LPI2C_MasterTransferBlocking(handle->base, &handle->xfer);

#ifdef HSI48MHZ_EN
    FirmwareDelay(1000); //@48MHz
#else
    FirmwareDelay(100); //@8MHz
#endif
    /* prepare transfer structure for reading ram data */
    xfer->slaveAddress = DEV_ADDR_SLAVE_7B;
    xfer->direction = I2C_MODE_READ;
    xfer->subaddress = RAM_ADDR_OFFSET_SLAVE;
    xfer->subaddressSize = 2;
    xfer->data = handle->ramRx_buf;
    xfer->dataSize = LEN_DATA_RAM + 2;
    xfer->flags = kLPI2C_TransferDefaultFlag;
    //Read data from slave RAM.
    status = LPI2C_MasterTransferBlocking(handle->base, &handle->xfer);
    return status;
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

