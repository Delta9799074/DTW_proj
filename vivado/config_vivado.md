# 配置 Zynq PS IP
1. 选择zedboard
![](readme_img\vivado\1.png)
2. 创建Block Design，添加zynq7-processor IP核
![](readme_img\vivado\2.png)
3. 配置IP
    - MIO Configuration
    使用了SD卡和UART
    ![](readme_img\vivado\3_1.png)
    - Clock Configuration
    没有用到PL，取消PL时钟
    ![](readme_img\vivado\3_2.png)
    - PS-PL Configuration
    没有用到PL，全部取消
    - DDR Configuration
    Zedboard DDR3型号为MT41J128M16 HA-15E.
    后续还需要在Vitis中为Zedboard分配堆栈（在DDR内，DDR作为ps的内存，SD卡作为ps的存储器，程序将数据库从SD卡内读入到内存的堆（heap）内进行下一步的匹配）。
    ![](readme_img\vivado\3_4.png)
4. 导出xsa
    - 右键Block design， Generate Output Products -> 选择Global
    - Create HDL Wrapper
    - File -> export -> export Hardware