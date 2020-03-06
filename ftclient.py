'''
Name: Daniel Maynard
Program: FTP Client
Description: 
Start client by typing python ftclient.py <host> <port> <command> 
    Example: for listing files - python ftclient.py flip1 8090 -l
    Example: for downloading a file - python ftclient.py flip1 8090 -g test.txt 8091
This is the client program which will allow the user to list out server
directory data with a -l command. It will also be able to download a text file
with -g command.
Course: CS372
Last Modified: 8/12/19
'''

import socket
import sys
from time import sleep




def receiveMessage(mess):
    return mess.recv(50000)



def sendMessage(c, message):
    c.send(message)

#reference: https://www.pythonforbeginners.com/files/reading-and-writing-files-in-python
def createTextFile(message, fname):
    #open file
    file = open(fname,"w") 
    #write data onto file
    file.write(message) 
    #close file
    file.close() 
 
# this will be updated with the user PORT argument 1
TCP_PORT = 0



#host, port, command
if len(sys.argv) < 3:
    print ("Need to enter port")
    sys.exit()

hostChecker = 0
#check if host is correct
if sys.argv[1] == "flip1" or sys.argv[1] == "flip" or sys.argv[1] == "flip2" or sys.argv[1] == "flip3":
    hostChecker = 1


if hostChecker != 1:
    print ("Wrong host")
    sys.exit()
    


#add argument to port, must be integer
TCP_PORT = int(sys.argv[2])


#reference https://www.geeksforgeeks.org/socket-programming-python/
#connect to the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost', TCP_PORT))

#first add the command (-g, or -l)
servermessage = sys.argv[3]
#add on the port     
if sys.argv[3] == "-g":
    servermessage = sys.argv[3] 
    #data port will be argument 5
    TCP_PORT2 = int(sys.argv[5])
    #concatenate file name and data port to server message
    servermessage = servermessage + '\n' +sys.argv[4] + '\n' + sys.argv[5]

#send message to server
sendMessage(s, servermessage)


#create data connection
if sys.argv[3] == "-g":
    #create new socket
    #give time to allow data port to come up
    sleep(1)
    #connect to data port
    s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    #connect to localhost on the data port (argument 5)
    s2.connect(('localhost', TCP_PORT2))
    #receive data file name
    fileName = receiveMessage(s2)

    #send success back
    response = "success\n"
    sendMessage(s2, response)
    #receive message data
    data = receiveMessage(s2)    
    #create text file with filename and data
    createTextFile(data, fileName)
    #close data port connection
    s2.close()


#receive -l data
data = receiveMessage(s)
#print out directory data
print (data)

#close server connection
s.close()