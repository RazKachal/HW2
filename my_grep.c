#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024

int match(char *text, char *regexp);
int matchhere(char *text, char *regexp);
int matchstar(char *text, char c, char *regexp);

int main(int argc, char *argv[]) {
    FILE *file = stdin; // Default to stdin

    if (argv[1][0] != '-' || argv[1][1] != '\0') {
        file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error occurred\n");
            return 1;
        }
    }


    // Check if a pattern is provided
    char *pattern = argv[2];
    if (!pattern || *pattern == '\0') { // Check for empty pattern
        fprintf(stderr, "Error: empty pattern\n");
        return 1;
    }

    // Read the file line by line
    char buf[BUFSIZE];
    while (fgets(buf, BUFSIZE, file)) {
        // Replace '\n' with '\0' manually
        char *newline = buf;
        while (*newline != '\0') {
            if (*newline == '\n') {
                *newline = '\0';
                break;
            }
            newline++;
        }

        if (match(buf, pattern)) {
            printf("%s\n", buf);
        }
    }

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

int match(char *text, char *regexp) {
    if (regexp[0] == '^') {
        return matchhere(text, regexp + 1); // Match from start of line
    }
    do {
        if (matchhere(text, regexp)) {
            return 1; // Match anywhere in the line
        }
    } while (*text++ != '\0');
    return 0; // No match
}

int matchhere(char *text, char *regexp) {
    if (regexp[0] == '\0') {
        return 1; // End of pattern
    }
    if (regexp[1] == '*') {
        return matchstar(text, regexp[0], regexp + 2); // Handle '*'
    }
    if (regexp[0] == '$' && regexp[1] == '\0') {
        return *text == '\0'; // Match end of line
    }
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) {
        return matchhere(text + 1, regexp + 1); // Match single character
    }
    return 0; // No match
}

int matchstar(char *text, char c, char *regexp) {
    do {
        if (matchhere(text, regexp)) {
            return 1;
        }
    } while (*text != '\0' && (c == '.' || *text++ == c));
    return 0;
}