# From https://techtutorialsx.com/2018/05/17/esp32-arduino-sending-data-with-socket-client/
#https://stackoverflow.com/questions/14765351/python-socket-error-address-in-use run as sudo
import socket
import time
import select
#repeat whole program if client disconnects
lastHeard=time.time()
s = socket.socket()  
connected = False   

while not(connected): 
    try:
        s.bind(('192.168.43.199', 8090))
        connected=True
    except socket.error as ex:
        pass
        print "Error, the socket could not be opened"
        print  "Make sure you care connected to the correct wifi network"
        time.sleep(.5)


s.listen(0)       
 
while True:
    client, addr = s.accept()
    while True:
        content=""
        ready = select.select([s], [], [], 1)
        if ready[0]:
            content = client.recv(32)
        print (content)
        if (len (content) <=0):
           print "Mark A"
        elapsedTime= time.time()-lastHeard
        lastHeard=time.time()
        client.sendall("Hello from Computer!\n")
        
    print "Mark B"
    elapsedTime= time.time()-lastHeard
    client.close()
    if(elapsedTime > 5):
       print "Error, haven't heard from robot in a while, reconnecting"
    else:
        print "elapsed time" + str(elapsedTime)
  