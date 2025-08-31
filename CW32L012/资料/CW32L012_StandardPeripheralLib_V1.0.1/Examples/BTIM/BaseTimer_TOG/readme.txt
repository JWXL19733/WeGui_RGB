示例目的：
          BTIM的引脚翻转功能

硬件资源：
          1. CW32L012 StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟32分频，3MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=3MHz
          4. BTIM1的时钟设置为PCLK，1分频，ARR设置为0.
          5. BTIM1的TOGP（PA10）和TOGN（PA09）引脚
          6. PCLKOUT引脚（PA03）

演示说明：
           BTIM1以3MHz时钟频率计数，ARR设置为0，BTIM1的TOG引脚每周期翻转一次。可观测PCLKOUT引脚，BTIM1的TOGP和TOGN引脚上的波形。

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
