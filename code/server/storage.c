#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "storage.h"
#include "../util/utils.h"

int saveAccount(char* username, char* password, char* role) {
    if (!username || !password) return -1;
    if (!role) {
        role = (char*) malloc(sizeof(char) * 45);
        strcpy(role, "STUDENT");
    }

    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "a");
    if (!accountStorage) return -1;

    fprintf(accountStorage, "%s %s %s\n", username, password, role);

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

int accountIsActive(char* username) {
    if (!username) return -1;

    FILE* activeFile = fopen(ACTIVE_ACCOUNT_STORAGE_PATH, "r");

    char _username[45];
    while (fscanf(activeFile, "%s", _username) != EOF)
        if (!strcmp(_username, username))
            return 1;
    
    return -1;
}

int saveActiveAccount(char* username) {
    if (!username) return -1;

    FILE* activeFile = fopen(ACTIVE_ACCOUNT_STORAGE_PATH, "a");
    if (!activeFile) return -1;

    fprintf(activeFile, "%s\n", username);

    fclose(activeFile);
    return 1;
}

int deleteActiveAccount(char* username) {
    if (!username) return -1;

    FILE* old = fopen(ACTIVE_ACCOUNT_STORAGE_PATH, "r");
    FILE* new = fopen(ACTIVE_ACCOUNT_STORAGE_PATH_TMP, "w");

    char _username[45];
    while (fscanf(old, "%s", _username) != EOF) {
        if (strcmp(_username, username)) {
            fprintf(new, "%s\n", _username);
        }
    }

    remove(ACTIVE_ACCOUNT_STORAGE_PATH);
    rename(ACTIVE_ACCOUNT_STORAGE_PATH_TMP, ACTIVE_ACCOUNT_STORAGE_PATH);

    fclose(old);
    fclose(new);

    return 1;
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
        addToList(l, ques);
    }

    fclose(f);
    return l;
}


int saveRoom(struct Room room) {
    FILE* roomFile = fopen(ROOM_STORAGE_PATH, "a");

    if (roomFile == NULL) return -1;

    fprintf(roomFile, "\n%s %s %s %d", room.hostName, room.roomName, 
                                            room.questionsFile, room.status);

    fclose(roomFile);
    return 1;
}

struct Room* getRoomByRoomName(char* roomName) {
    FILE *f = fopen(ROOM_STORAGE_PATH, "r");

    while (1) {
        struct Room* room = (struct Room*) malloc(sizeof(struct Room));
        if (fscanf(f, "%s %s %s %d", room->hostName, room->roomName,
                                        room->questionsFile, &room->status) == EOF) {
            break;
        }

        if (!strcmp(roomName, room->roomName))
            return room;
    }

    fclose(f);
    return NULL;
}

int loadAllRooms(struct Room* output) {
    if (!output) output = (struct Room*) malloc(sizeof(struct Room) * 20);
    
    int count = 0;
    FILE *f = fopen(ROOM_STORAGE_PATH, "r");
    if (f == NULL || !output)  return -1;

	char hostName[45];
	char roomName[45];
	int status;
	char questionsFile[45];

    while (fscanf(f, "%s %s %s %d", hostName, roomName, questionsFile, &status) != EOF) {
        struct Room tmp;
        tmp.status = status;
        strcpy(tmp.roomName, roomName);
        strcpy(tmp.questionsFile, questionsFile);
        strcpy(tmp.hostName, hostName);

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