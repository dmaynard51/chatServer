/*
Name: Daniel Maynard
Program: Chat Client
Description: Most of my program initial connection setup came from my CS344 project
which was encoding and decoding a cyphertext. This program will connect
to a server once its compiled by ./chatclient localhost PORT
From there you must enter a user handle, and then will allow the user
to send messages to the server, it will then wait for a receive message
from the server. If the user types \quit it will end the connection with
the server.
Course: CS372
Last Modified: 7/25/19
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define BUFFSIZE 512






//holds our user name
char username[20];
//holds the message character
char message[502];
//combines user with message
char finalMessage[520];
//check if user entered quit
int quitConnection = 0;


void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

void getMessage()
{
    //loop until we have < 500 characters
    while (1){
    memset(finalMessage, '\0', sizeof(finalMessage)); // Clear out the buffer array    
    printf("%s: ", username);
    memset(message, '\0', sizeof(message)); // Clear out the buffer array
    fflush(stdin);
    fgets(message, 502, stdin);
    
    //make sure we have 500 characters
        if (strlen(message) < 503)
        {
            
            break;
        }
        printf("Too many characters, you must enter 500 and under.\n");
    }
    
    int j;
    
    

    //replace new line with enter key
    for (j = 0; j < 502; j++)
    {
        if (message[j] == '\n')
        {
            message[j] = '\0';
        }
    } 
    
    char* p; 
    p = strstr(message, "\\quit");
    
    if (p) {
        quitConnection = 1;
        
    }
    
    
    
    strcat(finalMessage, username);
    
    strcat(finalMessage, ": ");
    strcat(finalMessage, message);
    
    

    //make sure no \n end lines are in final message
    //python has an end line every print
    for (j = 0; j < 520; j++)
    {
        if (finalMessage[j] == '\n')
        {
            finalMessage[j] = '\0';
        }
    } 
    

    
}




int main(int argc, char *argv[])
{
if (argc < 3){
    printf("Must be a hostname and port\n ");
    exit(1);
}    
    
int socketFD, portNumber, charsWritten, charsRead;
struct sockaddr_in serverAddress;
struct hostent* serverHostInfo;
//holds the received text
char buffer[BUFFSIZE];



// Set up the server address struct
memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
serverAddress.sin_family = AF_INET; // Create a network-capable socket
serverAddress.sin_port = htons(portNumber); // Store the port number


serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

// Set up the socket
socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
if (socketFD < 0) error("CLIENT: ERROR opening socket");
// Connect to server
if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to addy
error("CLIENT: ERROR connecting");


//get username with 10 characters
fflush(stdin); 

//loop until we get a valid username
while (1) {
printf("What is your handle? ");    
memset(username, '\0', sizeof(username)); // Clear out the buffer array      
fgets(username, 13, stdin);
    if (strlen(username) < 12)
    {
        break;
    }
    
    printf("Username is too long\n");

}

int j;

//replace new line with enter nothing
//reference to remove \n:
//https://stackoverflow.com/questions/9628637/how-can-i-get-rid-of-n-from-string-in-c

for (j = 0; j < 20; j++)
{
    if (username[j] == '\n')
    {
        username[j] = '\0';
    }
}
fflush(stdin); 



//when connection made, loop send/receive until \quit entered
while (1){
    //get user input to send message
    getMessage();
    

    //send message
    charsWritten = send(socketFD, finalMessage, strlen(finalMessage), 0); // Write to the server
    if (quitConnection == 1){
        break;
    }
    
    
    
    int endLine = 0;
    int i;
    
    //clear read buffer
    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse    
    charsRead = recv(socketFD, buffer, sizeof(buffer)-1, 0); // Read data from the socket, leaving \0 at end
    
    printf("%s", buffer);
    printf("\n");
    
    char* p; 
    p = strstr(buffer, "\\quit");
        
    if (p) {
        quitConnection = 1;
        break;
    }

}

close(socketFD); // Close the socket
return 0;
}