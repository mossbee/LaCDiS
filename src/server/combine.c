#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 3000
#define MAX_WORD_SIZE 100

typedef struct {
    char word[MAX_WORD_SIZE];
    int count;
} KeyValue;

void Reduce(KeyValue* intermediate1, int intermediateSize1, KeyValue* intermediate2, int intermediateSize2, const char* outputFile) {
    FILE* file = fopen(outputFile, "w");  // Open in "a" mode for appending
    if (file == NULL) {
        perror("Error opening output file");
        return;
    }

    // Combine the word counts from both intermediate lists
    for (int i = 0; i < intermediateSize1; i++) {
        for (int j = 0; j < intermediateSize2; j++) {
            if (strcmp(intermediate1[i].word, intermediate2[j].word) == 0) {
                intermediate1[i].count += intermediate2[j].count;
                intermediate2[j].count = 0;  // Mark as combined
                break;
            }
        }
        fprintf(file, "%s %d\n", intermediate1[i].word, intermediate1[i].count);
    }

    // Write remaining entries from intermediate2 that were not combined
    for (int i = 0; i < intermediateSize2; i++) {
        if (intermediate2[i].count != 0) {
            fprintf(file, "%s %d\n", intermediate2[i].word, intermediate2[i].count);
        }
    }

    fclose(file);
}



int main(int argc, char* argv[]) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    if (argc != 4) {
        printf("Usage: %s <input_file1> <input_file2> <output_file>\n", argv[0]);
        return 1;
    }

    FILE* inputFile1 = fopen(argv[1], "r");
    if (inputFile1 == NULL) {
        perror("Error opening input file 1");
        return 1;
    }

    FILE* inputFile2 = fopen(argv[2], "r");
    if (inputFile2 == NULL) {
        perror("Error opening input file 2");
        return 1;
    }

    int intermediateSize1 = 0;
    int intermediateSize2 = 0;

    KeyValue intermediate1[MAX_SIZE];
    KeyValue intermediate2[MAX_SIZE];

    int count;

    // Read and parse intermediate results from input file 1
    while (fscanf(inputFile1, "%s %d", intermediate1[intermediateSize1].word, &count) != EOF) {
        intermediate1[intermediateSize1].count = count;
        intermediateSize1++;
    }

    // Read and parse intermediate results from input file 2
    while (fscanf(inputFile2, "%s %d", intermediate2[intermediateSize2].word, &count) != EOF) {
        intermediate2[intermediateSize2].count = count;
        intermediateSize2++;
    }


    fclose(inputFile1);
    fclose(inputFile2);

    // Combine and write the results to the output file
    Reduce(intermediate1, intermediateSize1, intermediate2, intermediateSize2, argv[3]);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%s took %f seconds to execute.\n", argv[0], cpu_time_used);
    return 0;
}
