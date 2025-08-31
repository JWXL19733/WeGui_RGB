示例目的：
          演示软件启动ADC单通道转换模式。

硬件资源：
          1. CW32L012CxTx StartKit  
          2. 时钟HSI
          3. 系统时钟源为HSI时钟1分频，96MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=96MHz
          4. ADC1的时钟源为PCLK，2分频， 48MHz，ADC1的采样率最高达2M/s
          5. PB09(LED1)设置为PUSH-PULL输出
          6. PB05设置为ADC1_SAM,指示转换时间。
          7. uart3配置为115200bps，作为调试打印

演示说明：
          ADC通道为AIN0:PA00。
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