// filepath: /home/mamat/Documents/HW2/my_grep.c
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024

int match(char *text, char *regexp);
int matchhere(char *text, char *regexp);
int matchstar(char *text, char c, char *regexp);

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
    // Check if a pattern is provided
    char *pattern = argv[2];
    if (!pattern) {
        fprintf(stderr, "Usage: %s <pattern> [<file>]\n", argv[0]);
        return 1;
    }
    // Check if the pattern is empty
    if (strlen(pattern) == 0) {
        fprintf(stderr, "Error: empty pattern\n");
        return 1;
    }
    // Check if the pattern is a valid regular expression
    for (int i = 0; pattern[i] != '\0'; i++) {
        if (pattern[i] == '*' || pattern[i] == '$' || pattern[i] == '^') {
            fprintf(stderr, "Error: invalid pattern %s\n", pattern);
            return 1;
        }
    }
    // Check if the file is opened successfully
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    // Read the file line by line
    char buf[BUFSIZE];
    while (fgets(buf, BUFSIZE, file)) {
        buf[strcspn(buf, "\n")] = '\0'; // Replace '\n' with '\0'
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
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}