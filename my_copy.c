#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    FILE *input = stdin;  // Default to stdin
    FILE *output = stdout; // Default to stdout

    // Open input file if the first argument is not "-"
    if (argv[1][0] != '-' || argv[1][1] != '\0') {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error occurred\n");
            return 1;
        }
    }

    // Open output file if the second argument is not "-"
    if (argv[2][0] != '-' || argv[2][1] != '\0') {
        output = fopen(argv[2], "w");
        if (!output) {
            fprintf(stderr, "Error occurred\n");
            if (input != stdin) fclose(input);
            return 1;
        }
    }

    // Copy content from input to output
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        if (fwrite(buffer, 1, bytes_read, output) != bytes_read) {
            fprintf(stderr, "Error occurred\n");
            if (input != stdin) fclose(input);
            if (output != stdout) fclose(output);
            return 1;
        }
    }

    // Check for read errors
    if (ferror(input)) {
        fprintf(stderr, "Error occurred\n");
        if (input != stdin) fclose(input);
        if (output != stdout) fclose(output);
        return 1;
    }

    // Close files if they are not stdin or stdout
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}