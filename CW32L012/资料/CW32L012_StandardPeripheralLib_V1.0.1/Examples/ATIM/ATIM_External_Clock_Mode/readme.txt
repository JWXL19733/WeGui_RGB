示例目的：
         通过配置 ATIM 的外部时钟模式2，使用外部信号作为计数器时钟源。

硬件资源：
          1. CW32L012 StartKit
          2. 系统时钟设置为HSI时钟
          3. 定时器的时钟源设置为外部时钟模式。
              预分频器（PSC）设置为0，表示无预分频。
              自动重载寄存器（ARR）设置为9。
          4. ATIM的ETR引脚（PB00）
          5. LED1         

演示说明：
         外部信号通过ETR引脚输入到ATIM模块，通过配置ATIM的外部时钟模式2，使用外部信号作为计数器时钟源。
         外部信号的上升沿触发计数器计数。计数器从0计数到自动重载值（9）时，会产生更新事件。
         更新事件触发中断，在中断服务程序中，将LED1的状态取反，然后清除中断标志位。


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
