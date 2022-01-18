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

    char spam[500];
    fgets(spam, 500, accountStorage);

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
    if (!activeFile) return -1;

    char spam[500];
    fgets(spam, 500, activeFile);

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
    FILE* new = fopen(ACTIVE_ACCOUNT_STORAGE_TMP_PATH, "w");

    char spam[500];
    fgets(spam, 500, old);
    fprintf(new, "%s\n", "username");

    char _username[45];
    while (fscanf(old, "%s", _username) != EOF) {
        if (strcmp(_username, username)) {
            fprintf(new, "%s\n", _username);
        }
    }

    remove(ACTIVE_ACCOUNT_STORAGE_PATH);
    rename(ACTIVE_ACCOUNT_STORAGE_TMP_PATH, ACTIVE_ACCOUNT_STORAGE_PATH);

    fclose(old);
    fclose(new);

    return 1;
}

int saveRoom(struct Room room) {
    FILE* roomFile = fopen(ROOM_STORAGE_PATH, "a");

    if (roomFile == NULL) return -1;

    fprintf(roomFile, "%s %s %s %s %d %d\n", room.hostName, room.roomName, room.questionFile,
                                            room.answerFile, room.status, room.numOfStudents);

    fclose(roomFile);
    return 1;
}

int deleteRoom(char* roomName) {
    if (!roomName) return -1;

    FILE* old = fopen(ROOM_STORAGE_PATH, "r");
    FILE* new = fopen(ROOM_STORAGE_TMP_PATH, "w");

    if (!old || !new) return -1;

    char spam[500];
    fgets(spam, 500, old);
    fprintf(new, "%s\n", "hostName roomName questionFile answerFile status numOfStudents");

    struct Room room;
    while (fscanf(old, "%s %s %s %s %d %d", room.hostName, room.roomName, room.questionFile, 
                                room.answerFile, &room.status, &room.numOfStudents) != EOF) {
        if (strcmp(roomName, room.roomName)) {
            fprintf(new, "%s %s %s %s %d %d\n", room.hostName, room.roomName, room.questionFile,
                                room.answerFile, room.status, room.numOfStudents);
        }
    }

    char tmp[100];
    strcpy(tmp, "./server/result_");
    strcat(tmp, roomName);
    remove(tmp);

    remove(ROOM_STORAGE_PATH);
    rename(ROOM_STORAGE_TMP_PATH, ROOM_STORAGE_PATH);

    fclose(old);
    fclose(new);
    return 1;
}

int updateRoom(struct Room* room) {
    if (!room) return -1;

    FILE* old = fopen(ROOM_STORAGE_PATH, "r");
    FILE* new = fopen(ROOM_STORAGE_TMP_PATH, "w");

    if (!old || !new) return -1;

    char spam[500];
    fgets(spam, 500, old);
    fprintf(new, "%s\n", "hostName roomName questionFile answerFile status numOfStudents");

    struct Room tmp;
    while (fscanf(old, "%s %s %s %s %d %d", tmp.hostName, tmp.roomName, tmp.questionFile, 
                                    tmp.answerFile, &tmp.status, &tmp.numOfStudents) != EOF) {
        if (strcmp(room->roomName, tmp.roomName)) {
            fprintf(new, "%s %s %s %s %d %d\n", tmp.hostName, tmp.roomName, 
                                tmp.questionFile, tmp.answerFile, tmp.status, tmp.numOfStudents);
        } else {
            fprintf(new, "%s %s %s %s %d %d\n", room->hostName, room->roomName, room->questionFile,
                                room->answerFile, room->status, room->numOfStudents);
        }
    }

    remove(ROOM_STORAGE_PATH);
    rename(ROOM_STORAGE_TMP_PATH, ROOM_STORAGE_PATH);

    fclose(old);
    fclose(new);

    return 1;
}

struct Room* getRoomByRoomName(char* roomName) {
    FILE *f = fopen(ROOM_STORAGE_PATH, "r");

    char spam[500];
    fgets(spam, 500, f);

    struct Room* room = (struct Room*) malloc(sizeof(struct Room));
    while (fscanf(f, "%s %s %s %s %d %d", room->hostName, room->roomName, room->questionFile, 
                                    room->answerFile, &room->status, &room->numOfStudents) != EOF) {
        if (!strcmp(roomName, room->roomName))
            return room;
    }

    free(room);
    fclose(f);
    return NULL;
}

int getAllRooms(struct Room* output) {
    if (!output) output = (struct Room*) malloc(sizeof(struct Room) * 20);
    
    int count = 0;
    FILE *f = fopen(ROOM_STORAGE_PATH, "r");
    if (f == NULL || !output)  return -1;

    char spam[500];
    fgets(spam, 500, f);

	char hostName[45];
	char roomName[45];
	int status, numOfStudents;
	char questionFile[45];
	char answerFile[45];

    while (fscanf(f, "%s %s %s %s %d %d", hostName, roomName, questionFile, 
                                    answerFile, &status, &numOfStudents) != EOF) {
        struct Room tmp;
        tmp.status = status;
        tmp.numOfStudents = numOfStudents;
        strcpy(tmp.roomName, roomName);
        strcpy(tmp.questionFile, questionFile);
        strcpy(tmp.answerFile, answerFile);
        strcpy(tmp.hostName, hostName);

        output[count++] = tmp;
    }

    fclose(f);
    return count;
}

int getAllQuestions(char *file_path, char** output) {
    if (!file_path || !output)  return -1;
    char path[100] = "";
    sprintf(path, "./server/%s", file_path);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    
    char spam[100];
    fgets(spam, 100, f);

    int n = 0;
    char line[1000];
    
    while (fgets(line, 1000, f) != NULL) {
        if (strlen(line) > 0)
            line[strlen(line) - 1] = '\0';
        output[n] = (char*) malloc(sizeof(char) * 1000);
        strcpy(output[n++], line);
    }

    return n;
}

int getAllAnswers(char* file_path, char** output) {
    if (!file_path || !output)  return -1;
    char path[100] = "";
    sprintf(path, "./server/%s", file_path);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    
    char spam[100];
    fgets(spam, 100, f);

    int n = 0;
    char line[1000];
    
    while (fgets(line, 1000, f) != NULL) {
        if (strlen(line) > 0)
            line[strlen(line) - 1] = '\0';
        output[n] = (char*) malloc(sizeof(char) * 1000);
        strcpy(output[n++], line);
    }

    fclose(f);
    return n;
}

int saveResult(char* roomName, char* username, char* score) {
    if (!roomName | !username | !score) return -1;

    char fileName[100] = "./server/result_"; strcat(fileName, roomName);
    FILE* resultFile = fopen(fileName, "a+");
    if (!resultFile) return -1;

    fprintf(resultFile, "%s %s\n", username, score);

    fclose(resultFile);
    return 1;
}

int isSubmited(char* roomName, char* username) {
    if (!roomName) return -1;

    char fileName[100] = "./server/result_"; strcat(fileName, roomName);
    FILE* resultFile = fopen(fileName, "r");
    if (!resultFile) return -1;
    
    char _username[100], _score[100];

    while (fscanf(resultFile, "%s %s", _username, _score) != EOF)
        if (!strcmp(username, _username)) return 1;

    fclose(resultFile);
    return -1;
}

int getAllResult(char* roomName, char** output) {
    if (!roomName) return -1;

    char fileName[100] = "./server/result_"; strcat(fileName, roomName);
    FILE* resultFile = fopen(fileName, "r");
    if (!resultFile) return -1;
    
    int count = 0;
    char line[100] = "";

    while (fgets(line, 100, resultFile) != NULL) {
        if (strlen(line) > 0)
            line[strlen(line) - 1] = '\0';
        output[count] = (char*) malloc(sizeof(char) * 100);
        strcpy(output[count++], line);
    }

    fclose(resultFile);
    return count;
}