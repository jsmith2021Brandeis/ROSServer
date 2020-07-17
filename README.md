#ROSServer Project Brandeis Automation Laboratory Summer 2020


Made Initially from Jacob Smith's Cosi 119 work, will be used to create ROS Server so Brandeis Community Members can program real robots from home



## ROS ROOMBA

#### **Overview**

###### runRoomba.sh: A bash file that automatically launches ROSCORE, gazebo, and the runRoomba node

runRoomba.py: A python ROS node (based on wander.py) which drives forward if it does not see a ditacen less than the threshold, and turns if it does. This behavior results in a wall avoiding pattern, much like a roomba.

Using [Cheese](https://help.ubuntu.com/community/Webcam) for Webcam Feedback

​	follow link to install software

​	run cheese in command terminal to launch

![View of Camera Software in Cheese](Camera.png)

​	Demonstration: This is my roomba program running on the gazebo simulator, it gets stuck after the video 	ends.

![Demonstration of my Roomba example](roombaDemo.gif)



**-Jacob Smith** jsmith2021@brandeis.edu



Sources

[Launch tab in gnome](https://unix.stackexchange.com/questions/158434/open-gnome-terminal-with-several-tabs-and-execute-a-few-commands-in-every-tab) 

[Rename tab in gnome](https://unix.stackexchange.com/questions/177572/how-to-rename-terminal-tab-title-in-gnome-terminal)

[Hide Error Message in Gnome](https://stackoverflow.com/questions/15678796/suppress-shell-script-error-messages)

[Name tab](https://unix.stackexchange.com/questions/24734/how-can-i-launch-gnome-terminal-with-unique-titles-for-multiple-tabs)

[Arduino Linux Tutorial](https://www.arduino.cc/en/guide/linux) [2](https://websiteforstudents.com/how-to-install-arduino-ide-on-ubuntu-18-04-16-04/)

[ESp32 Download error](https://community.platformio.org/session/sso?return_path=%2Ft%2Fa-fatal-error-occurred-invalid-head-of-packet%2F10233%2F2)

ROS on ESP32 Links [1](https://blog.hadabot.com/ros2-esp32-to-control-motor-driver-using-web-browser.html) [2](https://www.hackster.io/khasreto/run-rosserial-over-the-internet-with-esp32-0615f5) [3](https://github.com/ros-drivers/rosserial/tree/noetic-devel/rosserial_arduino/src/ros_lib) [4](https://github.com/ros-drivers/rosserial/pull/345) [5](http://wiki.ros.org/action/fullsearch/rosserial_arduino?action=fullsearch&context=180&value=linkto%3A%22rosserial_arduino%22%27) [6](https://medium.com/@e850506/ros-serial-with-esp-32-246248cb6bac) [7](https://www.birdbraintechnologies.com/roboticsathome/)

[Inlcuding Arduino Libraries in Linux](https://forum.arduino.cc/index.php?topic=141190.0)

[Using TImer Library](https://github.com/BrandeisMakerLab/Robotics_ZumoAutomation)