#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void printError(char* error) {
    if (!error || strlen(error) < 1) return;
    printf("\n\n-- Error\n");
    printf("---> %s\n", error);
    printf("--\n\n");
}

char* subString(char* str, int start, int len) {
    // printf("str: %s\n start: %d\n len: %d\n", str, start, len);
    if (!str || start < 0) return NULL;

    char* result = (char*) malloc(sizeof(char) * len);
    int n = len < strlen(str) ? len : strlen(str);
    int c = 0;
    for (int i = start; i < n; i++)
        result[c++] = str[i];
    result[c] = '\0';

    return result;
}