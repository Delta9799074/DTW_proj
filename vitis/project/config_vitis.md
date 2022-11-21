# 配置Vitis环境
1. 创建工程，选择vivado导出的xsa文件作为硬件平台
    - 注意：源代码是c++，但是需要创建一个hello world(c)工程，再把vitis自动创建的初始化硬件平台的代码(platform.c, platform.h, platform_config.h)拷贝到另外的文件夹内，关闭这个工程。
    - 再创建另一个empty(C++)工程，把上面的三个代码拷贝到src文件夹内。
    ![](readme_img\vitis\1_2.png)
    - **不能直接使用上面的c工程，会make失败**。
2. 配置SD卡(FAFTS文件系统)需要的头文件。
    - 修改BSP设置：        
    ![](readme_img\vitis\2_1.png)
    - 选择xilffs(包括了需要用到的头文件)
    ![](readme_img\vitis\2_2.png)
    - main.cpp 内include "ff.h"
    ![](readme_img\vitis\2_3.png)
    ff.h内的函数可以google fatfs查看用法。
    http://elm-chan.org/fsw/ff/00index_e.html
    ![](readme_img\vitis\2_4.png)
3. 配置DDR内堆栈空间
    - 右击cortex处理器，Generate Link Script
    ![](readme_img\vitis\3_1.png)
    - 分配Heap size和stack size
    ![](readme_img\vitis\3_2.png)
    **总共不能超过DDR大小的512MB**
4. 添加C++内需要的头文件
    properties -> c++ General -> Paths and Symbols -> add ->选择Strawberry的include文件夹。
    ![](readme_img\vitis\4_1.png)
    ![](readme_img\vitis\4_2.png)
5. 添加main.cpp文件，Ctrl + B编译main.cpp
6. 打开串口并烧录程序，从串口查看执行结果