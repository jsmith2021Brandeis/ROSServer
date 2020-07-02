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