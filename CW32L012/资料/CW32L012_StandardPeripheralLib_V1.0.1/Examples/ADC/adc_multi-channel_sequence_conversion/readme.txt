示例目的：
          演示软件触发ADC模块进行序列转换模式。

硬件资源：
          1. CW32L012 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟6分频，16MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=16MHz
          4. ADC1的时钟源为PCLK，1分频


演示说明：
          ADC SQR通道分别为：AIN00:PA00, AIN01:PA01, AIN02:PA02, AIN03:PA03, AIN04:PA04, AIN05:PA05,
          AIN06:PA06, AIN07:PB00。
          转换完成后，转换结果通过板载串口送出      

使用说明：
+ EWARM
          1. 打开adc_sqr_irq_sw.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sqr_irq_sw.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)