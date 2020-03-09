Instructions:
1. To turn on the server type "python chatserver.py PORT"

 (Must use a port that is not already in use)
To turn on the client:
2. Compile by typing gcc "-o chatclient chatcliet.c"
3. To execute the program type "./chatclient localhost PORT"
4. Type a handle name (must be 10 characters or less)
5. Type a message to the server (must be 500 characters or under).
6. Wait for the server to send a message.
7. Server enters a message (must be 500 characters or under).
8. Server waits for message from client.
9. Repeat 5 - 8 or until client or server type "\quit"
10. Server shutdown the server with a SIGINT (CTRL C)
