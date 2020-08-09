#Jacob Smith Cosi 119A 9/26/2019
#Roomba Assignment, used wander.py as template
#Moves robot backward to demonstrate website ability to add user files
#Demonstrates wall avoiding capabaility neccesary for a ROOMBA robot
#Robot moves for 60 seconds in simulation and real life
#!/usr/bin/env python

#import necesary libraries
import rospy
from geometry_msgs.msg import Twist


# Same code can be a publisher and a subscriber, this is no problem
# be ready to publish the cmd_vel topic of class Twist
cmd_vel_pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)

# Declare us to be a node
rospy.init_node('roomba')
state_change_time = rospy.Time.now()

#Set up loop to decide if robot is driving forward or turning
rate = rospy.Rate(10)
while not rospy.is_shutdown():
    # Create an all zero Twist() message. Note a new one is created each loop
    twist = Twist()
    twist.linear.x=.5
    #publish robot motion
    cmd_vel_pub.publish(twist)
    # Sleep for 1/rate seconds
    rate.sleep()
  