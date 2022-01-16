#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"

int saveAccount(char* username, char* password, char* role) {
    if (!username || !password) return -1;
    if (!role) {
        role = (char*) malloc(sizeof(char) * 45);
        strcpy(role, "STUDENT");
    }

    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "a");
    if (accountStorage == NULL) return -1;

    fprintf(accountStorage, "\n%s %s %s", username, password, role);

    fclose(accountStorage);
    return 1;
}

struct Account* getAccountByUsername(char* username) {
    if (!username) return NULL;
    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "r");
    struct Account* account = (struct Account*) malloc(sizeof(struct Account));
    if (!account) return NULL;
    if (!accountStorage) return NULL;

    char _username[45];
    char _password[45];
    char _role[45];

    while (fscanf(accountStorage, "%s %s %s", _username, _password, _role) != EOF) {
        if (strcmp(_username, username) == 0) {
            strcpy(account->username, _username);
            strcpy(account->password, _password);
            strcpy(account->role, _role);
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


int saveRoom(struct Room room) {
    FILE* roomFile = fopen(ROOM_STORAGE_PATH, "a");

    if (roomFile == NULL) return -1;

    fprintf(roomFile, "\n%s %d %s %s %d", room.roomName, room.status, room.questionsFile,
                                            room.hostName, room.numOfPlayer);
    for (int i = 0; i<room.numOfPlayer; i++) {
        fprintf(roomFile, " %s", room.players[i]);
    }

    fclose(roomFile);
    return 1;
}

struct Room* getRoomByRoomName(char* roomName) {
    FILE *f = fopen(ROOM_STORAGE_PATH, "r");

    while (1) {
        struct Room* room = (struct Room*) malloc(sizeof(struct Room));
        if (fscanf(f, "%s %d %s %s %d", room->roomName,
                                        &room->status,
                                        room->questionsFile,
                                        room->hostName,
                                        &room->numOfPlayer) < 5) {
            break;
        }
        for (int i = 0; i<room->numOfPlayer; i++) {
            room->players[i] = (char*) malloc(sizeof(char)*45);
            fscanf(f, "%s", room->players[i]);
        }

        if (!strcmp(roomName, room->roomName)) {
            return room;
        }
    }
    fclose(f);
    return NULL;
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

// int main() {
//     struct Room* room = getRoomByRoomName("room2");
//     if (!room) {
//         printf("noooo\n");
//     } else {
//         printf("room: %s.\n", room->roomName);
//         printf("{\nname: %s,\nstatus: %d,\nquesFile: %s,\nhostName: %s,\nnum_players: %d\n}\n",
//             room->roomName, room->status, room->questionsFile, room->hostName, room->numOfPlayer);
//         for (int i = 0; i<room->numOfPlayer; i++) {
//             printf("player %d: %s\n", i+1, room->players[i]);
//         }
//     }

// }