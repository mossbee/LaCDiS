#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    char key[100];
    int value;
} KeyValuePair;

int cmpfunc(const void* a, const void* b) {
    return strcmp(((KeyValuePair*)a)->key, ((KeyValuePair*)b)->key);
}

void remove_punctuation(char *str) {
    int i, j;
    for (i = 0, j = 0; i < strlen(str); i++) {
        if (!ispunct(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void mapper(const char* input_file, KeyValuePair** output, int* output_size) {
    FILE* file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Failed to open the input file.\n");
        return;
    }

    char word[100];
    int word_count = 0;

    // Read words from the input file
    while (fscanf(file, "%s", word) == 1) {
        // Remove punctuation
        remove_punctuation(word);
        // Convert word to lowercase
        for (int i = 0; word[i] != '\0'; i++) {
            word[i] = tolower(word[i]);
        }

        // Emit key-value pair
        KeyValuePair* kv_pair = malloc(sizeof(KeyValuePair));
        strcpy(kv_pair->key, word);
        kv_pair->value = 1;

        (*output_size)++;
        *output = realloc(*output, (*output_size) * sizeof(KeyValuePair));
        (*output)[(*output_size) - 1] = *kv_pair;

        free(kv_pair);
    }

    fclose(file);
}

void reducer(KeyValuePair* input, int input_size, const char* output_file) {
    FILE* file = fopen(output_file, "w");
    if (file == NULL) {
        printf("Failed to open the output file.\n");
        return;
    }

    // Sort the input key-value pairs by key (word)
    qsort(input, input_size, sizeof(KeyValuePair), cmpfunc);

    char current_word[100];
    int current_count = 0;

    for (int i = 0; i < input_size; i++) {
        if (i == 0 || strcmp(input[i].key, current_word) != 0) {
            // A new word encountered
            if (i != 0) {
                // Write the word count to the output file
                fprintf(file, "%s: %d\n", current_word, current_count);
            }

            strcpy(current_word, input[i].key);
            current_count = input[i].value;
        } else {
            // Existing word, increment the count
            current_count += input[i].value;
        }
    }

    // Write the last word count to the output file
    fprintf(file, "%s: %d\n", current_word, current_count);

    fclose(file);
}

int main(int argc, char* argv[]) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    if (argc < 3) {
        printf("Usage: wordcount <input_file> <output_file>\n");
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    KeyValuePair* output = NULL;
    int output_size = 0;

    // Map phase
    mapper(input_file, &output, &output_size);

    // Reduce phase
    reducer(output, output_size, output_file);

    // Clean up
    free(output);

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%s took %f seconds to execute.\n", argv[0], cpu_time_used);

    return 0;
}