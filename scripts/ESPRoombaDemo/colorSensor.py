#!/usr/bin/env python
# import String and Twist
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist

COLORFACTOR=1.5

#print a message explaining what this program is 
print "Processes Data Coming From the Color Sensor"

# Callback whenever a `keys` message is seen
def color_parse(msg,color_pub):
    colorValues=msg.data.split(":")
    if len(colorValues) == 5 and colorValues[0]== "COLOR" and colorValues[4]:
        ambient_light=int(colorValues[1])
        red_light=int(colorValues[2])
        green_light=int(colorValues[3])
        blue_light=int(colorValues[4])

        color="WHITE"
        if (ambient_light < 15):
            color="BLACK "
        color_pub.publish(color)
        

# Main program when this .py file is called directly (this is a standard python idiom)
if __name__ == '__main__':

# Declare node
    rospy.init_node('color_sensor')

# let me publish `color sensor` topic    
    color_pub = rospy.Publisher('color_pub', String, queue_size=1)

# let me know when someone else publishes `keys` topic, then call keys_cb
    rospy.Subscriber('ESPComm', String, color_parse,color_pub)


# Wait until ^c
    rospy.spin()