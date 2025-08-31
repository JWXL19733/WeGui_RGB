示例目的：
            UART带奇偶校验以查询方式发送和接收数据示例。

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟12分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK，波特率115200, 偶校验

演示说明：
          程序以偶校验的方式向串口助手发送字符‘A’，如果串口助手也以相同的校验发送字符‘A'，则会打印“Data received correctly: A”
          否则打印Parity Error或Data mismatch: sent A, received X。

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
