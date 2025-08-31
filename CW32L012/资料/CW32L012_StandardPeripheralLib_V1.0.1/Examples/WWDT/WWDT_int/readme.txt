示例目的：
          本样例演示 WWDT 定时产生中断，在中断服务程序中对PB09的输出电平进行翻转

硬件资源：
          1. CW32L012F8P6 StartKit
          2. 时钟HSI
          3. 系统时钟由HSI提供，分频为8MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=8MHz
          4. WWDT的时钟为PCLK/16384
          5. PB09设置为PUSH-PULL输出，驱动LED1          

演示说明：
          WWDT每129ms产生一次预溢出中断，在中断中喂狗，并翻转LED1的输出电平
          
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
