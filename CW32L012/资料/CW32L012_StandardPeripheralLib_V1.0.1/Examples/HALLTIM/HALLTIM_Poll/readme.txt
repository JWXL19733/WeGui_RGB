示例目的：
          演示HALL定时器功能

硬件资源：
          1. CW32L012FxPx StartKit
          2. 时钟HSI
          3. 系统工作频率位HSI默认分频24分频，96MHz/24=4MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=4MHz
          4. HILLTIM的ARR设置10000-1，则HILLTIM能测量最长9999个PCLK时钟周期的输入脉宽          
          5. PA03、04、05作为HILLTIM的输入
          6. PB00作为HILLTIM的输出
          7. PB01作为HILLTIM的TRGO输出

演示说明：
          将霍尔传感器接入到PA03、04、05，程序运行后可测量三个输入信号之间变化沿之间的间隔。
          同时PB00输出占空比位50%的PWM的波， PB01输出比较匹配时刻。
          
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
