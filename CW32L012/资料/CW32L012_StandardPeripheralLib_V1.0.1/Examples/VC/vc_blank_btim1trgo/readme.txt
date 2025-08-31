示例目的：
          演示VC的BLANK功能。

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. VC的滤波时钟源为PCLK
          5. PB09设置为PUSH-PULL输出
          6. BTIM1设置为定时功能，使用PCLK的8000分频计数，即1KHz时钟计数，100ms溢出产生TRGO信号

演示说明：
          VC输入P通道为PA02，N通道为AVCC通过电阻网络分压得到0.5AVCC，VC输出端口为PA00。
          PA02输入一个电压信号，PA02上的电压小于0.5AVCC时，PB09（LED）输出低电平，LED1灭。
          当PA02上的电压大于0.5AVCC时， PB09（LED）输出高电平，但由于BLANK的作用，当BTIM1产生TRGO信号时，
          VC将停止比较BLANKTIME设置的时间，此时VC的输出由BLANKLVL设置为低，因此会看到LED1将在程序的控制下
          熄灭50ms，使能LED1闪烁。
          
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