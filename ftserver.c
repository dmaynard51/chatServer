/*
Name: Daniel Maynard
Program: ftserver
Description:
1. Run the Makefile by typing ./MakeFile
2. Start server by typing ./ftserver <port> (example: ./ftserver 8090)
This program will be the server which will create a new process child for
each connection and then close the child. It will listen for connections
on the same port, and depending on the command. It will be able to list
server directory files, or send a text document to the client. When sending
a text document it will open a data connection using the port designated by
the user, and if the file name is correct will send the file and close
the connection with the client.
Course: CS372
Last Modified: 8/12/19
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <pthread.h>



#define BUFFSIZE 200000
char buffer[BUFFSIZE];

char fileContent[BUFFSIZE];
char keyContent[BUFFSIZE];
char plaintext[BUFFSIZE];
int childExitMethod = -5;
pid_t unfinishedProcesses[512];
int pidIncrementor = 0;

char * keyFile[3];
char encryptedFile[BUFFSIZE];
int encryptedNumber[BUFFSIZE];
char cypherText[BUFFSIZE];


//used to tokenize string

char *args[512];

char fileName[BUFFSIZE];
char command[BUFFSIZE];

//numerical value for file and key
int fileInts[BUFFSIZE];
int keyInts[BUFFSIZE];

int exitValidation = 0;

int noFile = 0;



//holds each file name
char* files[10];
//holds content of file
char fileContent[BUFFSIZE];







void error(const char *msg) 
{ 
    perror(msg); exit(1); 
} // Error function used for reporting issues





//read the directory files
//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
//Resouce: https://stackoverflow.com/questions/3554120/open-directory-using-c
//http://man7.org/linux/man-pages/man3/readdir.3.html
void getFileNames() 
{
    
    struct dirent *pDirent;

    DIR* dirCheck;
    //DIR* fp;
    char *ch;
    int file_descriptor;   
    
    ssize_t nread, nwritten;
    char readBuffer[1000];
    
    int i=0;
    
    //open current directory
    dirCheck = opendir (".");
    
    
    //open the latest folder we just found

    
    
    memset(buffer, '\0', strlen(buffer));    
    struct dirent *sDirent;
    //read files in directory
    while ((sDirent = readdir(dirCheck)) != NULL)
    {
        //loop through each file in the folder
        if(sDirent-> d_type == DT_REG)
        {
            //add name of the document to files[i]
            files[i] = sDirent->d_name;
            //copy file into buffer
            strcat(buffer, files[i]);
            //add on a end line
            strcat(buffer, "\n");
            i++;
        }
                    
    }
    
    
}


//reference: read the file data https://www.tutorialspoint.com/cprogramming/c_file_io.htm
void readFile(char* fn, char* fdesc)
{
    FILE *fp;
    char buff[BUFFSIZE-1];
    //clear 
    memset(buff, '\0', sizeof(buff));

//if we can access the file
//reference: https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
    if( access( fn, F_OK ) != -1 ) {

        //reference: https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
        FILE *f = fopen(fn, "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
        
        char *string = malloc(fsize + 1);
        fread(string, 1, fsize, f);
        fclose(f);
        //printf("%s", string);
        
        //copy what we have read and return it to fileData (fdesc)
        strcpy(fdesc, string);
        
        string[fsize] = 0;        

        
                        
    } 
    //if file doesnt exist
    else {
        noFile = 1;
        printf("%i", noFile);
        printf("file doesnt exist\n");


    }


}


int main(int argc, char *argv[])
{

int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
if (argc < 2) 
{ 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); 
} // Check usage & args

portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string


///////

socklen_t sizeOfClientInfo;

char keybuffer[BUFFSIZE];//delete this
struct sockaddr_in serverAddress, clientAddress;



// Set up the address struct for this process (the server)
memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct


serverAddress.sin_family = AF_INET; // Create a network-capable socket
serverAddress.sin_port = htons(portNumber); // Store the port number
serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

// Set up the socket
listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
if (listenSocketFD < 0) error("ERROR opening socket");

// Enable the socket to begin listening
if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
error("ERROR on binding");
listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections
printf("listening\n");

pid_t childpid = 0;
int i = 0;
while(1)
{
//return to parent


    



// Accept a connection, blocking if one is not available until one connects
sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
if (establishedConnectionFD < 0) error("ERROR on accept");
    
    

//reference: for my parent/child processes I took my program from CS344's encryption
// project. 
pid_t spawnPid = fork();

//create a child process
switch(spawnPid)
{
case -1:
        perror("Hull Breach!");
        exit(1);
        break;     

case 0: // Terminate the child process immediately
    
            printf("connection made!\n");
            
            

            printf("listening\n");
            
            pid_t childpid = 0;            
            
            
            // Get the message from the client and display it
            memset(buffer, '\0', strlen(buffer));
            memset(keybuffer, '\0', BUFFSIZE);

            //recv from client
            charsRead = recv(establishedConnectionFD, buffer, BUFFSIZE-1, 0); // Read the client's message from the socket

            int readFiles = 0;
            int transfer = 0;            
            char str[] ="";
            strcpy(str, buffer);
            //clear buffer
            memset(buffer, '\0', strlen(buffer));

            //printf("%s\n", str);
            int p = 0;
            //http://www.cplusplus.com/reference/cstring/strtok/
            //separate arguments by token
              char * pch;
              pch = strtok (str," \n");
              while (pch != NULL)
              {
                args[p] = pch;
                pch = strtok (NULL, " \n");
                p++;
              }           
            
            
            strcpy(command, args[0]);
            //if client wants to read user contents
            if (strcmp(args[0], "-l") == 0)
            {
                readFiles = 1;
                
                
            }
            
            if (readFiles == 1)
            {
                printf("Reading files\n");
                getFileNames();
                charsRead = send(establishedConnectionFD, buffer, strlen(buffer), 0); // Send success back
                
            }
            
            //create a data connection
            
            if (strcmp(args[0], "-g") == 0)
            {
                transfer = 1;
                printf("connecting to data port\n");

                
                
                
                int listenSocketFD2, establishedConnectionFD2, portNumber2, charsRead2;
                if (argc < 2) 
                { 
                    fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); 
                } // Check usage & args
                
 
                portNumber2 = atoi(args[2]); // Get the port number, convert to an integer from a string

                
                socklen_t sizeOfClientInfo2;
                

                struct sockaddr_in serverAddress2, clientAddress2;
                
                
                
                // Set up the address struct for this process (the server)
                memset((char *)&serverAddress2, '\0', sizeof(serverAddress2)); // Clear out the address struct
                
                
                serverAddress2.sin_family = AF_INET; // Create a network-capable socket
                serverAddress2.sin_port = htons(portNumber2); // Store the port number
                serverAddress2.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process
                
                // Set up the socket
                listenSocketFD2 = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
                int optval = 1;
                //create add new socket to current connection
                //reference: https://beej.us/guide/bgnet/html/multi/setsockoptman.html
                setsockopt(listenSocketFD2, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
                
                if (listenSocketFD2 < 0) error("ERROR opening socket");
                
                // Enable the socket to begin listening
                if (bind(listenSocketFD2, (struct sockaddr *)&serverAddress2, sizeof(serverAddress2)) < 0) // Connect socket to port
                error("ERROR on binding");
                listen(listenSocketFD2, 5); // Flip the socket on - it can now receive up to 5 connections
                printf("listening to data port\n");

                

  
                //get file name
                strcpy(fileName, args[1]);
                
                    
                
                
                
                // Accept a connection, blocking if one is not available until one connects
                sizeOfClientInfo2 = sizeof(clientAddress2); // Get the size of the address for the client that will connect
                establishedConnectionFD2 = accept(listenSocketFD2, (struct sockaddr *)&clientAddress, &sizeOfClientInfo2); // Accept
                if (establishedConnectionFD2 < 0) error("ERROR on accept"); 
                
                //read the file and put data content into fileContent variable
                readFile(fileName, fileContent);

                
                //if file not found replace buffer string
                if (noFile == 1)
                {
                    memset(buffer, '\0', strlen(buffer));
                    strcpy(buffer, "File does not exist\n");
                    printf("File does not exist\n");
                    charsRead = send(establishedConnectionFD2, buffer, strlen(buffer), 0); // Send success back                    
                }
                
                else
                {
                    //first send send filename
                    memset(buffer, '\0', strlen(buffer));                
                    strcpy(buffer, fileName);
                    charsRead = send(establishedConnectionFD2, buffer, strlen(buffer), 0); // Send success back                
                    
                    //recv message from client
                    memset(buffer, '\0', strlen(buffer));                  
                    charsRead = recv(establishedConnectionFD2, buffer, strlen(buffer), 0); // Read the client's message from the socket                

                    //2nd send file content
                    memset(buffer, '\0', strlen(buffer));    
                    //copy fileContent to buffer to be sent to client
                    strcpy(buffer, fileContent);                
                    charsRead = send(establishedConnectionFD2, buffer, strlen(buffer), 0); // Send success back
                }
                
                close(establishedConnectionFD2); // Close data connection
                close(listenSocketFD2); // Close the listening socket                
            }
            
            int errorCommand = 1;

            //if file sent
            if ((strcmp(command, "-g") == 0))
            {
                errorCommand = 0;
                strcpy(buffer, "File transfer complete.\n");
                charsRead = send(establishedConnectionFD, buffer, strlen(buffer), 0); // Send success back
                
            }
            
            if ((strcmp(command, "-l") == 0))
            {
                errorCommand = 0;
                strcpy(buffer, ">");
                charsRead = send(establishedConnectionFD, buffer, strlen(buffer), 0); // Send success back
                
            }            
            
            //clear buffer data
            memset(buffer, '\0', strlen(buffer));
            //strcat(buffer, "file sent!\n");
            
            //if wrong command then send back to client
            if (errorCommand == 1)
            {
                strcat(buffer, "invalid command\n");
                charsRead = send(establishedConnectionFD, buffer, strlen(buffer), 0); // Send success back
            }
            
            

            if (charsRead < 0) error("ERROR writing to socket");

            exit(1);

            //exit out of child
            default:
            if (1 == 1)
            {
            waitpid(-1, &childExitMethod, 0);
            spawnPid = -5;

            break;
            }
}
close(establishedConnectionFD); // Close the existing socket which is connected to the client


}


close(listenSocketFD); // Close the listening socket
return 0;
}