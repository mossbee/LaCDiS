#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 8888
#define BUFFER_SIZE 8096

void send_file(FILE *fp, SOCKET clientSocket) {
    int n;
    char data[BUFFER_SIZE] = {0};

    while ((n = fread(data, 1, sizeof(data), fp)) > 0) {
        if (send(clientSocket, data, n, 0) < 0) {
            perror("Error in sending file.");
            exit(1);
        }
        memset(data, 0, sizeof(data));
    }
}

void receive_file(FILE *fp, SOCKET clientSocket) {
    int n;
    char data[BUFFER_SIZE];

    while ((n = recv(clientSocket, data, sizeof(data), 0)) > 0) {
        fwrite(data, 1, n, fp);
        memset(data, 0, sizeof(data));
    }
}

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrSize = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

    // Check the command-line argument
    if (argc != 3) {
        printf("Usage: %s send|receive [FILENAME]\n", argv[0]);
        return 1;
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
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Bind socket to IP and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        perror("Bind failed");
        exit(1);
    }

    printf("Binding success.\n");

    // Listen for client connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        perror("Listen failed");
        exit(1);
    }

    printf("Listening....\n");

    // Accept client connection
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) == INVALID_SOCKET) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    if (strcmp(argv[1], "send") == 0) {
        // Open file to send
        FILE *sendFile = fopen(argv[2], "rb");
        if (sendFile == NULL) {
            perror("Error in opening file for sending.");
            exit(1);
        }

        // Send file to the client
        send_file(sendFile, clientSocket);
        printf("File data sent successfully.\n");

        fclose(sendFile);
    } else if (strcmp(argv[1], "receive") == 0) {
        // Receive file from the client
        FILE *receiveFile = fopen(argv[2], "wb");
        if (receiveFile == NULL) {
            perror("Error in creating file for receiving.");
            exit(1);
        }

        receive_file(receiveFile, clientSocket);
        printf("File received successfully.\n");

        fclose(receiveFile);
    } else {
        printf("Invalid argument. Use 'send' or 'receive'.\n");
    }

    // Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    printf("Server closed.\n");

    return 0;
}
