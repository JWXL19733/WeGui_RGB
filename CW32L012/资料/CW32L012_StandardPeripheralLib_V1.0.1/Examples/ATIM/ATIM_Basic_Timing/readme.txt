示例目的：
         演示ATIM作为基础定时器的使用方法。

硬件资源：
          1. CW32L012 StartKit
          2. 系统时钟设置为HSI时钟
          3. 定时器的时钟源设置为系统时钟（HSI）。
              预分频器（PSC）设置为0，表示无预分频。
              自动重载寄存器（ARR）设置为49999，表示PWM信号的周期为50000个时钟周期，频率为20Hz。
          4. LED1(PB09)         

演示说明：
         LED1以10Hz的频率闪烁。

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
