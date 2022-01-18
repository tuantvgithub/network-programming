#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int split(char* str, char* delim, char** output) {
    if (!str || !delim || !output) return -1;

    char* token = strtok(str, delim);
    int count = 0;
    while (token) {
        output[count] = (char*) malloc(sizeof(char) * 1000);
        strcpy(output[count++], token);
        token = strtok(NULL, delim);
    }

    return count;
}

void freeArr(char** arr, int size) {
    if (!arr) return;
    for (int i = 0; i < size; i++)
        free(arr[i]);
}