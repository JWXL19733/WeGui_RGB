示例目的：
          采用中断方式，通过I2C接口， 实现I2C从设备功能，和I2C主机进行数据通信。

硬件资源：
          1. CW32L012C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. PB09/PB08设置为PUSH-PULL输出，驱动LED

演示说明：
          1、采用默认时钟配置，初始化I2C为从机，400KHz。
          2、程序流程：从机完成I2C从设备配置后，一直等待主机发送数据。
          3、从机接收到数据后，自动保存到RAM目标地址:0x20000000 + 0x0800开始的数据区，LED1闪亮。
          4、从机接收到主机读数据指令后，自动将RAM目标地址中的数据回传给主机。

使用说明：
+ EWARM
          1. 打开I2C_mcu_slave_Int.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开I2C_mcu_slave_Int.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
