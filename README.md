# LaCDiS: LAN-based Client-server Distributed WordCount System with C

A project to simulate a distributed word count system using C programming language. 

We created a simple distributed word count system using the client-server model. The server divides the data into two parts and sends one part to the client for processing through LAN communication and a client-server program written in C.The client counts the number of words in the data and sends the result back to the server. The server combines the results from the client and the server itself to get the final result.

The project was developed for the course "Computer Network" at Hanoi University of Science and Technology. Main purpose of the project is to understand the basic concepts of network programming and the client-server model. Another goal is to understand the basic concepts of distributed systems and the mechanism of data division and combination in distributed systems thorugh MapReduce algorithm.

## Process

The process of the simulation for multiple clients:

```
                :   ... (more) ...    :
                |                     |
                |    _______  COUNT   |
                :--> CLIENT  ---------:
                |    _______          |
                |                     |
                |    _______  COUNT   |
                :--> CLIENT  ---------:
______  DIVIDE* |    _______          |   COMBINE*    _______
INPUT  ---------|                     :-------------> OUTPUT
______          |    _______          |               _______
                '--> SERVER ----------'
                     _______  COUNT*
                        
*: Process in server.
```
To simulate the distributed word count system, you need two computers: one for the server and one for the client. Perform the following steps:

Copy the source code in the `src` directory to the server and client corresponding to the `src/server` and `src/client` directories with the `Makefile`. When the source code is available on the two simulation computers, compile the program as follows.

### Makefile

```bash
## If you are using make on Linux
# On the server
make server
# On the client
make client

## If you are using mingw32-make
# On the server
mingw32-make server
# On the client
mingw32-make client
```

After compiling, the simulation will start from the client side (the client is idle and waiting to be assigned a task).



### Client

On the client side, the tasks to be processed are controlled by `control`, so just call

```bash
./control SERVER_IP
# Ex: ./control 192.168.2.110
```

and wait for the data to be received, processed, and sent back to the server.

### Server

The tasks to be processed in the simulation are controlled by `control_program` on the server. To start running

```bash
./control_program
# Prompt:
#     Enter the name of the input file: ../../data/fulltext.txt
```

When the control program is running, you will be asked for the path to the text file used in this word count simulation. After entering the path to the file, the program will start dividing the data into 2 parts and send one part of the data to the client while the server also performs word count calculations and waits for the result to return from the client.

### Processing programs

#### `data-divide` - Data division

Split the data into two parts with approximately the same number of words.

```bash
./data-divide INPUT OUTPUT1 OUTPUT2
# Ex: ./data-divide fulltext.txt divided1.txt divided2.txt
```



#### `combine` - Hợp nhất dữ liệu

Combine the calculation results from multiple computers.

```bash
./combine INPUT1 INPUT2 OUTPUT
# Ex: ./combine count1.txt count2.txt count.txt
```



#### `wordcount` - Words count

Count the number of occurrences of each word in the text file.

```bash
./wordcount INPUT OUTPUT
# Ex: ./wordcount divided1.txt count1.txt
```



#### `client` - Receive and send files from the client side

The file transfer bridge between the server and the client with the mechanism: the error sending file will be resent until successful; connection to the server fails will try to reconnect after a period of time.

```bash
./client send FILENAME [SERVER_IP]
./client receive FILENAMETOSAVE [SERVER_IP]
# Ex: ./client send count2.txt 192.168.2.110
#     ./client receive divided2.txt 192.168.2.110
```



#### `server` - Receive and send files from the server side

The file transfer bridge between the server and the client.

```bash
./server send FILENAME
./server receive FILENAMETOSAVE
# Ex: ./server send divided2.txt
#     ./server receive count2.txt 
```
