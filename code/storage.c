#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

int saveAccount(char* username, char* password) {
    if (username == NULL || password == NULL) return -1;
    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "a");

    if (accountStorage == NULL) return -1;
    if (getAccountByUsername(username) != NULL) return -1;

    fprintf(accountStorage, "\n%s %s", username, password);

    fclose(accountStorage);
    return 1;
}

struct Account* getAccountByUsername(char* username) {
    if (username == NULL) return NULL;
    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "r");
    struct Account* account = (struct Account*) malloc(sizeof(struct Account));
    if (account == NULL) return NULL;

    char _username[45];
    char _password[45];
    while (fscanf(accountStorage, "%s %s", _username, _password) != EOF) {
        if (strcmp(_username, username) == 0) {
            strcpy(account->username, _username);
            strcpy(account->password, _password);
            return account;
        }
    }

    fclose(accountStorage);
    return NULL;
}


Node createNode(void* value) {
    Node node = (Node) malloc(sizeof(node));
    node->value = value;
    node->next = NULL;
    return node;
}

List newList() {
    List l;
    l.head = NULL;
    l.count = 0;
    return l;
}

void addEnd(List* l, void* value) {
    Node temp = l->head;
    Node node = createNode(value);
    if (l->head == NULL) {
        l->head = node;
        l->count++;
        return;
    }
    
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = node;
    l->count++;
}

List getAllQuestion(char *ques_file) {
    char line[500] = "";

    FILE *f = fopen(ques_file, "r");
    List l = newList();
    if (f == NULL)  return l;

    struct Question *ques;
    while(fgets(line, 500, f) != NULL){
        ques = (struct Question*) malloc(sizeof(struct Question));
        // printf("ques: %s..\n", line);
        ques->id = atoi(strtok(line, "|"));
        strcpy(ques->ques, strtok(NULL, "|"));
        strcpy(ques->choices[0], strtok(NULL, "|"));
        strcpy(ques->choices[1], strtok(NULL, "|"));
        strcpy(ques->choices[2], strtok(NULL, "|"));
        strcpy(ques->choices[3], strtok(NULL, "|"));
        strcpy(ques->answer, strtok(NULL, "|"));
        addEnd(&l, ques);
    }
    fclose(f);
    return l;
}

List getAllRoom(char *room_file) {
    FILE *f = fopen(room_file, "r");
    List l = newList();
    if (f == NULL)  return l;

    struct Room *r;
    char r_name[100];
    while (fscanf(f, "%[^\n]s", r_name) != EOF) {
        r = (struct Room*) malloc(sizeof(struct Room));
        strcpy(r->name, r_name);
        addEnd(&l, r);
    }
    fclose(f);
    return l;
}

// int main() {
//     List quesL = getAllQuestion("question.txt");
//     printf("ques count: %d..\n", quesL.count);
// }