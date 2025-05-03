// filepath: /home/mamat/Documents/HW2/my_grep.c
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024

int match(char *text, char *regexp);
int matchhere(char *text, char *regexp);
int matchstar(char *text, char c, char *regexp);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pattern> [file]\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    FILE *file = argc > 2 ? fopen(argv[2], "r") : stdin;

    if (!file) {
        perror("Error opening file");
        return 1;
    }

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
        return matchhere(text, regexp + 1);
    }
    do {
        if (matchhere(text, regexp)) {
            return 1;
        }
    } while (*text++ != '\0');
    return 0;
}

int matchhere(char *text, char *regexp) {
    if (regexp[0] == '\0') {
        return 1;
    }
    if (regexp[1] == '*') {
        return matchstar(text, regexp[0], regexp + 2);
    }
    if (regexp[0] == '$' && regexp[1] == '\0') {
        return *text == '\0';
    }
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text)) {
        return matchhere(text + 1, regexp + 1);
    }
    return 0;
}

int matchstar(char *text, char c, char *regexp) {
    do {
        if (matchhere(text, regexp)) {
            return 1;
        }
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}