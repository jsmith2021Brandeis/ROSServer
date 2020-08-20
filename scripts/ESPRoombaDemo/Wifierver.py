#https://www.kite.com/python/examples/1795/select-construct-a-server-that-uses-%2560select%2560-to-handle-i%252Fos-for-multiple-connections
import socket
import select
import time
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(0)
server_address = ('192.168.43.231', 8090)
server.bind(server_address)
server.listen(5)

inputs = [server]
outputs = list()

processedMessage="NULL"
live=True
endIndex=0
while live:
    read_ready, write_ready, exceptions = select.select(inputs, outputs, [])

    for s in read_ready:
        # accept connection if server is read-ready
        if s is server:
            connection, client_address = s.accept()
            print "server received connection: socket " + str(connection.fileno())
            inputs.append(connection)
        # receive message if a connection is read-ready
        else:
            #note, a true buffering strateg yshould be used here
            message=s.recv(1024)
            #clear string if starts with start character
            if message.find('/*'):
                processedMessage=""
            #find first instance of end character
            endIndex=message.find("*\\")
            if endIndex != -1:
                processedMessage+=message[0:endIndex+2]
                #extra cehck because of cut off lines, should not be neccessary
                if processedMessage.find('/*') != -1:
                    #print processedMessage
                    print processedMessage[2:len(processedMessage)-2].split()
                processedMessage=""