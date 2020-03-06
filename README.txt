Name: Daniel Maynard
Project: Project 2

Instructions:

1. Run the Makefile by typing ./MakeFile
2. Start server by typing ./ftserver <port> (example: ./ftserver 8090)
3. Start client by typing python ftclient.py <host> <port> <command>
    Example: for listing files - python ftclient.py flip1 8090 -l
    Example: for downloading a file - python ftclient.py flip1 8090 -g test.txt 8091
4.To close the server use SIG INT: CTRL C

Note: For each connection that is made I adopted my CS344 project, which
creates a new child process for each connection, and exits the child/parent
after each command.