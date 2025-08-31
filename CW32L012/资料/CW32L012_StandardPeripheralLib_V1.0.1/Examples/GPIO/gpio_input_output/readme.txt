示例目的：
          演示GPIO通过库函数和宏定义的方法完整初始化配置和输入输出的读写操作

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟24分频，4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. PB08 / PB09推挽输出，PA00上拉输入

演示说明：
         按下按键KEY1(PA00),观察 PB08 / PB09电平。

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