#Jacob Smith Cosi 119A 9/26/2019
#Roomba Assignment, used wander.py as template
#Demonstrates wall avoiding capabaility neccesary for a ROOMBA robot
#Robot moves for 60 seconds in simulation and real life
#!/usr/bin/env python
#Sources
#https://stackoverflow.com/questions/34384523/how-can-loop-through-a-list-from-a-certain-index/34384791
#https://www.pythoncentral.io/pythons-time-sleep-pause-wait-sleep-stop-your-code/
#https://riptutorial.com/python/example/3973/infinity-and-nan---not-a-number--

#Shell command to run on robot
#export ROS_MASTER_URI=http://donatello.dyn.brandeis.edu:32476/
#export ROS_MASTER_URI=http://roba.dyn.brandeis.edu:11311
#export ROS_IP=172.20.77.41
#export TURTLEBOT3_MODEL="burger"


#Variables used in tuning robot
LINEAR_VEL=.5#.5
TURN_VEL=1#1
DISTANCE_THRESH=.4
TURN_TIME=-5

#import necesary libraries
import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan
from random import randint
import time
import math

# Function is called every time there's a scan
# Saves the smallest distance, which would be the closest obstacle
# This is without regard to the direction of the obstacle!
# global keyword is needed so g_range_ahead is available outside of the function
def scan_callback(msg):
    global g_range_ahead
    #print msg.ranges
    minRange=10
    #find the minimum range in the chunk in front of the robot
    for range in msg.ranges[346:360]:
        if (not math.isinf(range)) and range > .01:
            minRange=min(minRange,range)
    for range in msg.ranges[0:15]:
        if (not math.isinf(range)) and  range > .01:
            minRange=min(minRange,range)
    #update minimum range to global variable
    g_range_ahead=minRange

# Main program
g_range_ahead = 1 # anything to start

# Declare a subscriber to message 'scan' with message class LaserScan
scan_sub = rospy.Subscriber('scan', LaserScan, scan_callback)

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
    print (g_range_ahead)
    #if robot is too close to a wall, turn
    if g_range_ahead<DISTANCE_THRESH:
        twist.angular.z=TURN_VEL
        print ("Turning")
    # if robot is not too close to a wall, move forward
    #and reset timer
    else:
        twist.linear.x = LINEAR_VEL
        print ("Driving Forward")
    #publish robot motion
    cmd_vel_pub.publish(twist)
    # Sleep for 1/rate seconds
    rate.sleep()
  