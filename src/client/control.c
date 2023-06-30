#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 256

const char* SERVER_IP = "169.254.175.107";

void executeCommand(const char* command) {
    int result = system(command);
    if (result == -1) {
        printf("Failed to execute command: %s\n", command);
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    system("mkdir files");

    char dividedFile[MAX_FILENAME_LENGTH] = "files/divided_file.txt";
    char processedFile[MAX_FILENAME_LENGTH] = "files/processed_file.txt";

    if (argc == 2) {
        SERVER_IP = argv[1];
    }

    // Transfer divided file from the server computer (using client.c and server.c)
    char transferCommand[MAX_FILENAME_LENGTH];
    snprintf(transferCommand, sizeof(transferCommand), "client receive %s %s", dividedFile, SERVER_IP);
    executeCommand(transferCommand);

    // Execute wordcount.c on the client computer (using processedFile)
    char wordCountCommand[MAX_FILENAME_LENGTH + 50];
    snprintf(wordCountCommand, sizeof(wordCountCommand), "wordcount %s %s", dividedFile, processedFile);
    executeCommand(wordCountCommand);

    // Transfer processed file to the server computer (using client.c and server.c)
    snprintf(transferCommand, sizeof(transferCommand), "client send %s %s", processedFile, SERVER_IP);
    executeCommand(transferCommand);

    printf("Process completed successfully.\n");

    return 0;
}