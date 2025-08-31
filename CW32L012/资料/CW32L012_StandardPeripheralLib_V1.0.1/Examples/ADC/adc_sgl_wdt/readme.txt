示例目的：
          演示ADC模块单通道模拟看门狗功能。

硬件资源：
          1. CW32L012CxTx StartKit  
          2. 系统时钟源为默认HSIOSC的24分配，4MHz， PCLK、HCLK不分频，
          PCLK=HCLK=SysClk=4MHz
          3. ADC的时钟源为PCLK，1分频
          4. PB09设置为PUSH-PULL输出，驱动LED1
          5. UART3作为调试串口

演示说明：
          软件触发ADC模块进行单通道单次转换。LED1以0.5Hz的频率闪速
          ADC模拟看门狗通道为AIN0:PA00，下阈值为0x600, 上门限为0x700；
          外接可调直流电源到PA00，ADC转换完成，如果采样值小于下阈值，则LED1灭；
          如果采样值大于上阈值，LED1亮。

使用说明：
+ EWARM
          1. 打开adc_sgl_wdt.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sgl_wdt.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)