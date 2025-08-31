示例目的：
         演示UART的同步通信功能

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟1分频，96MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=96MHz
          4. 板载打印串口，波特率115200， 1-8-N-1
          5. 板载LED1
          6. UART1的传输时钟设置为PCLK，波特率1Mbps，同步模式
          7. 板载SPI FLASH（W25Q64JV）
          8. PB02 Push-pull输出

演示说明：
          UART1同步模式下和SPI FLASH通信，读取FLASH的JEDEC ID的信息，并通过板载打印串口输出。完成后LED1闪烁。

硬件连接：
          UART1_TXD (PB01) -- SPI.SCK
          UART1_RXD (PB00) -- SPI.MOSI和SPI.MISO
          PB02             -- SPI.CS          

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
