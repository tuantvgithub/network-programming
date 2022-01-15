#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"

int saveAccount(char* username, char* password) {
    if (username == NULL || password == NULL) return -1;
    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "a");

    if (accountStorage == NULL) return -1;

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

void addEnd(struct List* l, void* value) {
    struct Node* temp = l->head;
    struct Node* node = createNode(value);
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

struct List* getAllQuestion(char *quesFile) {
    char line[500] = "";

    FILE *f = fopen(quesFile, "r");
    struct List* l = newList();
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
        addEnd(l, ques);
    }

    fclose(f);
    return l;
}

int loadAllRooms(struct Room* output) {
    if (!output) output = (struct Room*) malloc(sizeof(struct Room) * 20);
    
    int count = 0;
    FILE *f = fopen(ROOM_STORAGE_PATH, "r");
    if (f == NULL || !output)  return -1;

	char roomName[45]; 
	int status;
	char questionsFile[45];
	char hostName[45];
	int numOfPlayer;

    while (fscanf(f, "%s %d %s %s %d", roomName, &status, 
                            questionsFile, hostName, &numOfPlayer) != EOF) {
        struct Room tmp;
        tmp.status = status;
        tmp.numOfPlayer = numOfPlayer;
        strcpy(tmp.roomName, roomName);
        strcpy(tmp.questionsFile, questionsFile);
        strcpy(tmp.hostName, hostName);

        for (int i = 0; i < numOfPlayer; i++) {
            tmp.players[i] = (char*) malloc(sizeof(char) * 45);
            fscanf(f, "%s", tmp.players[i]);
        }

        output[count++] = tmp;
    }

    fclose(f);
    return count;
}

int getAllOnRooms(struct Room* roomArr, int size, struct Room* output) {
    if (!roomArr) return -1;
    if (!output) output = (struct Room*) malloc(sizeof(struct Room) * 20);
    
    return -1;
}