#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int split(char* str, char* delim, char** output) {
    if (!str || !delim || !output) return -1;

    char* token = strtok(str, delim);
    int count = 0;
    while (token) {
        output[count] = (char*) malloc(sizeof(char) * 100);
        strcpy(output[count++], token);
        token = strtok(NULL, delim);
    }

    return count;
}

struct Node* createNode(void* value) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->value = value;
    node->next = NULL;
    return node;
}

struct List* newList() {
    struct List* l = (struct List*) malloc(sizeof(struct List));
    l->head = NULL;
    l->count = 0;
    return l;
}

void addToList(struct List* l, void* value) {
    struct Node* temp = l->head;
    struct Node* node = createNode(value);
    if (l->head == NULL) {
        l->head = node;
        l->count++;
        return;
    }
    
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = node;
    l->count++;
}