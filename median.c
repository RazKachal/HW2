#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORE 100
#define MIN_SCORE 0

// Function to compare integers for qsort
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    FILE *file = stdin; // Default to stdin
    char *path = NULL;

    // Check if a file path is provided
    if (argc == 2 && strcmp(argv[1], "-") != 0) {
        path = argv[1];
        file = fopen(path, "r");
        if (!file) {
            perror("Error opening file");
            return 1;
        }
    }

    int *scores = NULL;
    size_t size = 0, capacity = 10;
    scores = malloc(capacity * sizeof(int));
    if (!scores) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    char line[256];
    int line_number = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        char *endptr;
        int score = strtol(line, &endptr, 10);

        // Check for invalid input
        if (*endptr != '\n' && *endptr != '\0') {
            fprintf(stderr, "Error at line %d: invalid input %s", line_number, line);
            free(scores);
            if (file != stdin) fclose(file);
            return 1;
        }

        // Check if the score is within the valid range
        if (score < MIN_SCORE || score > MAX_SCORE) {
            fprintf(stderr, "Error at line %d: invalid input %s", line_number, line);
            free(scores);
            if (file != stdin) fclose(file);
            return 1;
        }

        // Resize the array if needed
        if (size == capacity) {
            capacity *= 2;
            scores = realloc(scores, capacity * sizeof(int));
            if (!scores) {
                fprintf(stderr, "Memory allocation error\n");
                if (file != stdin) fclose(file);
                return 1;
            }
        }

        // Add the score to the array
        scores[size++] = score;
    }

    if (file != stdin) fclose(file);

    // Sort the scores
    qsort(scores, size, sizeof(int), compare);

    // Calculate the median
    int median_index = (size + 1) / 2 - 1; // [(N+1)/2] - 1 for 0-based index
    printf("%d\n", scores[median_index]);

    free(scores);
    return 0;
}