'''
Name: Daniel Maynard
Program: Chat Server
Description: This program will open a TCP connection, and wait for a client connection
To run the program you type "python chatserver.py PORT"
The server will wait on a specific port until a connection is made or until
it receives a SIGINT (CTRL C). 
Once a connection is made, it will wait for the client to send a message,
and once it receives a message it will allow the server to enter a message.
If the client enters \quit, it will end the current connection, and move back
to waiting to accept a connection.
Course: CS372
Last Modified: 7/25/19
'''

import socket
import sys




def receiveMessage(mess):
    return mess.recv(512)


def sendMessage(c, message):
    c.send(message)
    
 
 
# this will be updated with the user PORT argument 1
TCP_PORT = 0


if len(sys.argv) < 2:
    print "Need to enter port"
    sys.exit()

#add argument to port, must be integer
TCP_PORT = int(sys.argv[1])

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('localhost', TCP_PORT))
s.listen(1)



#to close connection please use SIGINT (CTRL C)
while (1):
    #accept connections
    print "The server is ready to receive"
    conn, addr = s.accept()

    print 'Connection address:', addr
    
    #for this connection receive and send
    while(1):
        #receive message
        data = receiveMessage(conn)
        #print to terminal
        print data
        #if \quit found in the receive, we end the loop/connection
        if "\quit" in data:
            break
        
        #get user input
        servermessage = raw_input("Daniel: ") 
        
    
        
        #concatenate username + message
        servermessage = "Daniel: " + servermessage
        
        #send message, if quit found then break from loop, and wait for
        #new connection
        sendMessage(conn, servermessage)
        if "\quit" in servermessage:
            break    

    #close this connection, and wait for new connections
    print "Closing connection"
    conn.close()