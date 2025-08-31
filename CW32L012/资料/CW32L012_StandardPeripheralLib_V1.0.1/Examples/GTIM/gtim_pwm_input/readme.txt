示例目的：
          演示GTIM的PWM输入捕获功能

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，1分频，96MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=96MHz
          4.  GTIM1的分频系数1，以96MHz的频率计数，ARR设置最大0xFFFF，将CH1映射到IC1和IC2上，
             IC1上升沿捕获，IC2下降沿捕获             
          5. PA06配置为GTIM1的CH1输入通道。
          6. 板载串口 115200bps
          7. PA04输出HCLK/64，模拟测试信号

演示说明：
           将PWM输入信号引入到PA06上。使用GTIM1的CH1单个通道对PWM输入信号进行测量,CCR1寄存器测量周期，
           CCR2寄存器测量脉宽。
      
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