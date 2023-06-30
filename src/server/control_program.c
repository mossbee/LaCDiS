#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 256

void executeCommand(const char* command) {
    int result = system(command);
    if (result == -1) {
        printf("Failed to execute command: %s\n", command);
        exit(1);
    }
}

int main() {
    char inputFilename[MAX_FILENAME_LENGTH];
    char dividedFile1[MAX_FILENAME_LENGTH] = "files/divided_file_1.txt";
    char dividedFile2[MAX_FILENAME_LENGTH] = "files/divided_file_2.txt";
    char processedFile1[MAX_FILENAME_LENGTH] = "files/processed_file_1.txt";
    char processedFile2[MAX_FILENAME_LENGTH] = "files/processed_file_2.txt";
    char combinedFile[MAX_FILENAME_LENGTH] = "output/combined_file.txt";

    clock_t start, end;
    double cpu_time_used;

    system("mkdir files");
    system("mkdir output");

    // Prompt for input file name
    printf("Enter the name of the input file: ");
    fgets(inputFilename, sizeof(inputFilename), stdin);
    inputFilename[strcspn(inputFilename, "\n")] = '\0';
    
    // Start the clock
    start = clock();
    
    // Divide the input file into two parts
    char divideCommand[MAX_FILENAME_LENGTH * 2 + 50];
    snprintf(divideCommand, sizeof(divideCommand), "data_divide %s %s %s", inputFilename, dividedFile1, dividedFile2);
    executeCommand(divideCommand);

    // Transfer divided file 2 to the other computer (using client.c and server.c)
    char transferCommand[MAX_FILENAME_LENGTH];
    snprintf(transferCommand, sizeof(transferCommand), "server send %s", dividedFile2);
    executeCommand(transferCommand);

    // Execute wordcount.c on the first computer (using processedFile1)
    char wordCountCommand1[MAX_FILENAME_LENGTH + 50];
    snprintf(wordCountCommand1, sizeof(wordCountCommand1), "wordcount %s %s", dividedFile1, processedFile1);
    executeCommand(wordCountCommand1);

    // Transfer processedFile1 back to the main computer (using client.c and server.c)

    // Transfer divided file 2 to the other computer (using client.c and server.c)

    // Transfer processedFile2 back to the main computer (using client.c and server.c)
    char receiveCommand[MAX_FILENAME_LENGTH];
    snprintf(receiveCommand, sizeof(receiveCommand), "server receive %s", processedFile2);
    executeCommand(receiveCommand);
    // Combine the processed files
    char combineCommand[MAX_FILENAME_LENGTH * 3 + 50];
    snprintf(combineCommand, sizeof(combineCommand), "combine %s %s %s", processedFile1, processedFile2, combinedFile);
    executeCommand(combineCommand);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Process completed successfully in %f seconds. Combined file: %s\n",  cpu_time_used, combinedFile);

    return 0;
}
