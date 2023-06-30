#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_SIZE 1000

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: ./data_devide <input_file> <output_file1> <output_file2>\n");
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE* outputFile1 = fopen(argv[2], "w");
    if (outputFile1 == NULL) {
        perror("Error opening output file 1");
        return 1;
    }

    FILE* outputFile2 = fopen(argv[3], "w");
    if (outputFile2 == NULL) {
        perror("Error opening output file 2");
        return 1;
    }

    char word[MAX_WORD_SIZE];
    int wordCount = 0;
    int totalWords = 0;

    // Count the total number of words in the input file
    while (fscanf(inputFile, "%s", word) != EOF) {
        totalWords++;
    }

    // Reset the file pointer to the beginning of the file
    fseek(inputFile, 0, SEEK_SET);

    // Divide the data into two halves
    while (fscanf(inputFile, "%s", word) != EOF) {
        FILE* currentOutputFile = wordCount < totalWords / 2 ? outputFile1 : outputFile2;
        fprintf(currentOutputFile, "%s ", word);
        wordCount++;
    }

    fclose(inputFile);
    fclose(outputFile1);
    fclose(outputFile2);

    return 0;
}
