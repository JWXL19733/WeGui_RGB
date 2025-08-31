示例目的：
          使用 DAC 生成一个参考电压，然后利用电压比较器 VC 将该参考电压与其他输入电压进行比较。

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. VC的滤波时钟源为PCLK
          5. PB09设置为PUSH-PULL输出

演示说明：
          VC输入P通道为PA02，N通道为DAC1内部输出，VC输出端口为PA00。
          PA02输入一个电压信号，作为VC的正端输入，其负端输入内DAC的通道一输出。DAC的输出每50ms变化一次，
          当DAC的输出电压高于PA02上的电压后，VC产生中断，PB09（LED）输出高电平，LED1亮，否则LED1灭。
		  

          
使用说明：
+ EWARM
          1. 打开vc_detect_falling_irq.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开vc_detect_falling_irq.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)