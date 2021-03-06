#!/bin/bash
#Jacob smith Brandeis Auomtation Lab 7/2/2020
#Sets up ROS and runs ROS programs for ROSSERVER Project
cd /home/robotics/catkin_ws/src/ROSServer/scripts/Other/userFiles
#print helper message
echo "Main Command Terminal, will execute the other commands" 
#execute necessary commands for ROSSERVER setup
#in order: launch camera, roslaunch, ros subscriber (reads keys) , ros publisher (publishes key press)
gnome-terminal \
    --tab -e "bash -c   'sleep 2; export TURTLEBOT3_MODEL=burger; roslaunch turtlebot3_gazebo turtlebot3_stage_1.launch;  read -n1'" \
    --tab -e "bash -c 'echo -ne \"\033]0;Robot Commander\007\" ; sleep 1 ; python userScript.py ; read -n1'" \

#close terminal windows after X  seconds
sleep 300
#close gazebo
pkill gzserver 
pkill gzclient
#close terminals
pkill gnome-terminal