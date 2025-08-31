示例目的：
            演示LPUART使用DMA（直接存储器访问）进行数据传输，提高数据传输效率，适用于需要高速数据传输的场景。

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟12分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK，波特率9600，无奇偶校验

演示说明：
         程序运行后，先通过DMA发送200个数据，然后通过DMA接收10个数据，再通过DMA将接收的数据发送。
          

硬件连接：
          UART3_TXD (PA8) -- PCRX
          UART3_RXD (PA9) -- PCTX  

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
