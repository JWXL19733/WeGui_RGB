示例目的：
           演示LPUART在中断驱动模式下的发送和接收功能，通过中断处理接收和发送数据，适用于需要低延迟和高效率的场景。

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟12分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK，波特率9600，无奇偶校验

演示说明：
          程序运行后，串口发送字符串“1234567890”和“\r\n\0"不可显示字符共13个字符，后等待接收到13个字符，然后继续将接收到的字
          符通过串口发送出来。

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
