示例目的：
          演示定时器外部触发启动ADC转换，定时1s采样PA00(AIN0)、PA01(AIN1)。

硬件资源：
          1. CW32L012CxTx StartKit  
          2. 时钟HSI
          3. 系统时钟源为HSI时钟1分频，96MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=96MHz
          4. ADC1的时钟源为PCLK，2分频， 48MHz
          5. PB09(LED1)设置为PUSH-PULL输出
          6. PB05设置为ADC1_SAM,指示转换时间，PA00、PA01模拟输入，作为AIN0、AIN1。
          7. uart3配置为115200bps，作为调试打印
          8.GTIM1的时钟源为PCLK，9600分频，溢出时间1s，CH1的占空比50%，PWM2模式，比较匹配时刻触发ADC序列采样
          9. PB04设置为推挽输出，复用做GTIM1的CH1，输出PWM

演示说明：
         通用定时器GTIM1定时1s自动触发ADC模块进行转换，ADC采样通道为PA00、PA01。
          ADC转换完成后，LED1的电平翻转，以0.5Hz的频率闪烁。可以通过PB04观察GTIM1_CH1的输出情况，PB05观察ADC的采样时刻
         

使用说明：
+ EWARM
          1. 打开adc_sqr_ext_trig.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sqr_ext_trig.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)