#!/usr/bin/env python

# termios library allows access to terminal (shell) input 

import sys, select, tty, termios
import rospy
from std_msgs.msg import String

#store the current and most recent key press
key="NULL"
oldKey="NULL"

# Convention that means: "if this program was run directly (vs. imported)"
if __name__ == '__main__':
    key_pub = rospy.Publisher('keys', String, queue_size=1) 
    rospy.init_node("keyboard_driver")
    rate = rospy.Rate(100)

    # Save "keyboard attributes" so you can restore them at the end.
    old_attr = termios.tcgetattr(sys.stdin) 
    tty.setcbreak(sys.stdin.fileno())
    print "Publishing keystrokes. Press Ctrl-C to exit..." 
    
    while not rospy.is_shutdown():
        if select.select([sys.stdin], [], [], 0)[0] == [sys.stdin]: 
            key=sys.stdin.read(1)
            #print the key if it is a new direction
            if (key != oldKey):
                print key +" Pressed"
                key_pub.publish(key)
            oldKey=key
        rate.sleep()
    
    # restore keyboard attributes
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_attr)