示例目的：
          采用中断方式，通过I2C接口， 实现I2C主设备功能，和I2C从机进行数据通信。

硬件资源：
          1. CW32L012C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. PB09/PB08设置为PUSH-PULL输出，驱动LED

演示说明：
          1、采用默认时钟配置，初始化I2C为主机，400KHz。
          2、主从通信主设备I2C发送时序为多字节写：依次发送START信号、SLA+W字节、多个数据字节，最后发送STOP信号。
          3、主从通信主设备I2C接收时序为随机地址+顺序读。
          4、程序流程：程序完成I2C主设备配置后，先将发送数组中的内容发给从机，从机RAM目标地址:0x20000000 + 0x0800。
          然后延时，主机启动数据接收流程，并比较接收数据是否与发送数据一致，如果相同，LED1指示灯亮，否则LED2指示灯亮。
          最后重新初始化发送数组内容，不断重复数据发送、接收过程。
          5、注意主机、从机上电顺序，建议从机先工作，等待主机开始发送数据。

硬件连接：
          LPI2C_MASTER SCL  (PB06)-- LPI2C_SLAVE SCL  (PB06)
          LPI2C_MASTER SDA  (PB07)-- LPI2C_SLAVE SDA  (PB07)

使用说明：
+ EWARM
          1. 打开I2C_mcu_master_Int.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开I2C_mcu_master_Int.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
