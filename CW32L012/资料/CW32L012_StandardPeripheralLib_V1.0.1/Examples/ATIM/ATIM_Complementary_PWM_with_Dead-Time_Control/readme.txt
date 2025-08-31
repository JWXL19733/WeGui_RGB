示例目的：
          演示ATIM的PWM互补输出功能

硬件资源：
          1. CW32L012 StartKit
          2. 系统时钟设置为HSI时钟1分频，96MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=96MHz         
          3. ATIM中心对齐模式计数，计数时钟96MHz,ARR设置为2399，单边溢出周期25us，
             中心对齐共50us，PWM周期20kHz, 输出比较, 使用ATIM的CH1和CH1N,
             CH2和CH2N，CH3和CH3N， CH4和CH4N，CH5和CH5N通道输出互补的PWM信号


演示说明:
         PB02：CH1 和 PA07：CH1N
         PB10：CH2 和 PB00：CH2N
         PB11：CH3 和 PB01：CH3N
         PA03：CH4 和 PA02：CH4N
         PF06：CH5 和 PF07：CH5N 互为互补输出，占空比分别为12.5%， 25%， 37.5%，50%，62.5%
         并插入不对称的死区时间。

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
