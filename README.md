The goal of this project is to design and implement a distributed system using inter-process communication (IPC) mechanisms, threads, and synchronization in C on Linux. The system will consist of a server that can handle requests from multiple clients simultaneously. The clients will communicate with the server using IPC mechanisms such as pipes, message passing, or shared memory.

Create directory on Desktop:
cd Desktop
mkdir detyra

Navigate to the project directory:
cd detyra

Create files:
gedit server.c
gedit client.c

Compile the server program:
gcc server.c -o server -lpthread
Compile the client program:
gcc client.c -o client

Usage
Start the server:
./server
The server will start running and wait for client connections.

Start a client:
./client
The client will connect to the server and send a request. The server will process the request and respond with a message. The client will display the received response on the terminal.

You can run multiple client instances simultaneously to test concurrent requests handling.
