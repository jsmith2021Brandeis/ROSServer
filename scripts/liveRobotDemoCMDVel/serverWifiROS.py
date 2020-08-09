# From https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/
#https://stackoverflow.com/questions/14765351/python-socket-error-address-in-use run as sudo
import socket
import time
import select
import sys

import sys, select, tty, termios
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist

vel="CMDVEL 0 0"
velSeen=False
#repeat whole program if client disconnects
lastHeard=time.time()
elapsedTime=0

# Callback whenever a `keys` message is seen
def drive_Esp32(t):
    global vel
    global velSeen
    velSeen=False
    vel="CMDVEL "+str(t.angular.z) +" "+str(t.linear.x)
    print (vel)

def connect():
    s = socket.socket()  
    connected = False  
    while not(connected): 
        try:
            s.bind(('192.168.43.231', 8090)) #awas .231
            connected=True
        except socket.error as ex:
            print "Error, the socket could not be opened"
            print  "Make sure you care connected to the correct wifi network"
            time.sleep(1)
    return s

  
# Convention that means: "if this program was run directly (vs. imported)"
if __name__ == '__main__':
    ESPComm = rospy.Publisher('ESPComm', String, queue_size=1) 
    rospy.init_node("ESPListener")
    rate = rospy.Rate(100)

    # let me know when someone else publishes `keys` topic, then call keys_cb
    #rospy.Subscriber('keys', String, keys_cb)

     # let me know when someone else publishes `keys` topic, then call keys_cb
    rospy.Subscriber('cmd_vel', Twist, drive_Esp32)

    print "Publishing ESP to ROS Message" 
    
    while not rospy.is_shutdown():
        while True:
            s=connect()
            s.listen(0) 
            client, addr = s.accept()
            while True:
                try: 
                    content = client.recv(32)
                except socket.error:
                    print "Robot Reconnected!"
                    sys.exit(0) 
                if not velSeen:
                    client.sendall(str(vel)+"\n")
                    velSeen=True
                print(content)
                ESPComm.publish(content)

        client.close()
        rate.sleep()