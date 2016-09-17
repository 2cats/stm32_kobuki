# STM32F103 Client For [Kobuki](http://kobuki.yujinrobot.com/) / [Turtlebot 2](http://www.turtlebot.com/)

## Introduction
This the embedded side implement for [kobuki_driver]( https://github.com/yujinrobot/kobuki_core/tree/indigo/kobuki_driver). The Protocol Specification can be found [HERE](https://yujinrobot.github.io/kobuki/doxygen/enAppendixProtocolSpecification.html).
The porject now contains basic control logic as an example of usage.
The main principle of implementing this protocol library is to keep the API both **simple** and **safe**.

## Documentation
Currently,API-Usage now is written in the header file.

## Test
*Ubuntu14.04 indigo*
```bash
$ export TURTLEBOT_SERIAL_PORT=/dev/ttyUSB0
$ roslaunch turtlebot_bringup minimal.launch --screen
$ rqt -s kobuki_dashboard
```
