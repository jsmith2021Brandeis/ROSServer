# From https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/
#https://stackoverflow.com/questions/14765351/python-socket-error-address-in-use run as sudo
import socket
import time
import select
import sys

import sys, select, tty, termios
import rospy
from std_msgs.msg import String

vel=0
#global client
command ="VEL{-1}\n"
#repeat whole program if client disconnects
lastHeard=time.time()
elapsedTime=0
 
# Callback whenever a `keys` message is seen
def keys_cb(msg):
    global vel
    key=msg.data
    if key=='w':
       vel=1
    elif key=='s':
       vel=0
    elif key=='x':
       vel=-1
    print (vel)


def connect():
    s = socket.socket()  
    connected = False  
    while not(connected): 
        try:
            s.bind(('192.168.43.199', 8090))
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
    rospy.Subscriber('keys', String, keys_cb)

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
                client.sendall("VEL{"+str(vel)+"}\n")
                ESPComm.publish(content)

        client.close()
        rate.sleep()