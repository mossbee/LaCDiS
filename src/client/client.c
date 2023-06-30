#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 8888
#define BUFFER_SIZE 8096

const char* SERVER_IP = "169.254.175.107"; // Replace with your server IP

void send_file(FILE *fp, SOCKET serverSocket) {
    int n;
    char data[BUFFER_SIZE];

    while ((n = fread(data, 1, sizeof(data), fp)) > 0) {
        if (send(serverSocket, data, n, 0) < 0) {
            perror("Error in sending file.");
            exit(1);
        }
        memset(data, 0, sizeof(data));
    }
}

void receive_file(FILE *fp, SOCKET serverSocket) {
    int n;
    char data[BUFFER_SIZE];

    while ((n = recv(serverSocket, data, sizeof(data), 0)) > 0) {
        fwrite(data, 1, n, fp);
        memset(data, 0, sizeof(data));
    }
}

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr;

    // Check the command-line argument
    if (argc < 3) {
        printf("Usage: %s send|receive filename [SERVER_IP]\n", argv[0]);
        return 1;
    }
    else if (argc == 4) {
        SERVER_IP = argv[3];
    }

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Failed to initialize winsock");
        exit(1);
    }

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Failed to create socket");
        exit(1);
    }

    // Configure server settings
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    int flag = 0;
    while (1) {
        // Connect to the server
        if (connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            Sleep(10);
            if (!flag) {
                flag = 1;
                printf("Reconnect every 10 ms...\n");
            }
            continue; // Continue to the next iteration of the loop to retry
        }

        printf("Connected to server.\n");

        if (strcmp(argv[1], "send") == 0) {
            // Open file to send
            FILE *sendFile = fopen(argv[2], "rb");
            if (sendFile == NULL) {
                perror("Error in opening file for sending.");
                exit(1);
            }

            // Send file to the server
            send_file(sendFile, serverSocket);
            printf("File sent successfully.\n");

            fclose(sendFile);
        } else if (strcmp(argv[1], "receive") == 0) {
            // Receive file from the server
            FILE *receiveFile = fopen(argv[2], "wb");
            if (receiveFile == NULL) {
                perror("Error in creating file for receiving.");
                exit(1);
            }

            receive_file(receiveFile, serverSocket);
            printf("File received successfully.\n");

            fclose(receiveFile);
        } else {
            printf("Invalid argument. Use 'send' or 'receive'.\n");
        }

        // Close the socket
        closesocket(serverSocket);
        break; // Break out of the loop if the file transfer is successful
    }

    WSACleanup();
    printf("Client closed.\n");

    return 0;
}
