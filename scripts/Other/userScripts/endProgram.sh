#!/bin/bash
#Jacob smith Brandeis Auomtation Lab 8/5/2020
#Closes gazebo and all running Terminals
cd /home/robotics/catkin_ws/src/ROSServer/scripts/Other/userFiles
#close gazebo
pkill gzserver 
pkill gzclient
#close terminals
pkill gnome-terminal