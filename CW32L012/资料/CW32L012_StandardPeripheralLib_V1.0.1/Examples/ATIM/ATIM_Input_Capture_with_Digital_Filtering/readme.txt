示例目的：
         展示如何使用ATIM的输入捕获功能来测量外部信号的周期和脉冲宽度。

硬件资源：
          1. CW32L012 StartKit
          2. 系统时钟设置为HSI时钟
          3. 定时器的时钟源设置为系统时钟（HSI）。
              预分频器（PSC）设置为0，表示无预分频。
              自动重载寄存器（ARR）设置为0xFFFF，
          4. PA05 作为ATIM的捕获输入
          

演示说明：
         将外部信号连接到 ATIM 的输入捕获通道，配置捕获模式和滤波参数，当外部信号的边沿触发捕获时，
         读取计数器的值并记录到捕获比较寄存器中，通过计算两个捕获值之间的差值得到信号的周期或脉冲宽度，进而计算出信号的频率。
         

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
