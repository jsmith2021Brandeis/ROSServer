#!/usr/bin/env python
# import String and Twist
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist
import time

#print a message explaining what this program is 
print "Pilots the robot based off its color sensor" 

backingUp=False

millis = lambda: int(round(time.time() * 1000))
initTime=millis()

# Callback whenever a `keys` message is seen
def roomba(msg, twist_pub):
    color=msg.data.strip()
    t = Twist()
    global backingUp 
    global initTime

    if backingUp and millis()-initTime < 4000:
        print ("\tContinue Turn")
        return
    backingUp=False
    if color == "BLACK":
        print ("Forward")
        t.linear.x = .5
    elif color == "WHITE":
        print ("Start Turn")
        initTime=millis()
        t.linear.x = -.5
        #t.angular.z = .5 #stop robot when sees white
        backingUp=True     
    twist_pub.publish(t)

# Main program when this .py file is called directly (this is a standard python idiom)
if __name__ == '__main__':

# Declare node
    rospy.init_node('pilot')

# let me publish `cmd_vel` topic    
    twist_pub = rospy.Publisher('cmd_vel', Twist, queue_size=1)

# let me know when someone else publishes `keys` topic, then call keys_cb
    rospy.Subscriber('color_pub', String, roomba, twist_pub)


# Wait until ^c
    rospy.spin()