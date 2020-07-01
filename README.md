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
