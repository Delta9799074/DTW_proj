# Dynamic Time Warping Project

## Prepare
### C++
Install Straberry g++ Compiler to get the external libraries.
https://strawberryperl.com/
### python
python 3.10.4
### Database
Kaggle Audio Mnist
https://www.kaggle.com/datasets/sripaadsrinivasan/audio-mnist
## Hierarchy
```
python
|--dtw.py(Fastdtw library)
|--mfcc.py(using librosa to extract mfcc)
cpp
|--DTW_Function.cpp
|--DTW_Function.h
|--mian.cpp
vivado
|--config_vivado.md
|--design_1_wrapper.xsa
|--ddr_auto.xpr
vitis
|-platform_header(auto-generate by vitis)
  |--platform.c
  |--platform.h
  |--platform_config.h
|-project
  |--config_vitis.md
  |--test_myfunc.prj
|--main.cpp
```
## Database(SD)
x.csv: Number x match library
qx.csv: Number x query library
https://sunyatsen-my.sharepoint.cn/:f:/g/personal/zhouqm8_ms_sysu_edu_cn/Ek33urylr8hOjn4nlt6clvwBhOSfygY6D34d8LOjwVV4Zg?e=ZN7o7O