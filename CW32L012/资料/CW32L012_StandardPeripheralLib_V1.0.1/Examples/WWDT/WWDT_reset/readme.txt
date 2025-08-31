示例目的：
          本样例演示 WWDT 的启动及重载及复位

硬件资源：
          1. CW32L012F8P6 StartKit
          2. 时钟HSI
          3. 系统时钟由HSI提供，分频为8MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=8MHz
          4. WWDT的时钟为PCLK/131072
          5. PB09设置为PUSH-PULL输出，驱动LED1          

演示说明：
          程序上电后，LED1快速闪烁4次，然后慢速闪6次，约1秒后，程序复位。
          
使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
