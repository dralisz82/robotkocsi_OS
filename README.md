# robotkocsi_OS
Controller firmware for a mobile robot, having a K64F MCU on board.

Supported robot: https://robotkocsi.blog.hu

This firmware is built on the Mbed OS framework (https://os.mbed.com)

Initial version published on: https://os.mbed.com/users/dralisz82/code/robotkocsi_OS/

Classes:
Lights: controlling lamps and lights of the robot
Drive: controlling main motor and steering of the robot
Sensors: factory class, maintaining the on-board sensors
Sensor: represents a sensor, can be obtained from Sensors
Demo: wrapper class for a thread, running a simple demo by driving the robot and using the lights
CommandHandler: wrapper class for command handling routines and necessary variables - doesn't exist, yet, coming soon
