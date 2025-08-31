示例目的：
          系统时钟源在HSI和LSE之间进行切换。

硬件资源：
          1. CW32L012 StartKit  
          2. 时钟HSI和LSI
          3. HSI作为系统时钟时，设置为HSIOSC到HSI时钟的分频比为24，则SysClk为4MHz，
             PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. LSE作为系统时钟时，32.768K。PCLK、HCLK不分频，PCLK=HCLK=SysClk=32.768KHz。

演示说明：
          程序循环在HSI和LSE时钟之间进行切换，可看到在不同时钟下，LED1闪烁频率不一样，如果切换失败，则LED1灯常亮。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开rcc_hsi_lse.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
