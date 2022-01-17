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
    if (accountStorage == NULL) return NULL;
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

int getAllQuestion(char *file_path, struct Question *quesList) {
    if (!file_path || !quesList)  return -1;
    char path[100] = "";
    sprintf(path, "./server/%s", file_path);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    
    int n = 0;
    char line[1000], delim[2] = "|";
    
    while (fgets(line, 1000, f) != NULL) {
        quesList[n].id = atoi(strtok(line, delim));
        strcpy(quesList[n].ques, strtok(NULL, delim));
        strcpy(quesList[n].choices[0], strtok(NULL, delim));
        strcpy(quesList[n].choices[1], strtok(NULL, delim));
        strcpy(quesList[n].choices[2], strtok(NULL, delim));
        strcpy(quesList[n].choices[3], strtok(NULL, delim));
        strcpy(quesList[n++].answer, strtok(NULL, delim));
    }
    return n;
}

int questionToString(struct Question question, char* buf) {
    if (!buf) return -1;
    
    sprintf(buf, "%d|%s|%s|%s|%s|%s|\n", question.id, question.ques,
                                        question.choices[0],
                                        question.choices[1],
                                        question.choices[2],
                                        question.choices[3]);
    return strlen(buf);
}