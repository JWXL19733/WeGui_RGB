示例目的：
        演示BTIM1的触发功能。

硬件资源：
          1. CW32L012 StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟2分频，48MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=48MHz
          4. BTIM1的时钟设置为PCLK，48分频
          5. LED2
          6. 按键KEY1
          7. PA06复用为BTIM1的ETR引脚

演示说明：
          将KEY1和BTIM1的ETR引脚用条线连接，按下KEY1后，触发BTIM1以1MHz时钟进行计数，ARR寄存器为49999，
          则BTIM1将每50ms溢出一次，并触发中断。中断服务程序中，LED1以10Hz的频率闪烁

使用说明
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
