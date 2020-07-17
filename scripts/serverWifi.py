# From https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/
#https://stackoverflow.com/questions/14765351/python-socket-error-address-in-use run as sudo
import socket
import time
import select
import sys
#repeat whole program if client disconnects
lastHeard=time.time()
elapsedTime=0
 

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
        print (content)
        client.sendall("Hello from Computer!\n")
        
    #print "Mark B"
    #elapsedTime= time.time()-lastHeard
    client.close()
    #if(elapsedTime > 5):
    #   print "Error, haven't heard from robot in a while, reconnecting"
    #else:
     #   print "elapsed time" + str(elapsedTime)
  