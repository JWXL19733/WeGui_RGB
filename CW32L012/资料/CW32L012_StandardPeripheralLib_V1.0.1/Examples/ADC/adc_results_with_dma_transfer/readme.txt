示例目的：
          演示ADC1与ADC2同步启动的功能。ADC1做主，ADC2做从，通过ADC1的启动同步启动ADC2。

硬件资源：
          1. CW32L012CxTx StartKit  
          2. 系统时钟源为默认的HSIOSC ，分频到16MHz， PCLK、HCLK不分频，
          PCLK=HCLK=SysClk=16MHz
          3. ADC1的时钟源为PCLK，8分频， 2MHz
          4. PB05设置为ADC1_SAM,指示转换时间。
          5. uart3配置为1Mbps，作为调试打印
          6. DMA 通道1设置为ADC的EOS触发
          7. DMA 通道2设置为UART3的TXE触发

演示说明：
          ADC1使用通道为AIN0~7，序列采样结束后，触发DMA通道1，开启UART3的DMA功能，UART3开始将采样结果依次打印。
         

使用说明：
+ EWARM
          1. 打开adc_sgl_sw.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sgl_sw.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)