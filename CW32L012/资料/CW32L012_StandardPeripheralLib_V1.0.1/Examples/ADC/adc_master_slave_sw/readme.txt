示例目的：
          演示ADC1与ADC2同步启动的功能。ADC1做主，ADC2做从，通过ADC1的启动同步启动ADC2。

硬件资源：
          1. CW32L012CxTx StartKit  
          2. 系统时钟源为默认的HSIOSC 24分频，4MHz， PCLK、HCLK不分频，
          PCLK=HCLK=SysClk=4MHz
          3. ADC1的时钟源为PCLK，1分频， 4MHz
          4. ADC2的时钟源为PCLK，1分频， 4MHz
          5. PB09(LED1)设置为PUSH-PULL输出
          6. PB05设置为ADC1_SAM,指示转换时间。
          7. PA04设置为ADC2_SAM,指示转换时间。
          7. uart3配置为115200bps，作为调试打印

演示说明：
          ADC1使用通道为AIN0:PA00。
          ADC2使用通道为AIN0：PA05
          ADC转换完成，LED1闪烁,并通过UART3输出转换结果
          支持查询模式和中断模式，获取采样结果
          支持单次转换和连续连续
         

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