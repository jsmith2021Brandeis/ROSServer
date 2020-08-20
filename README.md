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

[ESP32 Multithreading](https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/)

Temp 

/home/robotics/Downloads/rosserial-noetic-devel/rosserial_arduino/src/rosserial_arduino

 rosrun rosserial_arduino make_libraries.py /home/robotics/Arduino/libraries/


/home/robotics/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries/WiFi

links 7/22/20
https://github.com/sachin0x18/rosserial_esp32/blob/master/src/examples/echo/main/esp_echo.cpp
https://github.com/sachin0x18/rosserial_esp32/tree/master/src/examples/echo
https://answers.ros.org/question/328467/sending-rostopic-form-esp32-through-husarnet/
https://www.hackster.io/khasreto/run-rosserial-over-the-internet-with-esp32-0615f5
http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
https://github.com/ros-drivers/rosserial

links for cmd vel to motor speeds
https://github.com/UbiquityRobotics/ubiquity_motor/tree/kinetic-devel/src
https://github.com/danielsnider/simple_drive/blob/master/drive_firmware/src/main.cpp
https://github.com/ros-controls/ros_controllers/tree/melodic-devel
https://github.com/merose/diff_drive
https://answers.ros.org/question/209963/cmd_veltwist-transform-twist-message-into-left-and-right-motor-commands/

links for website / running multiple robots on ROS
https://github.com/ramonus/flask-video-stream
https://answers.ros.org/question/41433/multiple-robots-simulation-and-navigation/
https://github.com/ManivannanMurugavel/opencv-flask-livecam/blob/master/app.py
https://answers.ros.org/question/160071/running-multiple-instances-of-same-node/
Video Streaming with Flask - miguelgrinberg.com





[Final Link for Website Streaming](https://github.com/log0/video_streaming_with_flask_example)

[Final Link For Website Uploading](https://github.com/thamizhchelvan/Python/blob/master/flask-file-upload/app.py)

https://www.quora.com/How-do-I-write-a-Python-script-to-execute-bash-commands-Python-3-6

[Final Link for screensharing](https://github.com/nikhil-web/screenshare) works on phone version: https://github.com/gvsurenderreddy/screenshare-2



link for passwords https://realpython.com/introduction-to-flask-part-2-creating-a-login-page/

https://pythonspot.com/login-authentication-with-flask/



launch terminal from python https://unix.stackexchange.com/questions/373186/open-gnome-terminal-window-and-execute-2-commands

change camera orientation gazebo https://answers.gazebosim.org//question/408/is-there-a-way-to-modify-the-default-camera-position-in-the-world-file/

run program in background https://askubuntu.com/questions/106351/running-programs-in-the-background-from-terminal



connext to wifi network using bssid https://forum.arduino.cc/index.php?topic=642938.0

require username and password to access website https://flask-basicauth.readthedocs.io/en/latest/

ip adress tunneling https://www.sitepoint.com/accessing-localhost-from-anywhere/

get unique id of ESP 32 https://www.reddit.com/r/esp32/comments/fan3rj/unique_id/